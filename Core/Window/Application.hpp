#pragma once

#include "Window.hpp"

namespace Core
{
    class Application
    {
    public:
        static constexpr int WIDTH = 1200;
        static constexpr int HEIGHT = 800;

        void Run();

    private:
        Window mWindow{WIDTH, HEIGHT, "VulkanStudy"};
    };
}