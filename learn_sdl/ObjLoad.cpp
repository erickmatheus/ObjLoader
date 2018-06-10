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
    
    std::vector<float> normals;

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
            //std::cout << split.size() << std::endl;
            obj->addVertex(std::stof(split[1]), std::stof(split[2]), std::stof(split[3]));
        } else if (split.front() == "vn" && split.size() >= 3) {
            normals.push_back(std::stof(split[1]));
            normals.push_back(std::stof(split[2]));
            normals.push_back(std::stof(split[3]));
        } else if (split.front() == "f"){
                std::vector<int> vertex;
                std::vector<int> texture;
                std::vector<int> normal;
                /*
                 * f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
                 * f v1/vt1 v2/vt2 v3/vt3 ...
                 * f v1//vn1 v2//vn2 v3//vn3 ...
                 * f v1 v2 v3 ....
                 */
                for (int i = 1; i < split.size(); ++i) {
                    // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
                    // f v1/vt1 v2/vt2 v3/vt3 ...
                    // f v1//vn1 v2//vn2 v3//vn3 ...
                    if (split[i].find('/') != std::string::npos){
                        // f v1//vn1 v2//vn2 v3//vn3 ...
                        if (split[i].find("//") != std::string::npos) {
                            std::replace(split[i].begin(),split[i].end(), '/', ' ');

                            std::stringstream replaced(split[i]);

                            int vindex, nindex;

                            replaced >> vindex;
                            replaced >> nindex;

                            vertex.push_back(vindex);

                            auto n1 = normals[(nindex - 1) * 3];
                            auto n2 = normals[(nindex - 1) * 3 + 1];
                            auto n3 = normals[(nindex - 1) * 3 + 2];

                            std::cout << n1 << " " << n2 << " " << n3 << std::endl;
                            obj->addNormal(n1, n2, n3);

                        } else {
                            // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
                            // f v1/vt1 v2/vt2 v3/vt3 ...


                            std::replace(split[i].begin(),split[i].end(), '/', ' ');

                            std::stringstream replaced(split[i]);

                            int vindex, tindex;

                            replaced >> vindex;
                            replaced >> tindex;

                            vertex.push_back(vindex);
                            texture.push_back(tindex);

                            int nindex;
                            if (replaced >> nindex) {
                                auto n1 = normals[(nindex - 1) * 3];
                                auto n2 = normals[(nindex - 1) * 3 + 1];
                                auto n3 = normals[(nindex - 1) * 3 + 2];

                                std::cout << n1 << " " << n2 << " " << n3 << std::endl;
                                obj->addNormal(n1, n2, n3);
                            }
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