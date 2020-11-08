// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <array>
#include <AGL3Window.hpp>
#include <Linetab.h>
#include <MyLine.h>

//#include "AGL3Window.hpp"
//#include "AGL3Drawable.hpp"
// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class Background : public AGLDrawable
{
public:
    Background(int n, GLfloat xStart = -1, GLfloat yStart = 1, GLfloat xEnd = 1, GLfloat yEnd = -1);
    void setShaders();
    void setBuffers();
    void draw();
private:
    std::array<std::array<GLfloat,2>, 6> squareVertices;
    float num;
};

Background::Background(int n,GLfloat xStart, GLfloat yStart, GLfloat xEnd, GLfloat yEnd)
{
    num = 0.5f * n;
    squareVertices[0][0] = xStart;
    squareVertices[0][1] = yStart;
    squareVertices[1][0] = xEnd;
    squareVertices[1][1] = yStart;
    squareVertices[2][0] = xStart;
    squareVertices[2][1] = yEnd;
    squareVertices[3][0] = xEnd;
    squareVertices[3][1] = yEnd;
    squareVertices[4][0] = xStart;
    squareVertices[4][1] = yEnd;
    squareVertices[5][0] = xEnd;
    squareVertices[5][1] = yStart;

    setShaders();
    setBuffers();
}
void Background::setShaders()
{
    compileShaders(R"END(
                   #version 330
                   #extension GL_ARB_explicit_uniform_location : require
                   #extension GL_ARB_shading_language_420pack : require
                   layout(location = 0) in vec2 pos;
                   layout(location = 2) uniform vec3  first_color;
                   layout(location = 3) uniform vec3  second_color;
                   out vec2 pvec;
                   flat out vec3 color1;
                   flat out vec3 color2;

                   void main(void) {
                       vec2 p = pos;
                       gl_Position = vec4(p, 0.0, 1.0);
                       color1 = first_color;
                       color2 = second_color;
                       pvec = pos;
                   }
                   )END", R"END(
                          #version 330
                          #extension GL_ARB_explicit_uniform_location : require
                          #extension GL_ARB_shading_language_420pack : require
                          layout(location = 4) uniform float n;
                          flat in vec3 color1;
                          flat in vec3  color2;
                          in vec2 pvec;
                          void main(void) {
                                float total = floor(pvec.x * n) + floor(pvec.y * n);
                                bool isEven = mod(total,2.0) == 0.0;
                                 gl_FragColor = (isEven)? vec4(color1,1) : vec4(color2, 1);
                          }
                          )END");
}



void Background::setBuffers()
{
    bindBuffers();
    GLfloat temp[6][2] = {{squareVertices[0][0], squareVertices[0][1]},
                          {squareVertices[1][0], squareVertices[1][1]},
                          {squareVertices[2][0], squareVertices[2][1]},
                          {squareVertices[3][0], squareVertices[3][1]},
                          {squareVertices[4][0], squareVertices[4][1]},
                          {squareVertices[5][0], squareVertices[5][1]}};
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

void Background::draw()
{
    bindProgram();
    bindBuffers();
    glUniform3f(2, 1.0, 1.0, 1.0);
    glUniform3f(3, 0.0, 0.0, 0.0);
    glUniform1f(4, num);
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

   Linetab lines(seed, n); //constructed with seed
   Background background(n);
   bool col = false;
   int previousMove = 0;
   int previousRotation = 0;
   lines.setAngles();
   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
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
