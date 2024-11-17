#include "Objects.hpp"
#include "DrawableObject.hpp"
#include "Model.hpp"
#include "Models/bushes.h"
#include "Models/gift.h"
#include "Models/sphere.h"
#include "Models/tree.h"
#include "ObjectData.hpp"
#include <glm/gtc/type_ptr.hpp>

DrawableObject *createBall(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material) {
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

  return new DrawableObject(
      dataStorage.getObject("ball"), shaderProgram, transformations,
      []() { glDrawArrays(GL_TRIANGLES, 0, 2880); }, material);
}

DrawableObject *createTree(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material) {
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

  return new DrawableObject(
      dataStorage.getObject("tree"), shaderProgram, transformations,
      []() { glDrawArrays(GL_TRIANGLES, 0, 92814); }, material);
}
DrawableObject *createBush(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material) {
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

  return new DrawableObject(
      dataStorage.getObject("bush"), shaderProgram, transformations,
      []() { glDrawArrays(GL_TRIANGLES, 0, 8730); }, material);
}
DrawableObject *createGift(ShaderProgram *shaderProgram,
                           std::shared_ptr<Transformation> transformations,
                           std::shared_ptr<Material> material) {
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

  return new DrawableObject(
      dataStorage.getObject("gift"), shaderProgram, transformations,
      []() { glDrawArrays(GL_TRIANGLES, 0, 66624); }, material);
}
