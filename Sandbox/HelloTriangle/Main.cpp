#ifndef NOMINMAX
#define NOMINMAX
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
#include <optional>
#include <ranges>
#include <limits>
#include <filesystem>
#include <fstream>
#include <array>
#include <cstddef>

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
  +--- Surface -------------------- GLFW Window
  |        ^
  |        |
  |     Swapchain
  |
  +--- Physical Device
            |
            v
        Logical Device
            |
            +--- Graphics/Present Queue
            |
            +--- Swapchain
                      |
                      v
                Swapchain Images
*/

/*  SPIR-V 二进制文件的结构
    SPIR-V 物理结构 · 前 5 个 Word
    Word 0: Magic Number (0x07230203)
    Word 1: Version Number (0x00010000)
    Word 2: Generator 生成工具标识(例如 glslangValidator, SPIRV-Cross 等)
    Word 3: Bound 结果 ID 的排他上界(指令中使用的 ID 的最大值 + 1)
    Word 4: Schema (保留字段, 当前版本为 0)
*/

class HelloTriangleApplication
{
    // ========================Member variables========================
private:
    GLFWwindow *mWindow{nullptr};
    vk::raii::Context mContext;
    vk::raii::Instance mInstance{nullptr};
    vk::raii::DebugUtilsMessengerEXT mDebugMessenger{nullptr};
    vk::raii::SurfaceKHR mSurface{nullptr};
    vk::raii::PhysicalDevice mPhysicalDevice{nullptr};
    vk::raii::Device mDevice{nullptr};
    vk::raii::Queue mGraphicsQueue{nullptr};
    vk::raii::SwapchainKHR mSwapChain{nullptr};

    std::vector<vk::Image> mSwapChainImages;
    std::vector<vk::raii::ImageView> mSwapChainImageViews;

    vk::SurfaceFormatKHR mSwapChainSurfaceFormat{};
    vk::Extent2D mSwapChainExtent{};

    // ========================Member functions========================
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
    [[nodiscard]] static std::filesystem::path getExecutableDirectory()
    {
        std::wstring buffer(260, L'\0');

        while (true)
        {
            const DWORD length = GetModuleFileNameW(
                nullptr,
                buffer.data(),
                static_cast<DWORD>(buffer.size()));

            if (length == 0)
            {
                throw std::runtime_error("Failed to get executable path");
            }

            if (length < buffer.size())
            {
                buffer.resize(length);

                return std::filesystem::path(buffer).parent_path();
            }

            if (buffer.size() >= 32768)
            {
                throw std::runtime_error("Executable path is too long");
            }

            buffer.resize(std::min(buffer.size() * 2, std::size_t{32768}));
        }
    }

    [[nodiscard]] static std::vector<uint32_t> readSpirv(const std::filesystem::path &path)
    {
        // 1. 打开 SPIR-V 文件, 并以二进制模式读取文件内容, 同时将文件指针移动到文件末尾以获取文件大小
        // Windows 文本模式可能进行换行等转换, 而 SPIR-V 必须保留原始二进制内容, 不能使用普通文本读取方式
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open SPIR-V file");
        }
        const std::streamoff fileSize = file.tellg();

        // 2. 检查文件大小是否为 4 的倍数, 因为 SPIR-V 文件是以 32 位字为单位存储的
        std::vector<uint32_t> code(static_cast<std::size_t>(fileSize) / sizeof(uint32_t));
        if (!std::in_range<std::size_t>(fileSize) || !std::in_range<std::streamsize>(fileSize))
        {
            throw std::runtime_error("SPIR-V file is too large");
        }

        // 3. 将文件指针移动到文件开头, 并读取整个文件内容到 code 向量中
        if (!file.seekg(0, std::ios::beg) || !file.read(reinterpret_cast<char *>(code.data()), static_cast<std::streamsize>(fileSize)))
        {
            throw std::runtime_error("Failed to read complete SPIR-V file");
        }

        // 4. 检查 SPIR-V 文件的魔数是否正确, 以确保文件格式有效
        constexpr uint32_t kSpirvMagicNumber = 0x07230203;
        if (code.front() != kSpirvMagicNumber)
        {
            throw std::runtime_error("Invalid SPIR-V magic number");
        }
        return code;
    }

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
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createGraphicsPipeline();
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
        mSwapChainImageViews.clear();
        mSwapChainImages.clear();
        mSwapChain.clear();
        mSurface.clear(); // 先释放 Surface, 再销毁其对应的 GLFW Window
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

    void createSurface()
    {
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        if (glfwCreateWindowSurface(static_cast<VkInstance>(*mInstance), mWindow, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface");
        }
        mSurface = vk::raii::SurfaceKHR(mInstance, surface);
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

        // 检查物理设备是否支持所需的队列族, 需要至少同时支持图形&呈现(现阶段避免跨队列同步)
        const bool supports = std::ranges::any_of(std::views::iota(size_t{0}, queueFamilies.size()), [&](size_t i)
                                                  { return (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) && device.getSurfaceSupportKHR(static_cast<uint32_t>(i), *mSurface); });
        if (!supports)
        {
            std::cerr << "Device does not support required queue families for graphics and present\n";
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
        // 获取支持图形操作&呈现的队列族索引, 以及队列信息
        const std::vector<vk::QueueFamilyProperties> queueFamilies = mPhysicalDevice.getQueueFamilyProperties();
        std::optional<uint32_t> graphicsPresentFamilyIndex;
        for (uint32_t index = 0; index < queueFamilies.size(); ++index)
        {
            const bool graphics = static_cast<bool>(queueFamilies[index].queueFlags & vk::QueueFlagBits::eGraphics);
            const bool present = graphics && mPhysicalDevice.getSurfaceSupportKHR(index, *mSurface);
            if (present)
            {
                graphicsPresentFamilyIndex = index;
                break;
            }
        }
        if (!graphicsPresentFamilyIndex)
        {
            throw std::runtime_error("Failed to find a graphics and present queue family");
        }

#ifndef NDEBUG
        std::cout << "Graphics/Present Queue Family Index: " << graphicsPresentFamilyIndex.value() << '\n';
#endif

        const float queuePriority = 0.5f;
        vk::DeviceQueueCreateInfo queueCreateInfo{
            .queueFamilyIndex = graphicsPresentFamilyIndex.value(),
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
        mGraphicsQueue = vk::raii::Queue(mDevice, graphicsPresentFamilyIndex.value(), 0);
    }

    void createSwapChain()
    {
        // 图像数量, 尺寸, 用途等限制
        const vk::SurfaceCapabilitiesKHR capabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(*mSurface);

        // 可以使用什么像素格式与颜色空间
        const std::vector<vk::SurfaceFormatKHR> formats = mPhysicalDevice.getSurfaceFormatsKHR(*mSurface);

        // 图像如何排队等待呈现
        const std::vector<vk::PresentModeKHR> presentModes = mPhysicalDevice.getSurfacePresentModesKHR(*mSurface);

        // 选择交换链的图像数量, 尺寸, 像素格式与颜色空间, 呈现模式
        const uint32_t imageCount = chooseSwapMinImageCount(capabilities);
        mSwapChainSurfaceFormat = chooseSwapSurfaceFormat(formats);
        mSwapChainExtent = chooseSwapExtent(capabilities);
        if (mSwapChainExtent.width == 0 || mSwapChainExtent.height == 0)
        {
            throw std::runtime_error("Cannot create swapchain with zero extent");
        }
        const vk::PresentModeKHR presentMode = chooseSwapPresentMode(presentModes);

        // 检查所选的复合 Alpha 模式是否受支持, 如果不受支持则从 ePreMultiplied、ePostMultiplied、eInherit 中选出一个受支持的模式
        const auto supported = capabilities.supportedCompositeAlpha;
        vk::CompositeAlphaFlagBitsKHR compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        if (!static_cast<bool>(supported & compositeAlpha))
        {
            if (static_cast<bool>(supported & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied))
            {
                compositeAlpha = vk::CompositeAlphaFlagBitsKHR::ePreMultiplied;
            }
            else if (static_cast<bool>(supported & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied))
            {
                compositeAlpha = vk::CompositeAlphaFlagBitsKHR::ePostMultiplied;
            }
            else if (static_cast<bool>(supported & vk::CompositeAlphaFlagBitsKHR::eInherit))
            {
                compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eInherit;
            }
            else
            {
                throw std::runtime_error("No supported composite alpha mode found");
            }
        }

        vk::SwapchainCreateInfoKHR swapChainCreateInfo{
            .surface = *mSurface,
            .minImageCount = imageCount,
            .imageFormat = mSwapChainSurfaceFormat.format,
            .imageColorSpace = mSwapChainSurfaceFormat.colorSpace,
            .imageExtent = mSwapChainExtent,
            .imageArrayLayers = 1,
            .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
            .imageSharingMode = vk::SharingMode::eExclusive,
            .preTransform = capabilities.currentTransform,
            .compositeAlpha = compositeAlpha,
            .presentMode = presentMode,
            .clipped = VK_TRUE};

        mSwapChain = vk::raii::SwapchainKHR(mDevice, swapChainCreateInfo);
        mSwapChainImages = mSwapChain.getImages();

#ifndef NDEBUG
        std::cout << "===================================================================\n";
        std::cout << "Surface Min Image Count: " << capabilities.minImageCount << ", Max Image Count:" << capabilities.maxImageCount << '\n';
        std::cout << "Surface Current Extent: " << capabilities.currentExtent.width << "x" << capabilities.currentExtent.height << '\n';
        std::cout << "Selected Swapchain Format: " << vk::to_string(mSwapChainSurfaceFormat.format) << '\n';
        std::cout << "Selected Color Space: " << vk::to_string(mSwapChainSurfaceFormat.colorSpace) << '\n';
        std::cout << "Selected Present Mode: " << vk::to_string(presentMode) << '\n';
        std::cout << "Selected Swapchain Extent: " << mSwapChainExtent.width << "x" << mSwapChainExtent.height << '\n';
        std::cout << "Selected Composite Alpha: " << vk::to_string(compositeAlpha) << '\n';
        std::cout << "Requested Image Count: " << imageCount << '\n';
        std::cout << "Actual Swapchain Image Count: " << mSwapChainImages.size() << '\n';
#endif
    }

    uint32_t chooseSwapMinImageCount(const vk::SurfaceCapabilitiesKHR &capabilities) const
    {
        // 根据 Vulkan 规范, minImageCount 尽量请求至少三张图像
        uint32_t count = std::max(3u, capabilities.minImageCount);
        if (capabilities.maxImageCount != 0 && count > capabilities.maxImageCount)
        {
            count = capabilities.maxImageCount;
        }
        return count;
    }

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &formats) const
    {
        // 优先选择 VK_FORMAT_B8G8R8A8_SRGB 和 VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        for (const auto &format : formats)
        {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                return format;
            }
        }
        // 如果没有找到, 则返回第一个可用的格式
        return formats[0];
    }

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &modes) const
    {
        // 优先 Mailbox, 不支持即回退 FIFO
        for (const auto &mode : modes)
        {
            if (mode == vk::PresentModeKHR::eMailbox)
            {
                return mode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const
    {
        // Surface 已规定尺寸
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        // Surface 允许应用选择尺寸
        int width = 0, height = 0;
        glfwGetFramebufferSize(mWindow, &width, &height);
        vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }

    void createImageViews()
    {
        mSwapChainImageViews.reserve(mSwapChainImages.size());
        for (const auto &image : mSwapChainImages)
        {
            vk::ImageViewCreateInfo createInfo{
                .image = image,
                .viewType = vk::ImageViewType::e2D,
                .format = mSwapChainSurfaceFormat.format,
                .components = vk::ComponentMapping{
                    .r = vk::ComponentSwizzle::eIdentity,
                    .g = vk::ComponentSwizzle::eIdentity,
                    .b = vk::ComponentSwizzle::eIdentity,
                    .a = vk::ComponentSwizzle::eIdentity},
                .subresourceRange = vk::ImageSubresourceRange{.aspectMask = vk::ImageAspectFlagBits::eColor, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}};

            mSwapChainImageViews.emplace_back(mDevice, createInfo);
        }
#ifndef NDEBUG
        std::cout << "Created ImageView Count: " << mSwapChainImageViews.size() << '\n';
#endif
    };

    [[nodiscard]] vk::raii::ShaderModule createShaderModule(const std::vector<uint32_t> &code) const
    {
        const vk::ShaderModuleCreateInfo createInfo{
            .codeSize = code.size() * sizeof(uint32_t),
            .pCode = code.data()};

        return vk::raii::ShaderModule(mDevice, createInfo);
    }

    void createGraphicsPipeline()
    {
        const auto shaderPath = getExecutableDirectory() / "shaders" / "HelloTriangle.spv";
        const std::vector<uint32_t> shaderCode = readSpirv(shaderPath);
        vk::raii::ShaderModule shaderModule = createShaderModule(shaderCode);

        const vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
            .stage = vk::ShaderStageFlagBits::eVertex,
            .module = *shaderModule,
            .pName = "vertMain"};

        const vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
            .stage = vk::ShaderStageFlagBits::eFragment,
            .module = *shaderModule,
            .pName = "fragMain"};

        const std::array shaderStages{vertShaderStageInfo, fragShaderStageInfo};
#ifndef NDEBUG
        std::cout << "SPIR-V Byte Size: " << shaderCode.size() * sizeof(uint32_t) << " bytes\n";
        std::cout << "SPIR-V Word Count: " << shaderCode.size() << " words\n";
        std::cout << "Shader Module created successfully\n";
        std::cout << "Prepared Shader Stage Count:" << shaderStages.size() << '\n';
        std::cout << "Vertex Shader Entry Point: " << vertShaderStageInfo.pName << '\n';
        std::cout << "Fragment Shader Entry Point: " << fragShaderStageInfo.pName << '\n';
#endif
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