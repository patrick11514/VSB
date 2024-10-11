// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <string>
#include <optional>

#include "Scenes/Scene.hpp"
#include "Shader/ShaderStorage.hpp"

#pragma once

class App
{
    GLFWwindow *window;

    ShaderStorage shaders;
    std::unordered_map<std::string, Scene> scenes;
    std::optional<std::string> currentScene = std::nullopt;

private:
    void createWindow();
    void printVersionInfo();

    // callbacks
    static void error_callback(int error, const char *description);

    void addScene(const std::string &name, Scene scene);
    const Scene &getScene(const std::string &name) const;

public:
    void initialize();
    void createShaders();
    void createModels();
    void run();
    void destroy(int status = EXIT_SUCCESS);
};