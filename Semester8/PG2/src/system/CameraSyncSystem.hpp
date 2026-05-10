#pragma once
#include "../Camera.hpp"
#include "../attributes.hpp"
#include "ISystem.hpp"
#include <glm/gtc/matrix_transform.hpp>

class CameraSyncSystem : public ISystem {
private:
  Camera *camera;

public:
  CameraSyncSystem(Camera *cam) : camera(cam) {}

  // Task 1: keep scene helpers aligned with the active camera for visibility.
  void update(entt::registry &registry) override {
    if (!camera)
      return;

    glm::vec3 camPos = camera->getPosition();
    glm::vec3 camDir = glm::normalize(camera->getTarget());

    auto view = registry.view<attributes::Transform>();
    view.each([&](entt::entity entity, attributes::Transform &transform) {
      if (!registry.all_of<attributes::CameraSync>(entity)) {
        return;
      }

      transform.useMatrix = true;

      float offsetDistance = 6.0f;
      glm::vec3 offsetPos = camPos + camDir * offsetDistance;

      glm::vec3 finalPos = transform.pos + offsetPos;

      glm::mat4 modelMatrix = glm::mat4(1.0f);
      modelMatrix = glm::translate(modelMatrix, finalPos);
      modelMatrix =
          glm::rotate(modelMatrix, transform.rot.x, glm::vec3(1, 0, 0));
      modelMatrix =
          glm::rotate(modelMatrix, transform.rot.y, glm::vec3(0, 1, 0));
      modelMatrix =
          glm::rotate(modelMatrix, transform.rot.z, glm::vec3(0, 0, 1));
      modelMatrix = glm::scale(modelMatrix, transform.scale);

      transform.modelMatrix = modelMatrix;
    });
  }
};
