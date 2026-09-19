#include "Application.hpp"

namespace Core
{
    void Application::Run()
    {
        while (!mWindow.ShouldClose())
        {
            glfwPollEvents();
        }
    }
}