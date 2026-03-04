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
        // Evaluate visibility relative to parents
        bool isVisible = true;
        entt::entity currEntity = entity;
        while (registry.valid(currEntity))
        {
            if (registry.all_of<attributes::Togglable>(currEntity))
            {
                if (!registry.get<attributes::Togglable>(currEntity).visible)
                {
                    isVisible = false;
                    break;
                }
            }
            if (registry.all_of<attributes::Parent>(currEntity))
            {
                currEntity = registry.get<attributes::Parent>(currEntity).entity;
            }
            else
            {
                break;
            }
        }

        if (!isVisible) return; // Skip rendering if any ancestor is hidden

        // Compute compound model matrix
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        currEntity = entity;
        
        while (registry.valid(currEntity))
        {
            if (registry.all_of<attributes::Transform>(currEntity))
            {
                auto &t = registry.get<attributes::Transform>(currEntity);
                glm::mat4 localMatrix;
                if (t.useMatrix)
                {
                    localMatrix = t.modelMatrix;
                }
                else
                {
                    localMatrix = glm::mat4(1.0f);
                    localMatrix = glm::translate(localMatrix, t.pos);
                    localMatrix = glm::rotate(localMatrix, t.rot.x, glm::vec3(1, 0, 0));
                    localMatrix = glm::rotate(localMatrix, t.rot.y, glm::vec3(0, 1, 0));
                    localMatrix = glm::rotate(localMatrix, t.rot.z, glm::vec3(0, 0, 1));
                    localMatrix = glm::scale(localMatrix, t.scale);
                }
                
                // Pre-multiply parent transform so child is relative to parent
                modelMatrix = localMatrix * modelMatrix;
            }
            
            if (registry.all_of<attributes::Parent>(currEntity))
            {
                currEntity = registry.get<attributes::Parent>(currEntity).entity;
            }
            else
            {
                break;
            }
        }

        glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, &modelMatrix[0][0]);

        auto &renderMesh = view.get<attributes::RenderMesh>(entity);
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
