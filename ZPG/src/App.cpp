#include "App.hpp"
#include "Shader/Shader.hpp"
#include "Object/ObjectData.hpp"
#include "Object/Objects.hpp"

#include <stdexcept>

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
        Shader vertexShader("../shaders/vertex/ColorByCoords.vert",
                            ShaderType::Vertex);
        Shader fragmentShader("../shaders/fragment/ColorByCoords.frag",
                              ShaderType::Fragment);

        ShaderProgram shaderProgram(vertexShader, fragmentShader);
        this->shaders.addShaderProgram("ColorByCoords", shaderProgram);

        Shader vertexShader2("../shaders/vertex/Purple.vert",
                             ShaderType::Vertex);

        Shader fragmentShader2("../shaders/fragment/Purple.frag",
                               ShaderType::Fragment);

        ShaderProgram shaderProgram2(vertexShader2, fragmentShader2);
        this->shaders.addShaderProgram("ColorPurple", shaderProgram2);
    }
    catch (const std::runtime_error &)
    {
        this->destroy(EXIT_FAILURE);
    }
}

void App::createModels()
{
    Scene scene;

    Model model({0.5f, 0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 -0.5f, -0.5f, 0.0f,
                 -0.5f, 0.5f, 0.0f,
                 0.75f, 1.0f, 0.0f,
                 1.0f, 0.5f, 0.0f,
                 0.5f, 0.5f, 0.0f,
                 0.0f, 0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 -0.5f, -0.5f, 0.0f});
    ObjectData data(model);

    scene.addObject(Object(
        data,
        this->shaders.getShaderProgram("ColorByCoords"),
        Transformations(),
        []()
        {
            glDrawArrays(GL_TRIANGLES, 4, 3);
        }));

    scene.addObject(Object(
        data,
        this->shaders.getShaderProgram("ColorPurple"),
        Transformations(),
        []()
        {
            glDrawArrays(GL_QUADS, 0, 4);
        }));

    this->addScene("objects", scene);

    Scene ballScene;

    ballScene.addObject(createBall(this->shaders.getShaderProgram("ColorByCoords"), Transformations()));

    this->addScene("ball", ballScene);

    this->currentScene = "ball";
}

void App::run()
{
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->window))
    {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (this->currentScene.has_value())
        {
            this->getScene(this->currentScene.value()).render();
        }

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

void App::addScene(const std::string &name, Scene scene)
{
    this->scenes.emplace(name, scene);
}

const Scene &App::getScene(const std::string &name) const
{
    auto it = this->scenes.find(name);

    if (it == this->scenes.end())
    {
        throw std::runtime_error("Cannot find scene");
    }

    return it->second;
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