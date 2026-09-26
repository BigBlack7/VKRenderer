#include "Pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace Core
{
    Pipeline::Pipeline(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
    {
        try
        {
            CreateGraphicsPipeline(vertexShaderPath, fragmentShaderPath);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    std::vector<char> Pipeline::ReadFile(const std::string &filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    void Pipeline::CreateGraphicsPipeline(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
    {
        // Read shader files
        std::vector<char> vertexShaderCode = ReadFile(vertexShaderPath);
        std::vector<char> fragmentShaderCode = ReadFile(fragmentShaderPath);

        std::cout << "Vertex Shader Code Size: " << vertexShaderCode.size() << " bytes" << std::endl;
        std::cout << "Fragment Shader Code Size: " << fragmentShaderCode.size() << " bytes" << std::endl;
    }
}