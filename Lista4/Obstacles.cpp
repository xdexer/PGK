//
// Created by dominik on 17.11.2020.
//

#include "Obstacles.h"
#include <glm/glm.hpp>

void Obstacles::setBuffers() {
    bindBuffers();
    glm::vec3 middlePoints[num*num*num];
    int index = 0;
    float step = 2.0f / (2.0f*num);
    float vertices[] = {
        -1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         -1.0f, -1.0f, 0.0f,
    };
    float midx = -1.0f + step;
    float midy = -1.0f + step;
    float midz = -1.0f + step;
    //calculating center of box to draw triangles
    for(int i = 0; i < num; i++){//x
        for(int j = 0; j < num; j++)//y
        {
            for(int k = 0; k < num; k++)//z
            {
                glm::vec3 translation;
                translation.x = midx;
                translation.y = midy;
                translation.z = midz;
                middlePoints[index++] = translation;
                midx += 2*step;
            }
            midx = -1.0f + step;
            midy += 2 * step;
        }
        midx = -1.0f + step;
        midy = -1.0f + step;
        midz += 2 * step;
    }

}

void Obstacles::setShaders() {


}

void Obstacles::draw() {


}