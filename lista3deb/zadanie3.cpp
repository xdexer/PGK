// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <random>
#include <vector>
#include <sstream>
#include <memory>
#include <array>
#include <AGL3Window.hpp>
#include <Linetab.h>
#include <MyLine.h>

//#include "AGL3Window.hpp"
//#include "AGL3Drawable.hpp"
// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================

class BackgroundRectangle : public AGLDrawable
{
public:
    BackgroundRectangle(GLfloat xStart = -1, GLfloat yStart = 1, GLfloat xEnd = 1, GLfloat yEnd = -1);
    void setShaders();
    void setBuffers();
    void draw();
private:
    std::array<std::array<GLfloat,2>, 6> vert;
};

BackgroundRectangle::BackgroundRectangle(GLfloat xStart, GLfloat yStart, GLfloat xEnd, GLfloat yEnd)
{

    vert[0][0] = xStart;
    vert[0][1] = yStart;
    vert[1][0] = xEnd;
    vert[1][1] = yStart;
    vert[2][0] = xStart;
    vert[2][1] = yEnd;
    vert[3][0] = xEnd;
    vert[3][1] = yEnd;
    vert[4][0] = xStart;
    vert[4][1] = yEnd;
    vert[5][0] = xEnd;
    vert[5][1] = yStart;


    setShaders();
    setBuffers();
}
void BackgroundRectangle::setShaders()
{
    compileShaders(R"END(
                   #version 330
                   #extension GL_ARB_explicit_uniform_location : require
                   #extension GL_ARB_shading_language_420pack : require
                   layout(location = 0) in vec2 pos;
                   layout(location = 0) uniform float scale;
                   layout(location = 1) uniform vec2  center;
                   layout(location = 2) uniform vec3  first_color;
                   layout(location = 3) uniform vec3  second_color;
                   out vec2 pvec;
                   flat out vec3 color1;
                   flat out vec3 color2;
                   void main(void) {
                       vec2 p = (pos * scale + center);
                       gl_Position = vec4(p, 0.0, 1.0);
                       color1 = first_color;
                       color2 = second_color;
                       pvec = pos;
                   }
                   )END", R"END(
                          #version 330
                          flat in vec3 color1;
                          flat in vec3  color2;
                          in vec2 pvec;
                          void main(void) {
                                //pvec.x > -1 && pvec.x < -0.8 || pvec.x > -0.6 && pvec.x < -0.4 || pvec.x > -0.2 && pvec.x < 0 || pvec.x > 0.2 && pvec.x < 0.4 || pvec.x > 0.6 && pvec.x < 0.8)
                                if(pvec.x > -1 && pvec.x < -0.8 || pvec.x > -0.6 && pvec.x < -0.4 || pvec.x > -0.2 && pvec.x < 0 || pvec.x > 0.2 && pvec.x < 0.4 || pvec.x > 0.6 && pvec.x < 0.8)
                                {
                                    gl_FragColor = vec4(color1, 1);
                                }
                                else
                                {
                                    gl_FragColor = vec4(color2, 1);
                                }
                          }
                          )END");
}



void BackgroundRectangle::setBuffers()
{
    bindBuffers();
    GLfloat temp[6][2] = {{vert[0][0], vert[0][1]}, {vert[1][0], vert[1][1]}, {vert[2][0], vert[2][1]}, {vert[3][0], vert[3][1]}, {vert[4][0], vert[4][1]}, {vert[5][0], vert[5][1]}};
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), temp, GL_STATIC_DRAW);
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

void BackgroundRectangle::draw()
{
    bindProgram();
    bindBuffers();
    glUniform1f(0, 1.0f);  // scale  in vertex shader
    glUniform2f(1, 0.0f, 0.0f);  // center in vertex shader
    glUniform3f(2, 1.0, 1.0, 1.0);
    glUniform3f(3, 0.0, 0.0, 0.0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop(int seed, int n);
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
       ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
       ; // do something
    }
}

// ==========================================================================
void MyWin::MainLoop(int seed, int n) {
   ViewportOne(0,0,wd,ht);

   //MyTri   trian;
   Linetab lines(seed, n); //constructed with seed
   BackgroundRectangle background;
   bool col = false;
   int previousMove = 0;
   int previousRotation = 0;
   lines.setAngles();
   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      //trian.draw();
      background.draw();
      lines.drawLines();

      AGLErrors("main-afterdraw");
      WaitForFixedFPS(1.0/60); //same speed for all devices
      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

      if(lines.getWin()){
          lines.animation();
      }

      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
          col = lines.collision();
          if(!(previousMove == 1 && col)) {
              lines.movePlayerDown();
              previousMove = 1;
          }
      } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
          col = lines.collision();
          if(!(previousMove == 2 && col)) {
              lines.movePlayerUp();
              previousMove = 2;
          }
      } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
          col = lines.collision();
          if(!(previousRotation == 1 && col)) {
              lines.setPlayerAngle(1);
              previousRotation = 1;
          }
      } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
          col = lines.collision();
          if(!(previousRotation == 2 && col)) {
              lines.setPlayerAngle(-1);
              previousRotation = 2;
          }
      }


   } while( (glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[]) {
   MyWin win;
   win.Init(800,800,"Zadanie 3",0,33);
   int seed,n;
   if(argc == 3) {
       seed = atoi(argv[1]);
       n = atoi(argv[2]);
   } else{
       seed = 2137;
       n = 10;
   }
   win.MainLoop(seed,n);
   return 0;
}
