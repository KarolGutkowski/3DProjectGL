#pragma once
#include <string>

struct ShaderSource
{
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
};

ShaderSource ParseShader(const std::string filename);