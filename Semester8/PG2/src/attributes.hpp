#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace attributes
{
    struct Name
    {
        std::string name;
    };

    struct Transform
    {
        glm::vec3 pos, rot, scale;
        bool useMatrix = false;
        glm::mat4 modelMatrix = glm::mat4(1.0f);
    };

    struct RenderMesh
    {
        std::vector<int> meshIndices;
    };

    struct CameraSync
    {
        glm::mat4 relativeMatrix = glm::mat4(1.0f);
    };

    struct Togglable
    {
        bool visible = true;
    };

    struct RenderOnTop
    {
    };

    struct IsAxis
    {
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