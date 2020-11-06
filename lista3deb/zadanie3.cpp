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
class MyTri : public AGLDrawable {
public:
   MyTri() : AGLDrawable(0) {
      setShaders();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         out vec2 vpos;

         void main(void) {
            const vec2 vertices[3] = vec2[3](vec2( 0.9, -0.9),
                                             vec2(-0.9, -0.9),
                                             vec2( 0.9,  0.9));
            const vec4 colors[]    = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0));
         
            vcolor = colors[gl_VertexID];
            vpos = vertices[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END", R"END(

         #version 330 
         in  vec4 vcolor;
         out vec4 color;
         in vec2 vpos;
         void main(void) {
            float r = vpos.x * vpos.x + vpos.y * vpos.y;
            if(r < 0.01){
               color = vec4(1,1,1,0);
            }
            else{
            color = vcolor;
            }
         } 

      )END");
   }
   void draw() {
      //bindProgram();
      glDrawArrays(GL_TRIANGLES, 0, 3);
   }
};

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
   bool col = false;
   int previousMove = 0;
   int previousRotation = 0;
   lines.setAngles();
   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      //trian.draw();
      lines.drawLines();

      AGLErrors("main-afterdraw");
      WaitForFixedFPS(1.0/60); //same speed for all devices
      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

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
