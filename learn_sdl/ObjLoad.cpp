//
// Created by Dantas on 25/05/2018.
//

#include <iostream>
#include <sstream>
#include <array>
#include <tuple>
#include <algorithm>
#include "ObjLoad.h"

ObjLoad::ObjLoad(std::string file) {
    ifstream.open(file);

    if (!ifstream.is_open()) {
        std::cout << "File not opened" << std::endl;
    }

    obj = nullptr;
}

void ObjLoad::load() {

    if (!ifstream.is_open()) {
        std::cout << "File not opened" << std::endl;
        return;
    }
    std::cout << "File opened" << std::endl;

    obj = new Obj;

    std::string line;
    while (std::getline(ifstream, line)){
        //ifstream >> line;
        //std::cout << "line: " << line << std::endl;
        std::vector<std::string> split;

        std::stringstream ss(line);

        std::string token;
        while (ss >> token) split.push_back(token);

        if (split.empty() || split.front() == "#") continue;

        if (split.front() == "v" && split.size() > 3){
            std::cout << split.size() << std::endl;
            obj->addVertex(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
        } else if (split.front() == "f"){
            std::vector<int> vertex;
            std::vector<int> texture;
            std::vector<int> normal;
            for (int i = 1; i < split.size(); ++i) {
                if (split[i].find('/') != std::string::npos){
                    if (split[i].find("//") != std::string::npos) {
                        std::replace(split[i].begin(),split[i].end(), '/', ' ');

                        std::stringstream replaced(split[i]);

                        int vindex, nindex;

                        replaced >> vindex;
                        replaced >> nindex;

                        vertex.push_back(vindex);
                        normal.push_back(nindex);

                        //vertexNormal(vertex, normal);
                    } else {
                        std::replace(split[i].begin(),split[i].end(), '/', ' ');

                        std::stringstream replaced(split[i]);

                        int vindex, tindex;

                        replaced >> vindex;
                        replaced >> tindex;

                        vertex.push_back(vindex);
                        texture.push_back(tindex);
                    }
                } else {
                    int x, y ,z;

                    std::stringstream stream(split[i]);

                    stream >> x;
                    stream >> y;
                    stream >> z;

                    vertex.push_back(x);
                    vertex.push_back(y);
                    vertex.push_back(z);
                }
            }

            obj->addEBO(vertex.at(0) - 1, vertex.at(1) - 1, vertex.at(2) - 1);
            if (vertex.size() - 1 != 2) {
                for (unsigned int j = 2; j < vertex.size(); ++j) {
                    obj->addEBO(vertex.at(0) - 1, vertex.at(j - 1) - 1, vertex.at(j) - 1);
                }
            }

        }

    }

    std::cout << "Obj loaded"<< std::endl;

}

Obj * ObjLoad::getObject() {
    return obj;
}