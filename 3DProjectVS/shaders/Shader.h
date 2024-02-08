#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shaders_utils.h"
#include "GLM/gtc/type_ptr.hpp"

class Shader
{
public:
	unsigned int shaderProgram;

	Shader(const char* vertexAndFragmentPath)
	{
		auto shaderSource = ParseShader(vertexAndFragmentPath);

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertexShader2Source = shaderSource.vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexShader2Source, NULL);
        glCompileShader(vertexShader);

        const char* fragmentShaderSource = shaderSource.fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::VERTEX::SHADER::COMPILE_FAIL\n" << infoLog << std::endl;
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::FRAGMENT::SHADER::COMPILE_FAIL\n" << infoLog << std::endl;
        }

        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
	}

    void use()
    {
        glUseProgram(shaderProgram);
    }
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    void set4Float(const std::string& name, float v1, float v2, float v3, float v4)
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2, v3, v4);
    }

    void setVec3(const std::string& name, float v1, float v2, float v3) {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), v1, v2, v3);
    }

    void setVec3(const std::string& name, glm::vec3 vec) {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), vec.x, vec.y ,vec.z);
    }

    void set4Matrix(const std::string& name, const glm::mat4& trans)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
    }
};


#endif