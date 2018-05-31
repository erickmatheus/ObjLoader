//
// Created by Dantas on 25/05/2018.
//

#ifndef LEARNSDL2_OBJLOAD_H
#define LEARNSDL2_OBJLOAD_H


#include <fstream>
#include <vector>
#include "Obj.h"

class ObjLoad {
private:
    std::ifstream ifstream;
    Obj *obj;

    void check(std::string str);
public:
    ObjLoad(std::string file);

    void load();
    Obj * getObject();

    void getNextVertices();

    glm::vec3 toVec3(std::string basic_string, char i);

    std::vector<int> * convertFace(std::string basic_string);

    void vertexNormal(std::vector<int> &vector, std::vector<int> &normal);
};


#endif //LEARNSDL2_OBJLOAD_H
