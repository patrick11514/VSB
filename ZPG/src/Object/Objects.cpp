#include "Objects.hpp"
#include "DrawableObject.hpp"
#include "Model.hpp"
#include "Models/bushes.h"
#include "Models/gift.h"
#include "Models/plane.h"
#include "Models/skycube.h"
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
                      return 6;
                    }));
  }

  return new DrawableObject(dataStorage.getObject("ball"), shaderProgram,
                            transformations, material);
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

                      return 6;
                    }));
  }

  return new DrawableObject(dataStorage.getObject("tree"), shaderProgram,
                            transformations, material);
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

                      return 6;
                    }));
  }

  return new DrawableObject(dataStorage.getObject("bush"), shaderProgram,
                            transformations, material);
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

                      return 6;
                    }));
  }

  return new DrawableObject(dataStorage.getObject("gift"), shaderProgram,
                            transformations, material);
}

DrawableObject *createPlane(ShaderProgram *shaderProgram,
                            std::shared_ptr<Transformation> transformations,
                            std::shared_ptr<Material> material) {
  if (!dataStorage.hasObject("plane")) {
    Model model{plane, sizeof(plane) / sizeof(float)};
    dataStorage.add(
        "plane", ObjectData(model, 3, []() {
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                (GLvoid *)0);
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                (GLvoid *)(3 * sizeof(float)));
          glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                (GLvoid *)(6 * sizeof(float)));

          return 8;
        }));
  }

  return new DrawableObject(dataStorage.getObject("plane"), shaderProgram,
                            transformations, material,
                            []() { glDrawArrays(GL_QUADS, 0, 4); });
}

DrawableObject *
createPlaneNormal(ShaderProgram *shaderProgram,
                  std::shared_ptr<Transformation> transformations,
                  std::shared_ptr<Material> material) {
  printf("HERE\n");
  if (!dataStorage.hasObject("normal_plane")) {
    Model model{planeNormal, sizeof(planeNormal) / sizeof(float)};
    dataStorage.add(
        "normal_plane", ObjectData(model, 3, []() {
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                (GLvoid *)0);
          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                (GLvoid *)(3 * sizeof(float)));
          glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                (GLvoid *)(6 * sizeof(float)));

          return 8;
        }));
  }

  printf("HERE\n");
  return new DrawableObject(dataStorage.getObject("normal_plane"),
                            shaderProgram, transformations, material,
                            []() { glDrawArrays(GL_QUADS, 0, 4); });
}
