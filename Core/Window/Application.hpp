#pragma once

#include "Window.hpp"
#include "Graphics/Pipeline.hpp"

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
        Pipeline mPipeline{"../../../Assets/Shader/.Compile/Triangle.vert.spv", "../../../Assets/Shader/.Compile/Triangle.frag.spv"};
    };
}