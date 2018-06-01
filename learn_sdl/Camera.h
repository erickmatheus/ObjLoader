//
// Created by Dantas on 01/06/2018.
//

#ifndef LEARNSDL2_CAMERA_H
#define LEARNSDL2_CAMERA_H


#include "glm/vec3.hpp"

class Camera {
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
};


#endif //LEARNSDL2_CAMERA_H
