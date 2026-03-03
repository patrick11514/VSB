#pragma once
#include "ISystem.hpp"
#include "../attributes.hpp"
#include "../Camera.hpp"

class CameraSyncSystem : public ISystem
{
private:
    Camera *camera;

public:
    CameraSyncSystem(Camera *cam) : camera(cam) {}

    void update(entt::registry &registry) override
    {
        if (!camera)
            return;

        glm::vec3 camPos = camera->getPosition();
        // Camera target is already a normalized direction vector
        glm::vec3 camDir = glm::normalize(camera->getTarget());

        auto view = registry.view<attributes::Transform, attributes::CameraSync>();
        for (auto entity : view)
        {
            auto &transform = view.get<attributes::Transform>(entity);
            // auto &sync = view.get<attributes::CameraSync>(entity);

            transform.useMatrix = true;

            // X units in front of the camera
            float X = 6.0f; 
            glm::vec3 offsetPos = camPos + camDir * X;

            // Default position + moved X units in direction
            glm::vec3 finalPos = transform.pos + offsetPos;

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, finalPos);
            modelMatrix = glm::rotate(modelMatrix, transform.rot.x, glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, transform.rot.y, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, transform.rot.z, glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, transform.scale);

            transform.modelMatrix = modelMatrix;
        }
    }
};
