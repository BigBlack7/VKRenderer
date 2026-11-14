#include <vulkan/vulkan_raii.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <string>
#include <cstdint>
#include <vector>
#include <utility>
#include <iterator>

constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;

#ifdef NDEBUG
constexpr bool kEnableValidationLayers = false;
#else
constexpr bool kEnableValidationLayers = true;
#endif

/*
Instance
   |
   +-- Debug Messenger
   |
   +-- Physical Device
            |
            v
       Logical Device
            |
            v
       Graphics Queue
*/

class HelloTriangleApplication
{
public:
    void run()
    {
        try
        {
            initWindow();
            initVulkan();
            mainLoop();
        }
        catch (...)
        {
            cleanup();
            throw;
        }
        cleanup();
    }

private:
    GLFWwindow *mWindow{nullptr};
    vk::raii::Context mContext;
    vk::raii::Instance mInstance{nullptr};
    vk::raii::DebugUtilsMessengerEXT mDebugMessenger{nullptr};
    vk::raii::PhysicalDevice mPhysicalDevice{nullptr};
    vk::raii::Device mDevice{nullptr};
    vk::raii::Queue mGraphicsQueue{nullptr};

    void initWindow()
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        mWindow = glfwCreateWindow(kWindowWidth, kWindowHeight, "Vulkan", nullptr, nullptr);
        if (!mWindow)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    void initVulkan()
    {
        createInstance();
        setupDebugMessenger();
        pickPhysicalDevice();
        createLogicalDevice();
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(mWindow))
        {
            glfwPollEvents();
        }
    }

    void cleanup() noexcept
    {
        if (mWindow)
        {
            glfwDestroyWindow(mWindow);
            mWindow = nullptr;
        }
        glfwTerminate();
    }

    void createInstance()
    {
        // 获取所需的 Extension 列表及其数量, 并检查当前运行环境是否支持这些 Extension, 缺少任何 Extension 则抛出异常
        const std::vector<const char *> extensions = getRequiredInstanceExtensions();
        const std::vector<vk::ExtensionProperties> availableExtensions = mContext.enumerateInstanceExtensionProperties();
        for (const char *required : extensions)
        {
            const bool supported = std::ranges::any_of(
                availableExtensions, [required](const vk::ExtensionProperties &extension)
                { return std::strcmp(extension.extensionName, required) == 0; });

            if (!supported)
            {
                throw std::runtime_error(std::string("Missing required Vulkan instance extension: ") + required);
            }
        }

        // 获取当前运行环境可枚举的 Instance Layer 信息并检查是否包含所需的验证层, 缺少任何验证层则抛出异常
        std::vector<const char *> requiredLayers{};
        if (kEnableValidationLayers)
        {
            requiredLayers.emplace_back("VK_LAYER_KHRONOS_validation");
        }
        if (!requiredLayers.empty())
        {
            const std::vector<vk::LayerProperties> availableLayers = mContext.enumerateInstanceLayerProperties();
            for (const char *required : requiredLayers)
            {
                const bool layerFound = std::ranges::any_of(
                    availableLayers, [required](const vk::LayerProperties &layer)
                    { return std::strcmp(layer.layerName, required) == 0; });

                if (!layerFound)
                {
                    throw std::runtime_error(std::string("Missing required Vulkan validation layer: ") + required);
                }
            }
        }

        constexpr vk::ApplicationInfo appInfo{
            .pApplicationName = "Hello Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14};

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
            .ppEnabledLayerNames = requiredLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()};

        mInstance = vk::raii::Instance(mContext, createInfo);
    }

    std::vector<const char *> getRequiredInstanceExtensions()
    {
        std::vector<const char *> extensions{};

        // 获取 GLFW 所需的 Vulkan 实例扩展及其数量
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        if (!glfwExtensions || glfwExtensionCount == 0)
        {
            throw std::runtime_error("Failed to get required GLFW extensions");
        }
        // 将 GLFW 所需的扩展添加到 extensions 向量中
        extensions.insert(extensions.end(), glfwExtensions, glfwExtensions + glfwExtensionCount);

        // 当启用验证层时, 追加 VK_EXT_debug_utils 扩展
        if (kEnableValidationLayers)
        {
            extensions.emplace_back(vk::EXTDebugUtilsExtensionName);
        }
        return extensions;
    }

    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity, vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT *callbackData, void *userData)
    {
        (void *)userData; // Unused parameter
        std::cerr << "Type: " << vk::to_string(type) << ", Severity: " << vk::to_string(severity) << ", Message: " << callbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    void setupDebugMessenger()
    {
        if (!kEnableValidationLayers)
            return;

        vk::DebugUtilsMessengerCreateInfoEXT messengerInfo{
            .messageSeverity =
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,

            .messageType =
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,

            .pfnUserCallback = &debugCallback};

        mDebugMessenger = mInstance.createDebugUtilsMessengerEXT(messengerInfo);
    }

    void pickPhysicalDevice()
    {
        // 枚举-筛选-保存可用的物理设备, 并选择一个合适的物理设备
        std::vector<vk::raii::PhysicalDevice> physicalDevices = mInstance.enumeratePhysicalDevices();
        if (physicalDevices.empty())
        {
            throw std::runtime_error("Failed to find GPUs with Vulkan support");
        }

        // 使用 std::ranges::find_if 查找第一个满足 isDeviceSuitable 条件的物理设备, 如果没有找到则抛出异常
        const auto it = std::ranges::find_if(physicalDevices, [this](const vk::raii::PhysicalDevice &device)
                                             { return isDeviceSuitable(device); });
        if (it == physicalDevices.end())
        {
            throw std::runtime_error("Failed to find a suitable GPU");
        }
        else
        {
            std::cout << "Selected GPU: " << it->getProperties().deviceName << std::endl;
        }
        mPhysicalDevice = std::move(*it);
    }

    bool isDeviceSuitable(const vk::raii::PhysicalDevice &device) const
    {
        // 检查物理设备是否满足应用程序的需求, 返回 true 表示适合, false 表示不适合
        const vk::PhysicalDeviceProperties properties = device.getProperties();

#ifndef NDEBUG
        std::cout << "===================================================================\n";
        std::cout << "Device Name: " << properties.deviceName << "\n";
        std::cout << "Device Type: " << vk::to_string(properties.deviceType) << "\n";
        std::cout << "API Version: " << VK_VERSION_MAJOR(properties.apiVersion) << "."
                  << VK_VERSION_MINOR(properties.apiVersion) << "."
                  << VK_VERSION_PATCH(properties.apiVersion) << "\n";
#endif

        // 检查 Vulkan API 版本是否满足要求, 要求至少支持 Vulkan 1.4
        const bool supportsRequiredApi = properties.apiVersion >= vk::ApiVersion14;
        if (!supportsRequiredApi)
        {
            std::cerr << "Device does not support required Vulkan API version 1.4\n";
            return false;
        }

        const std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
        if (queueFamilies.empty())
        {
            std::cerr << "Device has no queue families\n";
            return false;
        }

#ifndef NDEBUG
        std::cout << "Queue Family Count: " << queueFamilies.size() << "\n";
        for (const auto &queueFamily : queueFamilies)
        {
            std::cout << "..............................................................\n";
            std::cout << "Queue Family Index: " << &queueFamily - &queueFamilies[0] << "\n";
            std::cout << "Queue Count: " << queueFamily.queueCount << "\n";
            std::cout << "Queue Flags: " << vk::to_string(queueFamily.queueFlags) << "\n";
        }
        std::cout << "===================================================================\n";
#endif

        // 检查物理设备是否支持所需的队列族, 需要至少支持图形队列
        const bool supportsGraphics = std::ranges::any_of(queueFamilies, [](const vk::QueueFamilyProperties &family)
                                                          { return static_cast<bool>(family.queueFlags & vk::QueueFlagBits::eGraphics); });
        if (!supportsGraphics)
        {
            std::cerr << "Device does not support required queue families (Graphics)\n";
            return false;
        }

        // 检查物理设备是否支持所需的扩展, 需要至少支持 VK_KHR_swapchain 扩展
        const std::vector<vk::ExtensionProperties> requiredExtensions = device.enumerateDeviceExtensionProperties();
        const bool supportsRequiredExtensions = std::ranges::any_of(requiredExtensions, [](const vk::ExtensionProperties &extension)
                                                                    { return std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
        if (!supportsRequiredExtensions)
        {
            std::cerr << "Device does not support required extension: " << VK_KHR_SWAPCHAIN_EXTENSION_NAME << "\n";
            return false;
        }

        // 检查物理设备是否支持所需的特性, 需要至少支持 shaderDrawParameters、dynamicRendering 和 extendedDynamicState 特性
        const auto features = device.getFeatures2<
            vk::PhysicalDeviceFeatures2,
            vk::PhysicalDeviceVulkan11Features,
            vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
        const bool supportsRequiredFeatures =
            features.get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
            features.get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
            features.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;
        if (!supportsRequiredFeatures)
        {
            std::cerr << "Device does not support required features: shaderDrawParameters, dynamicRendering, extendedDynamicState\n";
            return false;
        }
        return true;
    }

    void createLogicalDevice()
    {
        // 获取支持图形操作的队列族索引, 以及队列信息
        const std::vector<vk::QueueFamilyProperties> queueFamilies = mPhysicalDevice.getQueueFamilyProperties();
        const auto it = std::ranges::find_if(queueFamilies, [](const vk::QueueFamilyProperties &family)
                                             { return static_cast<bool>(family.queueFlags & vk::QueueFlagBits::eGraphics); });
        if (it == queueFamilies.end())
        {
            throw std::runtime_error("Failed to find a suitable queue family for graphics");
        }
        const uint32_t graphicsFamilyIndex = static_cast<uint32_t>(std::distance(queueFamilies.begin(), it));

        const float queuePriority = 0.5f;
        vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = graphicsFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority};

        // 使用 StructureChain 在创建逻辑设备时启用所需的特性
        vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features,
                           vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
            featureChain = {{},
                            {.shaderDrawParameters = true},
                            {.dynamicRendering = true},
                            {.extendedDynamicState = true}};

        // 指定所需的设备扩展, 这里仅启用 VK_KHR_swapchain 扩展
        const std::vector<const char *> requiredDeviceExtensions{vk::KHRSwapchainExtensionName};

        vk::DeviceCreateInfo deviceCreateInfo{
            .pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size()),
            .ppEnabledExtensionNames = requiredDeviceExtensions.data()};

        mDevice = vk::raii::Device(mPhysicalDevice, deviceCreateInfo);
        mGraphicsQueue = vk::raii::Queue(mDevice, graphicsFamilyIndex, 0);
    }
};

int main()
{
    try
    {
        HelloTriangleApplication app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}