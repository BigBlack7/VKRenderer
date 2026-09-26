#pragma once

#include <vector>
#include <string>

namespace Core
{
    class Pipeline
    {
    public:
        Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:
        static std::vector<char> ReadFile(const std::string& filePath);

        void CreateGraphicsPipeline(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    };
}