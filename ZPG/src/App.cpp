#include "App.hpp"

void App::error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

void App::initialize()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    this->createWindow();
}

void App::createShaders()
{
}

void App::createWindow()
{
    this->window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!this->window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    this->printVersionInfo();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
}

void App::printVersionInfo()
{
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}
