#include "Controller.hpp"
#include "Camera.hpp"
#include "Rasterizer.hpp"
#include <cstdio>

Controller *Controller::instance = nullptr;

Controller::Controller(Rasterizer *rasterizer, Camera* camera) : rasterizer(rasterizer), camera(camera) {}

Controller *Controller::getInstance(Rasterizer *rasterizer, Camera* camera) {
  if (Controller::instance == nullptr) {
    Controller::instance = new Controller(rasterizer, camera);
  }
  return Controller::instance;
}

Controller::~Controller() {
    // cleanup
}

void Controller::onKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void Controller::onMouse(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastMousePos.x = xpos;
        lastMousePos.y = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastMousePos.x;
    float yoffset = lastMousePos.y - ypos; 
    lastMousePos.x = xpos;
    lastMousePos.y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (camera) {
        camera->changeYaw(xoffset);
        camera->changePitch(yoffset);
    }
}

void Controller::onFrame() {
    if (!camera) return;

    if (keys[GLFW_KEY_W])
        camera->forward(cameraSpeed);
    if (keys[GLFW_KEY_S])
        camera->backward(cameraSpeed);
    if (keys[GLFW_KEY_A])
        camera->toLeft(cameraSpeed);
    if (keys[GLFW_KEY_D])
        camera->toRight(cameraSpeed);
    if (keys[GLFW_KEY_SPACE])
        camera->upMovement(cameraSpeed);
    if (keys[GLFW_KEY_LEFT_SHIFT])
        camera->downMovement(cameraSpeed);
}
