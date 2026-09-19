#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Core
{
    class Window
    {
    public:
        Window(int width, int height, const std::string &title);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        bool ShouldClose() const { return glfwWindowShouldClose(mWindow); };

    private:
        void InitWindow();

    private:
        const int mWidth{0}, mHeight{0};
        const std::string mTitle{nullptr};
        GLFWwindow *mWindow{nullptr};
    };
}