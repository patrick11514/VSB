#include "Objects.hpp"
#include "DrawableObject.hpp"
#include "Model.hpp"
#include "Models/bushes.h"
#include "Models/gift.h"
#include "Models/sphere.h"
#include "Models/tree.h"
#include "ObjectData.hpp"
#include <glm/gtc/type_ptr.hpp>

DrawableObject createBall(ShaderProgram *shaderProgram,
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

  return DrawableObject(
      dataStorage.getObject("ball"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", glm::value_ptr(matrix));

        glDrawArrays(GL_TRIANGLES, 0, 2880);
      });
}

DrawableObject createTree(ShaderProgram *shaderProgram,
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

  return DrawableObject(
      dataStorage.getObject("tree"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", glm::value_ptr(matrix));
        glDrawArrays(GL_TRIANGLES, 0, 92814);
      });
}
DrawableObject createBush(ShaderProgram *shaderProgram,
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

  return DrawableObject(
      dataStorage.getObject("bush"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", glm::value_ptr(matrix));
        glDrawArrays(GL_TRIANGLES, 0, 8730);
      });
}
DrawableObject createGift(ShaderProgram *shaderProgram,
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

  return DrawableObject(
      dataStorage.getObject("gift"), shaderProgram, transformations,
      [](const glm::mat4x4 &matrix, const ShaderProgram *shaderProgram) {
        shaderProgram->putParameter("modelMatrix", glm::value_ptr(matrix));

        glDrawArrays(GL_TRIANGLES, 0, 66624);
      });
}
