#include "RenderSystem.hpp"
#include "../Rasterizer.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../attributes.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

// Task 2-3: render loaded meshes with hierarchy-aware transforms and materials.
void RenderSystem::update(entt::registry &registry)
{
  if (!rasterizer)
    return;

  // 1. Get the ACTUALLY ACTIVE shader program (PBR, Shadow, or Depth)
  GLint currentProgram = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
  if (currentProgram == 0)
    return;

  // 2. Query uniform locations from the CURRENT program dynamically
  GLint mMatLoc = glGetUniformLocation(currentProgram, "modelMatrix");
  GLint uMatIdxLoc = glGetUniformLocation(currentProgram, "u_MaterialIndex");

  // 3. Detect if we are rendering the Stencil Shadow Volume pass
  // An easy hack: only the shadow shader has the "extrusionDistance" uniform!
  bool isShadowPass = (glGetUniformLocation(currentProgram, "extrusionDistance") != -1);

  auto view = registry.view<attributes::Transform, attributes::RenderMesh>();
  auto renderEntity = [&](entt::entity entity)
  {
    bool isVisible = true;
    entt::entity currEntity = entity;

    // Check Visibility
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

    if (!isVisible)
      return;

    // Calculate Hierarchy Transforms
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

    // Send the matrix to the shader (if the shader actually uses it)
    if (mMatLoc != -1)
    {
      glUniformMatrix4fv(mMatLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    }

    auto &renderMesh = view.get<attributes::RenderMesh>(entity);
    for (int meshIdx : renderMesh.meshIndices)
    {
      const auto &mesh = rasterizer->scene.meshes[meshIdx];

      if (uMatIdxLoc != -1)
      {
        glUniform1i(uMatIdxLoc, mesh.materialIndex);
      }

      glBindVertexArray(mesh.vao);

      // --- THE MAGIC SWITCH ---
      if (isShadowPass && mesh.adjacencyIndexCount > 0)
      {
        // Shadow pass: Bind the 6-index buffer and draw adjacency
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.adjacencyEbo);
        glDrawElements(GL_TRIANGLES_ADJACENCY, mesh.adjacencyIndexCount, GL_UNSIGNED_INT, 0);
      }
      else
      {
        // Normal pass (PBR/Depth): Bind standard 3-index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
      }

      // Clean up GL state to avoid leaking VAO/EBO bindings into other code
      glBindVertexArray(0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  };

  for (auto entity : view)
  {
    renderEntity(entity);
  }
}