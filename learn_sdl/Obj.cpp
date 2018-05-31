//
// Created by Dantas on 25/05/2018.
//

#include <iostream>
#include <glad/glad.h>
#include "Obj.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Obj::Obj() {
    model = glm::mat4(1.0f);
}

const float * Obj::getVertices() const {
    return vertices.data();
}



float Obj::getNum_vertices() const {
    return vertices.size();
}

void Obj::addVertex(float x, float y, float z, float w) {
    addVertex(glm::vec3(x, y, z));
    std::cout << "add vertex: " << x << "  " << y << "  " << z << std::endl;
}

void Obj::addVertex(glm::vec3 vec3) {
    vertices.push_back(vec3.x);
    vertices.push_back(vec3.y);
    vertices.push_back(vec3.z);
}

std::vector<float> Obj::verticesVector() {
    return vertices;
}

void Obj::addEBO(int x, int y, int z) {
    ebo.emplace_back(x);
    ebo.emplace_back(y);
    ebo.emplace_back(z);
    /*std::cout << "add index: " << x << "  " << y << "  " << z << std::endl;
    std::cout << vertices.at((x)*3) << "  " << vertices.at((x)*3+1) << "  " << vertices.at((x)*3+2) << std::endl;
    std::cout << vertices.at((y)*3) << "  " << vertices.at((y)*3+1) << "  " << vertices.at((y)*3+2) << std::endl;
    std::cout << vertices.at((z)*3) << "  " << vertices.at((z)*3+1) << "  " << vertices.at((z)*3+2) << std::endl;*/
}

int *Obj::getEBO() {
    return ebo.data();
}

void Obj::addEBO(glm::vec3 vec3) {
    addEBO(static_cast<int>(vec3.x), static_cast<int>(vec3.y), static_cast<int>(vec3.z));
}

void Obj::draw(unsigned int program) {
    glUseProgram(program);
    glBindVertexArray(VAO);

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, ebo.size(), GL_UNSIGNED_INT, 0);
}

Obj::~Obj() {

}

void Obj::initialize() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(getNum_vertices() * sizeof(float)), getVertices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeEBO() * sizeof(float), getEBO(), GL_STATIC_DRAW);
}

void Obj::autoRotate() {
     model = glm::rotate(model, 0.00033f, glm::vec3(0.0f, -1.0f, 0.0f));
}