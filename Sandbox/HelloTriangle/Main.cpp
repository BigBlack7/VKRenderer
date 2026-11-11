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

constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;

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
        // 获取 GLFW 所需的 Vulkan 实例扩展及其数量
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        if (!glfwExtensions || glfwExtensionCount == 0)
        {
            throw std::runtime_error("Failed to get required GLFW extensions");
        }

        // 检查所需的扩展是否可用, 缺少任何扩展则抛出异常
        const auto availableExtensions = mContext.enumerateInstanceExtensionProperties();
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

        constexpr vk::ApplicationInfo appInfo{
            .pApplicationName = "Hello Triangle",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14};

        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtensions};

        mInstance = vk::raii::Instance(mContext, createInfo);
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