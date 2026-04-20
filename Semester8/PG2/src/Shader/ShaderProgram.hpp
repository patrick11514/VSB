#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

class Controller;

class ShaderProgram
{
private:
    GLuint programId;

    std::string readFile(const std::string &filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filePath);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    GLuint compileShader(GLenum type, const std::string &source)
    {
        GLuint shader = glCreateShader(type);
        const char *srcChars = source.c_str();
        glShaderSource(shader, 1, &srcChars, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
        }
        return shader;
    }

public:
    ShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, Controller *controller = nullptr)
    {
        std::string vertexSource = readFile(vertexShaderPath);
        std::string fragmentSource = readFile(fragmentShaderPath);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);

        int success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(programId, 512, nullptr, infoLog);
            throw std::runtime_error("Shader linking failed: " + std::string(infoLog));
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ShaderProgram(const std::string &vertexShaderPath, const std::string &geometryShaderPath,
                  const std::string &fragmentShaderPath, Controller *controller = nullptr)
    {
        std::string vertexSource = readFile(vertexShaderPath);
        std::string geometrySource = readFile(geometryShaderPath);
        std::string fragmentSource = readFile(fragmentShaderPath);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        programId = glCreateProgram();
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, geometryShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);

        int success;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(programId, 512, nullptr, infoLog);
            throw std::runtime_error("Shader linking failed: " + std::string(infoLog));
        }

        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);
    }

    void activate() const
    {
        glUseProgram(programId);
    }

    GLuint getProgramID() const
    {
        return programId;
    }

    static void resetProgram()
    {
        glUseProgram(0);
    }
};
