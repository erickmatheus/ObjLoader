//
// Created by Dantas on 01/06/2018.
//

#ifndef LEARNSDL2_CAMERA_H
#define LEARNSDL2_CAMERA_H


#include "glm/vec3.hpp"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_mat4x4.hpp"

class Camera {
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 combined;

public:

    Camera(glm::mat4 projection);

    void lookAt(glm::vec3 lookAtPos);

    void update();

    void translate(float x, float y, float z);

    void setPosition(float x, float y, float z);

    const glm::mat4 &getCombined() const;

    const glm::mat4 &getView() const;

    const glm::mat4 &getProjection() const;

    void rotate(float angle, float x, float y, float z);
};


#endif //LEARNSDL2_CAMERA_H
