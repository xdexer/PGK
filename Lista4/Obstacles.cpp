//
// Created by dominik on 17.11.2020.
//

#include "Obstacles.h"
#include <glm/glm.hpp>

void Obstacles::setBuffers() {
    //bindBuffers();
    glm::vec3 middlePoints[num*num*num]; //global? and vector
    int index = 0;
    float step = 2.0f / (2.0f*num);

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
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*(num*num*num), &middlePoints[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    float vertices[] = {
            //positions        //colors
            -1.0f/num, 1.0f/num, 0.0f/num, 1.0f, 0.0f, 0.0f,
            1.0f/num, -1.0f/num, 0.0f/num, 0.0f, 1.0f, 0.0f,
            -1.0f/num, -1.0f/num, 0.0f/num, 0.0f, 0.0f, 1.0f,
    };
    //unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1 , 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);
}

void Obstacles::setShaders() {
    compileShaders(R"END(
        #version 330
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require
        layout(location = 0) in vec3 pos;
        layout(location = 1) in vec3 aColor;
        layout(location = 2) in vec3 aOffset;
        layout(location = 2) uniform mat4 model;
        layout(location = 4) uniform mat4 view;
        layout(location = 3) uniform mat4 projection;
        out vec3 vcolor;

        void main(void){
            gl_Position = projection * view * vec4(pos + aOffset, 1.0f);
            vcolor = aColor;
            }
)END", R"END(
        #version 330
        in vec3 vcolor;
        void main(void){
            gl_FragColor = vec4(vcolor, 1.0);
            }
)END");

}

void Obstacles::draw() {
    bindProgram();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArraysInstanced(GL_TRIANGLES,0,3,num*num*num);
    glBindVertexArray(0);
}