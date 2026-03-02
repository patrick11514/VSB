#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace attributes
{
    struct Transform
    {
        glm::vec3 pos, rot, scale;
    };

    struct Mesh
    {
        // WIP - https://gemini.google.com/app/1368c6de5e3d1148
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        int indexCount;
        int materialIndex;
    };
};