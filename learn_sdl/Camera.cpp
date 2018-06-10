//
// Created by Dantas on 01/06/2018.
//

#include <iostream>
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::mat4 projection) {
    this->projection = projection;

    position = glm::vec3(10.0f, 0.0f, 0.0f);
    direction = glm::vec3(0.0, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f , 0.0f);



    view = glm::lookAt(position, direction, up);/*glm::mat4(
            2.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 3.0f, 1.0f);*///glm::lookAt(position, direction, up);

    combined = projection.operator*=(view);
}

void Camera::lookAt(glm::vec3 lookAtPos) {
    direction = lookAtPos;
}

void Camera::update() {
    view = glm::lookAt(position, direction, up);

    combined = projection * view;
}

void Camera::translate(float x, float y, float z) {
    position = glm::vec3(position.x + x, position.y + y, position.z + z);
    std::cout << x << " " << y << " " << z << std::endl;
    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

void Camera::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
    view = glm::translate(view, glm::vec3(x, y, z));
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

void Camera::rotate(float angle, float x, float y, float z) {
    view = glm::rotate(view, angle, glm::vec3(x, y, z));
}

const glm::vec3 &Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getRight() {
    return glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));;
}

const glm::vec3 &Camera::getDirection() const {
    return direction;
}
