#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <AGL3Window.hpp>
#include <Linetab.h>
#include <MyLine.h>
#include <Camera.h>

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
float lastX = 800.0f / 2.0f;
float lastY = 800.0f / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


class MyWindow : public AGLWindow
{
public:
    MyWindow(){};
    MyWindow(int _wd, int _ht, const char *name, int vers, int fullscr=0) : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop(int seed, int n);
};

// ==========================================================================
void MyWindow::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
        ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
        ; // do something
    }
}

void MyWindow::MainLoop(int seed, int n) {
    ViewportOne(0,0,wd,ht);
    Linetab lines(seed, n);
    bool col = false;
    int previousMove = 0;
    int previousRotation = 0;
    lines.setAngles();

    //AGLDrawable cameraShader(2); //make camera compile shaders from files
    //cameraShader.compileShadersFromFile(
    //        "/home/dominik/Desktop/ISSP/2020-2021/PGK/Lista4/Shaders/camera.vs",
    //        "/home/dominik/Desktop/ISSP/2020-2021/PGK/Lista4/Shaders/camera.fs");



    do {
        glClear( GL_COLOR_BUFFER_BIT );
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        WaitForFixedFPS(1.0/60); //same speed for all devices
        //cameraShader.bindProgram();

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),1.0f,0.1f,100.0f);
        camera.setUniformMat4(3, projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        camera.setUniformMat4(4, view);

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        lines.drawLines();
        AGLErrors("main-afterdraw");

        // input/output events
        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();


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
    MyWindow win;
    win.Init(800,800,"Zadanie 4",0,33);
    int seed, n;
    if(argc == 3){
        seed = atoi(argv[1]);
        n = atoi(argv[2]);
    }else{
        seed = 2137;
        n = 10;
    }
    //glEnable(GL_DEPTH_TEST);
    win.MainLoop(seed, n);
    return 0;
}
