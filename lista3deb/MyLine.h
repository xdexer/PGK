//
// Created by dominik on 05.11.2020.
//

#ifndef LISTA3DEB_MYLINE_H
#define LISTA3DEB_MYLINE_H
#include <iostream>
#include <array>
#include <AGL3Drawable.hpp>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include <cmath>
class MyLine : public AGLDrawable {
public:
    MyLine() : AGLDrawable(0) {}
    void setShaders();
    void setBuffers();
    void rotation(int angle);
    void draw(float tx, float ty, int num);
    void setColor(float r, float g, float b);
   /* void settings(){
        setShaders();
        setBuffers();
    }
    */

private:
    GLfloat line_color[3] = { 0.0, 1.0, 0.0 };
    int angle;
    float x,y;
};


#endif //LISTA3DEB_MYLINE_H
