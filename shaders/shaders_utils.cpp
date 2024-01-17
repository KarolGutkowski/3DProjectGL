#include "shaders_utils.h"
#include <string>
#include <sstream>
#include <fstream>

#include <iostream>

ShaderSource ParseShader(const std::string filename)
{
    std::ifstream stream(filename);

    enum ShaderType
    {
        None = -1,
        Vertex = 0,
        Fragment = 1
    };

	ShaderType type = ShaderType::None;

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::Vertex;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::Fragment;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(),  ss[1].str() };
}