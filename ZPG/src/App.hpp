// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <string>

#include "ShaderProgram.hpp"
#include "Object.hpp"

#pragma once

class App
{
    GLFWwindow *window;

    std::unordered_map<std::string, ShaderProgram> shaderPrograms;
    std::unordered_map<std::string, Object> objects;

private:
    void createWindow();
    void printVersionInfo();

    void addShaderProgram(const std::string &name, ShaderProgram &shaderProgram);
    void setShaderProgram(const std::string &name);

    void addObject(const std::string &name, Object &object);
    void setObject(const std::string &name);

    // callbacks
    static void error_callback(int error, const char *description);

public:
    void initialize();
    void createShaders();
    void createModels();
    void run();
    void destroy(int status = EXIT_SUCCESS);
};