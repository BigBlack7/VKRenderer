#include "Window.hpp"

namespace Core
{
    Window::Window(int width, int height, const std::string &title)
        : mWidth(width), mHeight(height), mTitle(title)
    {
        InitWindow();
    }

    Window::~Window()
    {
        if (mWindow)
        {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }
    }

    void Window::InitWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    }
}