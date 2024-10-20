#include "Objects.hpp"
#include "Model.hpp"
#include "Models/bushes.h"
#include "Models/gift.h"
#include "Models/sphere.h"
#include "Models/tree.h"
#include "Object.hpp"
#include "ObjectData.hpp"

Object createBall(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations) {
  if (!dataStorage.hasObject("ball")) {
    Model model{sphere, sizeof(sphere) / sizeof(float)};
    dataStorage.add("ball", ObjectData(model, 2, []() {
                      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float), (GLvoid *)0);
                      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float),
                                            (GLvoid *)(3 * sizeof(float)));
                    }));
  }

  return Object(
      dataStorage.getObject("ball"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        // GLuint paramId = shaderProgram.getMatrixPosition();

        shaderProgram->putParameter("modelMatrix", matrix[0][0]);
        // glUniformMatrix4fv(paramId, 1, GL_FALSE, &matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 2880);
      });
}

Object createTree(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations) {
  if (!dataStorage.hasObject("tree")) {
    Model model{tree, sizeof(tree) / sizeof(float)};
    dataStorage.add("tree", ObjectData(model, 2, []() {
                      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float), (GLvoid *)0);
                      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float),
                                            (GLvoid *)(3 * sizeof(float)));
                    }));
  }

  return Object(
      dataStorage.getObject("tree"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 92814);
      });
}
Object createBush(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations) {
  if (!dataStorage.hasObject("bush")) {
    Model model{bushes, sizeof(bushes) / sizeof(float)};
    dataStorage.add("bush", ObjectData(model, 2, []() {
                      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float), (GLvoid *)0);
                      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float),
                                            (GLvoid *)(3 * sizeof(float)));
                    }));
  }

  return Object(
      dataStorage.getObject("bush"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 8730);
      });
}
Object createGift(const ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations) {
  if (!dataStorage.hasObject("gift")) {
    Model model{gift, sizeof(gift) / sizeof(float)};
    dataStorage.add("gift", ObjectData(model, 2, []() {
                      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float), (GLvoid *)0);
                      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                                            6 * sizeof(float),
                                            (GLvoid *)(3 * sizeof(float)));
                    }));
  }

  return Object(
      dataStorage.getObject("gift"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", matrix[0][0]);

        // GLuint paramId = shaderProgram.getMatrixPosition();

        // glUniformMatrix4fv(paramId, 1, GL_FALSE, &matrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 66624);
      });
}
