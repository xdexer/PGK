//
// Created by dominik on 17.11.2020.
//

#ifndef LISTA4_OBSTACLES_H
#define LISTA4_OBSTACLES_H
#include <AGL3Drawable.hpp>
#include <vector>
#include <glm/glm.hpp>

class Obstacles: public AGLDrawable {
    Obstacles(int n = 10, int s = 2137) : AGLDrawable(0){
        num = n;
        seed = s;
        setBuffers();
        setShaders();
    }

    void setBuffers();
    void setShaders();
    void draw();
private:
    //glm::vec2 *middlePoints;
    //std::vector<int> middlePoints;
    std::vector<float> angles;
    int num, seed;
};


#endif //LISTA4_OBSTACLES_H
