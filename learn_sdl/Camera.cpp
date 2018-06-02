//
// Created by Dantas on 01/06/2018.
//

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::mat4 projection) {
    this->projection = projection;

    position = glm::vec3(10.0f, 0.0f, 0.0f);
    direction = glm::vec3(0.0, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f , 0.0f);

    view = glm::lookAt(position, direction, up);

    combined = projection.operator*=(view);
}

void Camera::lookAt(glm::vec3 lookAtPos) {
    direction = lookAtPos;
}

void Camera::update() {
    view = glm::lookAt(position, direction, up);

    combined = projection;
}

void Camera::translate(float x, float y, float z) {
    position += glm::vec3(x, y, z);
}

void Camera::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

const glm::mat4 &Camera::getView() const {
    return view;
}

const glm::mat4 &Camera::getCombined() const {
    return combined;
}

const glm::mat4 &Camera::getProjection() const {
    return projection;
}
