//
// Created by Dantas on 25/05/2018.
//

#ifndef LEARNSDL2_OBJ_H
#define LEARNSDL2_OBJ_H


#include <vector>
#include "glm/glm.hpp"

class Obj {
private:
    std::vector<int> ebo;

    std::vector<float> vertices;
    std::vector<glm::vec3> uvs;
    std::vector<glm::vec3> normals;

    glm::mat4 model;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
public:
    virtual ~Obj();

    Obj();

    void initialize();

    const float * getVertices() const;

    std::vector<float> verticesVector();

    float getNum_vertices() const;

    void addVertex(float x, float y, float z, float w = 1.0);

    void addVertex(float x, float y);

    void addEBO(glm::vec3 vec3);

    int * getEBO();

    int sizeEBO() { return ebo.size(); }

    void addVertex(glm::vec3 vec3);

    void addEBO(int x, int y, int z);

    void autoRotate();

    void draw(unsigned int program);

    void draw(unsigned int program, float x, float y, float z);
};


#endif //LEARNSDL2_OBJ_H
