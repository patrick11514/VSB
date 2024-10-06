#include "App.hpp"
#include "Shader.hpp"

#include <stdexcept>
#include <iostream>

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
    try
    {
        Shader vertexShader("#version 330\n"
                            "layout(location=0) in vec3 vp;"
                            "out vec3 color;"
                            "void main () {"
                            "     gl_Position = vec4 (vp, 1.0);"
                            "     color = vp;"
                            "}",
                            ShaderType::Vertex);
        Shader fragmentShader("#version 330\n"
                              "in vec3 color;"
                              "out vec4 frag_colour;"
                              "void main () {"
                              "     frag_colour = vec4 (color, 1.0);"
                              "}",
                              ShaderType::Fragment);

        ShaderProgram shaderProgram(vertexShader, fragmentShader);
        this->addShaderProgram("ColorByCoords", shaderProgram);

        Shader vertexShader2("#version 330\n"
                             "layout(location=0) in vec3 vp;"
                             "void main () {"
                             "     gl_Position = vec4 (vp, 1.0);"
                             "}",
                             ShaderType::Vertex);

        Shader fragmentShader2("#version 330\n"
                               "out vec4 frag_colour;"
                               "void main () {"
                               "     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
                               "}",
                               ShaderType::Fragment);

        ShaderProgram shaderProgram2(vertexShader2, fragmentShader2);
        this->addShaderProgram("ColorPurple", shaderProgram2);
    }
    catch (const std::runtime_error &)
    {
        this->destroy(EXIT_FAILURE);
    }
}

void App::createModels()
{
    float points[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.75f, 1.0f, 0.0f,
        1.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f};

    Object object(points);

    this->addObject("BasicObjects", object);
}

void App::run()
{
    while (!glfwWindowShouldClose(this->window))
    {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->setShaderProgram("ColorByCoords");
        this->setObject("BasicObjects");
        // draw triangles
        glDrawArrays(GL_TRIANGLES, 4, 3); // mode,first,count

        this->setShaderProgram("ColorPurple");
        // glUseProgram(shaderProgram2);
        glDrawArrays(GL_QUADS, 0, 4); // mode,first,count
        //  update other events like input handling

        ShaderProgram::resetProgram();

        glfwPollEvents();
        // put the stuff we’ve been drawing onto the display
        glfwSwapBuffers(this->window);
    }
}

void App::destroy(int status)
{
    glfwDestroyWindow(this->window);
    glfwTerminate();

    exit(status);
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
    glfwGetFramebufferSize(this->window, &width, &height);
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

void App::addShaderProgram(const std::string &name, ShaderProgram &shaderProgram)
{
    this->shaderPrograms.emplace(name, shaderProgram);
}

void App::setShaderProgram(const std::string &name)
{
    auto it = this->shaderPrograms.find(name);

    if (it != this->shaderPrograms.end())
    {
        it->second.setProgram();
    }
}

void App::addObject(const std::string &name, Object &object)
{
    this->objects.emplace(name, object);
}

void App::setObject(const std::string &name)
{
    auto it = this->objects.find(name);

    if (it != this->objects.end())
    {
        it->second.setArray();
    }
}
