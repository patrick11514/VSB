// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#pragma once

class App
{
    GLFWwindow *window;

private:
    void createWindow();
    void printVersionInfo();

    // callbacks
    static void error_callback(int error, const char *description);

public:
    void initialize();
    void createShaders();
    void createModels();
    void run();
};