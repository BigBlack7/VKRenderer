#include <iostream>
#include <cstdint>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan_raii.hpp>

int main()
{
    int major = 0;
    int minor = 0;
    int revision = 0;

    glfwGetVersion(&major, &minor, &revision);

    std::cout << "GLFW: "
              << major << "." << minor << "." << revision
              << '\n';

    const glm::vec3 test{1.0f, 2.0f, 3.0f};

    std::cout << "GLM test: "
              << test.x + test.y + test.z << '\n';

    std::uint32_t version = VK_API_VERSION_1_0;

    if (vkEnumerateInstanceVersion(&version) != VK_SUCCESS)
    {
        std::cerr << "Vulkan Loader check failed\n";
        return 1;
    }

    std::cout << "Vulkan Loader API: "
              << VK_API_VERSION_MAJOR(version) << "."
              << VK_API_VERSION_MINOR(version) << "."
              << VK_API_VERSION_PATCH(version) << '\n';

    return 0;
}