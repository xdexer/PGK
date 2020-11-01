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
#include <memory>
#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
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
      bindProgram();
      glDrawArrays(GL_TRIANGLES, 0, 3);
   }
};


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================
class MyLine : public AGLDrawable {
public:
    MyLine() : AGLDrawable(0) {
    }
    void setShaders() {
        compileShaders(R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  center;
         out vec4 vtex;

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END", R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  cross_color;
         out vec4 color;

         void main(void) {
            color = vec4(cross_color,1.0);
         }

      )END");
    }
    void setBuffers() {
        bindBuffers();
        GLfloat vert[2][2]; // lines
        printf("%f %f \n", this->x, this->y);
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

    void rotation(int angle){
        this->angle = angle;
        this->x = 1.0f * cos(this->angle * (M_PI/180)) - 0.0f * sin(this->angle * (M_PI/180));
        this->y = 1.0f * sin(this->angle * (M_PI/180)) + 0.0f * cos(this->angle * (M_PI/180));
        setShaders();
        setBuffers();
    }
    void draw(float tx, float ty, int num){
        bindBuffers();
        //printf("%f %f \n", this->x, this->y);
        glUniform1f(0, 1.0/num);  // scale  in vertex shader (scaling size)
        glUniform2f(1, tx, ty);  // center in vertex shader
        glUniform3f(3, line_color[0],line_color[1],line_color[2]);

        glDrawArrays(GL_LINES, 0, 2);
    }
    void setColor(float r, float g, float b){
        line_color[0]=r;line_color[1]=g;line_color[2]=b;
    }

private:
    GLfloat line_color[3] = { 0.0, 1.0, 0.0 };
    int angle;
    float x,y;
};

class Linetab{
public:
    Linetab(unsigned int seed, unsigned int n): num(n){
        plx = -0.9f;
        ply = -0.9f;
        tab = new MyLine*[num];

        angles.push_back(90);
        srand(seed);

        for(int i = 0; i < num; ++i){
            tab[i] = new MyLine[num];
        }

        for(int i = 1; i < num*num; ++i){
            angles.push_back(rand()%360);
        }
        /*for(int i = 0; i < 100; ++i){
           std::cout << angles[i] << std::endl;
        } ANGLES WORK !*/
        //printf("%s", "done enemyline array \n");
    }

    void setAngles()
    {
        for(int i = 0; i < num; i++){
            for(int j = 0; j < num; j++) {
                tab[i][j].rotation(angles[i * num + j]);
            }
        }
    }

    void setPlayerAngle(int x)
    {
        angles[0] += x;
        tab[0][0].rotation(angles[0]);
    }

    void movePlayerUp()
    {
        plx += 0.01f * cos(angles[0] * (M_PI/180)) - 0.0f * sin(angles[0] * (M_PI/180));
        ply += 0.01f * sin(angles[0] * (M_PI/180)) + 0.0f * cos(angles[0] * (M_PI/180));
    }

    void movePlayerDown()
    {
        plx -= 0.01f * cos(angles[0] * (M_PI/180)) - 0.0f * sin(angles[0] * (M_PI/180));
        ply -= 0.01f * sin(angles[0] * (M_PI/180)) + 0.0f * cos(angles[0] * (M_PI/180));
    }

    void drawLines(){ //przeskalować odstępy i wielkości linii
        float lx = -0.9;
        float ly = -0.9;
        for(int i = 0; i < num; i++){
            for(int j = 0; j < num; j++){
                if(i != 0 || j != 0) {
                    tab[i][j].draw(lx,ly, num);
                }
                else{
                    tab[0][0].draw(plx,ply,num);
                }
                lx += 2.0f/num;
            }
            lx = -0.9;
            ly += 2.0f/num;
        }
    }

private:
    MyLine **tab;
    std::vector<int> angles;
    int num;
    float plx, ply;
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

bool touched(float cx, float cy, float tx, float ty)
{
   float r = 2.0f / 23.0f;
   float distance = sqrt(pow(cx-tx,2.0) + pow(cy-ty,2.0));
   if(distance < 2 * r){ 
      return false;
   }else{
      return true;
   }
}

// ==========================================================================
void MyWin::MainLoop(int seed, int n) {
   ViewportOne(0,0,wd,ht);

   //EnemyLine line;
   //MyTri   trian;
   Linetab lines(seed, n); //constructed with seed
   lines.setAngles();
   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      //trian.draw();
      //line.draw(tx,ty);
      lines.drawLines();
      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
         lines.movePlayerDown();
      } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
         lines.movePlayerUp();
      } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
          lines.setPlayerAngle(1);
      } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
          lines.setPlayerAngle(-1);
      }


   } while( (glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[]) {
   MyWin win;
   win.Init(800,600,"Zadanie 3",0,33);
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
