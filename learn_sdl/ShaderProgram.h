//
// Created by Dantas on 10/06/2018.
//

#ifndef LEARNSDL2_SHADERPROGRAM_H
#define LEARNSDL2_SHADERPROGRAM_H

#include "glad/glad.h"

class ShaderProgram {
public:
    ShaderProgram(std::string vertexShaderFilePath, std::string fragmentShaderFilePath);

    void setUniformMatrix4fv(std::string name, const float *matrix);

    void setVec3(std::string name, float x , float y, float z);

    void setUniform1f(std::string name, float x);

    void use();

    GLuint getProgram() const;

private:

    GLuint program;
};


#endif //LEARNSDL2_SHADERPROGRAM_H
