#include "RenderSystem.hpp"
#include "../Rasterizer.hpp"
#include "../attributes.hpp"
#include "../Shader/ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>

void RenderSystem::update(entt::registry &registry)
{
    if (!rasterizer || !rasterizer->program)
        return;

    GLuint mMatLoc = glGetUniformLocation(rasterizer->program->getProgramID(), "modelMatrix");
    GLuint uMatIdxLoc = glGetUniformLocation(rasterizer->program->getProgramID(), "u_MaterialIndex");

    auto view = registry.view<attributes::Transform, attributes::RenderMesh>();
    auto renderEntity = [&](entt::entity entity) {
        if (registry.all_of<attributes::Togglable>(entity))
        {
            auto &togglable = registry.get<attributes::Togglable>(entity);
            if (!togglable.visible)
                return;
        }

        auto &transform = view.get<attributes::Transform>(entity);
        auto &renderMesh = view.get<attributes::RenderMesh>(entity);

        glm::mat4 modelMatrix;
        if (transform.useMatrix)
        {
            modelMatrix = transform.modelMatrix;
        }
        else
        {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, transform.pos);
            modelMatrix = glm::rotate(modelMatrix, transform.rot.x, glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, transform.rot.y, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, transform.rot.z, glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, transform.scale);
        }

        glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, &modelMatrix[0][0]);

        for (int meshIdx : renderMesh.meshIndices)
        {
            const auto &mesh = rasterizer->scene.meshes[meshIdx];
            glUniform1i(uMatIdxLoc, mesh.materialIndex);
            glBindVertexArray(mesh.vao);
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        }
    };

    // Pass 1: Draw normal objects
    for (auto entity : view)
    {
        if (registry.all_of<attributes::RenderOnTop>(entity))
            continue;
        renderEntity(entity);
    }

    // Pass 2: Draw renderOnTop objects
    // Clear depth buffer so renderOnTop objects stay on top of Pass 1,
    // but keep depth test enabled so they intersect each other correctly
    glClear(GL_DEPTH_BUFFER_BIT);
    for (auto entity : view)
    {
        if (!registry.all_of<attributes::RenderOnTop>(entity))
            continue;
        renderEntity(entity);
    }
}
