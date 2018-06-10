//
// Created by Dantas on 10/06/2018.
//

#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <iostream>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string vertexShaderFilePath, std::string fragmentShaderFilePath) {
    std::string vCode, fCode;

    std::ifstream vertexFile(vertexShaderFilePath);

    if (vertexFile.is_open()){
        std::stringstream vStream;
        vStream << vertexFile.rdbuf();
        vCode = vStream.str();
        vertexFile.close();
    }

    std::ifstream fragmentFile(fragmentShaderFilePath);
    if (fragmentFile.is_open()){
        std::stringstream fStream;
        fStream << fragmentFile.rdbuf();
        fCode = fStream.str();
        fragmentFile.close();
    }

    vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);


    std::cout << vCode.size() << " " << fCode.size() << std::endl;

    auto vs = glCreateShader(GL_VERTEX_SHADER);
    const char* vc = vCode.c_str();
    glShaderSource(vs,  1, &vc, NULL);
    glCompileShader(vs);
    // Check if its compilation was sucessfull
    int  vertexCompilationSuccess;
    char vertexInfoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &vertexCompilationSuccess);
    if(!vertexCompilationSuccess)
    {
        glGetShaderInfoLog(vs, 512, NULL, vertexInfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertexInfoLog << std::endl;
    }


    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fc = fCode.c_str();
    glShaderSource(fs,  1, &fc, NULL);
    glCompileShader(fs);
    // Check if its compilation was sucessfull
    int  fragmentCompilationSuccess;
    char fragmentInfoLog[512];
    glGetShaderiv(fs, GL_COMPILE_STATUS, &fragmentCompilationSuccess);
    if(!fragmentCompilationSuccess)
    {
        glGetShaderInfoLog(fs, 512, NULL, fragmentInfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragmentInfoLog << std::endl;
    }

    program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Check if its creation was successfull
    int programSuccess ;
    char shaderProgramInfoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
    if(!programSuccess) {
        glGetProgramInfoLog(program, 512, NULL, shaderProgramInfoLog);
    }
}

void ShaderProgram::use(){
    glUseProgram(program);
}

void ShaderProgram::setUniformMatrix4fv(std::string name, const float *matrix) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix);
}

void ShaderProgram::setVec3(std::string name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

GLuint ShaderProgram::getProgram() const {
    return program;
}

void ShaderProgram::setUniform1f(std::string name, float x) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), x);
}

