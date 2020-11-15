//
// Created by dominik on 05.11.2020.
//

#include "MyLine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
void MyLine::setShaders() {
    compileShaders(R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 2) uniform mat4 model;
         layout(location = 4) uniform mat4 view;
         layout(location = 3) uniform mat4 projection;
         layout(location = 1) uniform vec2  center;
         layout(location = 5) uniform vec3  cross_color;
         out vec4 vtex;
         out vec4 vcolor;

         void main(void) {
            vec2 p = (pos * scale + center);
            //gl_Position = vec4(p, 0.0, 1.0);
	        gl_Position = view * vec4(p, 0.0f ,1.0f);
         if(gl_VertexID == 0){
                vcolor = vec4(cross_color, 1.0);
            }
         else{
                vcolor = vec4(0.0, 0.0, 1.0, 1.0);
            }
         }

      )END", R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         in vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;
         }

      )END");
}

void MyLine::setBuffers() {
    bindBuffers();
    GLfloat vert[2][2]; // lines
    //printf("%f %f \n", this->x, this->y);
    vert[0][0] = -(this->x);
    vert[0][1] = -(this->y);
    vert[1][0] = this->x;
    vert[1][1] = this->y;
    //printf("%f %f %f %f \n", vert[0][0], vert[0][1], vert[1][0], vert[1][1]);
    glBufferData(GL_ARRAY_BUFFER, 2*2*sizeof(float), vert, GL_STATIC_DRAW );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                 // attribute 0, must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,//24,             // stride
            (void*)0            // array buffer offset
    );
}

void MyLine::rotation(int angle){
    this->angle = angle;
    this->x = 1.0f * cos(this->angle * (M_PI/180)) - 0.0f * sin(this->angle * (M_PI/180));
    this->y = 1.0f * sin(this->angle * (M_PI/180)) + 0.0f * cos(this->angle * (M_PI/180));
    //setShaders();
    setBuffers();
}

void MyLine::setScale() {
    //std::cout << "setScale" << scale;
    if(scale < 0.0f){
        scaleStep = 0.05f;
    }
    else if(scale > 1.0f){
        scaleStep = -0.05f;
    }
    scale += scaleStep;
}

void MyLine::draw(float tx, float ty){
    bindProgram();
    bindBuffers();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tx,ty,1.0f));
    float ang = angle * 1.0f;
    model = glm::rotate(model, glm::radians(ang), glm::vec3(1.0f,1.0f,0.0f));
    glUniformMatrix4fv(2, 1, GL_FALSE, &model[0][0]);
    //printf("%f %f \n", this->x, this->y);
    glUniform1f(0, scale);  // scale  in vertex shader (scaling size)
    glUniform2f(1, tx, ty);  // center in vertex shader
    glUniform3f(5, line_color[0],line_color[1],line_color[2]);

    glDrawArrays(GL_LINES, 0, 2);
}

void MyLine::setColor(float r, float g, float b){
    line_color[0]=r;line_color[1]=g;line_color[2]=b;
}