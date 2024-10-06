#include "App.hpp"

int main(void)
{
    App app;
    app.initialize();
    app.createShaders();
    app.createModels();
    app.run();
    app.destroy();

    return EXIT_SUCCESS;
}