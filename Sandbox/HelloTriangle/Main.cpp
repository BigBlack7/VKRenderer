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

constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;

#ifdef NDEBUG
constexpr bool kEnableValidationLayers = false;
#else
constexpr bool kEnableValidationLayers = true;
#endif

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
        // 获取所需的 Extension 列表及其数量
        std::vector<const char *> extensions = getRequiredInstanceExtensions();

        // 获取当前运行环境可枚举的 Instance Layer 信息并检查是否包含所需的验证层, 缺少任何验证层则抛出异常
        const std::vector<vk::LayerProperties> availableLayers = mContext.enumerateInstanceLayerProperties();
        const std::vector<const char *> kValidationLayers{"VK_LAYER_KHRONOS_validation"};
        for (const char *layerName : kValidationLayers)
        {
            const bool layerFound = std::ranges::any_of(
                availableLayers, [layerName](const vk::LayerProperties &layer)
                { return std::strcmp(layer.layerName, layerName) == 0; });

            if (!layerFound)
            {
                throw std::runtime_error(std::string("Missing required Vulkan validation layer: ") + layerName);
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
            .enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size()),
            .ppEnabledLayerNames = kValidationLayers.data(),
            .enabledExtensionCount = (uint32_t)extensions.size(),
            .ppEnabledExtensionNames = std::move(extensions.data())};

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

        // 检查所需的扩展是否可用, 缺少任何扩展则抛出异常
        const std::vector<vk::ExtensionProperties> availableExtensions = mContext.enumerateInstanceExtensionProperties();
        for (uint32_t i = 0; i < glfwExtensionCount; ++i)
        {
            const char *required = glfwExtensions[i];

            const bool supported = std::ranges::any_of(
                availableExtensions, [required](const vk::ExtensionProperties &extension)
                { return std::strcmp(extension.extensionName, required) == 0; });

            if (!supported)
            {
                throw std::runtime_error(std::string("Missing required Vulkan instance extension: ") + required);
            }
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
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT *callbackData,
        void *userData)
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