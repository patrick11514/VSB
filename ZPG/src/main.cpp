#include "App.hpp"

int main(void)
{
    float points[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.75f, 1.0f, 0.0f,
        1.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f};

    printf("MAIN SIZE: %d\n", sizeof(points));

    App app;
    app.initialize();
    app.createShaders();
    app.createModels();
    app.run();
    app.destroy();

    return EXIT_SUCCESS;
}