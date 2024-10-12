#include "Objects.hpp"
#include "Model.hpp"
#include "Models/sphere.h"
#include "Models/tree.h"
#include "Models/bushes.h"
#include "Models/gift.h"
#include "Object.hpp"
#include "ObjectData.hpp"

Object createBall(const ShaderProgram &shaderProgram,
                  const Transformations &transformations) {
  Model model{sphere, sizeof(sphere) / sizeof(float)};
  ObjectData data(model, 2, []() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)(3 * sizeof(float)));
  });

  return Object(
      data, shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram &shaderProgram) {
        GLuint paramId = shaderProgram.getMatrixPosition();

        glUniformMatrix4fv(paramId, 1, GL_FALSE, &matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 2880);
      });
}

Object createTree(const ShaderProgram &shaderProgram,
                  const Transformations &transformations) {
  Model model{tree, sizeof(tree) / sizeof(float)};
  ObjectData data(model, 2, []() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)(3 * sizeof(float)));
  });

  return Object(
      data, shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram &shaderProgram) {
        GLuint paramId = shaderProgram.getMatrixPosition();

        glUniformMatrix4fv(paramId, 1, GL_FALSE, &matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 92814);
      });
}
Object createBush(const ShaderProgram &shaderProgram,
                  const Transformations &transformations) {
  Model model{bushes, sizeof(bushes) / sizeof(float)};
  ObjectData data(model, 2, []() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)(3 * sizeof(float)));
  });

  return Object(
      data, shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram &shaderProgram) {
        GLuint paramId = shaderProgram.getMatrixPosition();

        glUniformMatrix4fv(paramId, 1, GL_FALSE, &matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 8730);
      });
}
Object createGift(const ShaderProgram &shaderProgram,
                  const Transformations &transformations) {
  Model model{gift, sizeof(gift) / sizeof(float)};
  ObjectData data(model, 2, []() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (GLvoid *)(3 * sizeof(float)));
  });

  return Object(
      data, shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram &shaderProgram) {
        GLuint paramId = shaderProgram.getMatrixPosition();

        glUniformMatrix4fv(paramId, 1, GL_FALSE, &matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 66624);
      });
}
