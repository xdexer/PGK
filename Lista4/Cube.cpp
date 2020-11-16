//
// Created by dominik on 16.11.2020.
//

#include "Cube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Cube::setBuffers() {
    bindBuffers();
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1,&VBO);

    //glBindVertexArray(VAO);
    float vertices[] = {
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,

            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,

            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
    };

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Cube::setShaders() {
    compileShaders(R"END(
        #version 330
        #extension GL_ARB_explicit_uniform_location : require
        #extension GL_ARB_shading_language_420pack : require
        layout(location = 0) in vec3 pos;
        layout(location = 2) uniform mat4 model;
        layout(location = 4) uniform mat4 view;
        layout(location = 3) uniform mat4 projection;
        out vec4 vcolor;

        void main(void){
            gl_Position = projection * view * model * vec4(pos, 1.0f);
            vcolor = vec4(0.0,1.0,0.0,0.5);
            }
)END", R"END(
        #version 330
        in vec4 vcolor;
        void main(void){
            gl_FragColor = vcolor;
            }
)END");
}

void Cube::draw() {
    bindProgram();
    bindBuffers();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
    //model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f,1.0f,0.0f)); //rotation works
    glUniformMatrix4fv(2, 1, GL_FALSE, &model[0][0]);
    glDrawArrays(GL_TRIANGLES,0,36);
}