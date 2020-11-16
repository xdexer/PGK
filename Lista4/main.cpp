#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <array>
#include <AGL3Window.hpp>
#include <Camera.h>
#include <Sphere.h>

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
    virtual void MousePosCB(double xp, double yp);
    virtual void ScrollCB(double xp, double yp);
    void MainLoop(int seed, int n);
    void processInput();
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

void MyWindow::processInput()
{
    if (glfwGetKey(win(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win(), true);
    if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(0, deltaTime);
    if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(1, deltaTime);
    if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(2, deltaTime);
    if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(3, deltaTime);
}

void MyWindow::MousePosCB(double xp, double yp) {
    if(firstMouse)
    {
        lastX = xp;
        lastY = yp;
        firstMouse = false;
    }
    float xoffset = xp - lastX;
    float yoffset = lastY - yp;
    lastX = xp;
    lastY = yp;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void MyWindow::ScrollCB(double xp, double yp) {
    camera.ProcessMouseScroll(yp);
}

void MyWindow::MainLoop(int seed, int n) {
    ViewportOne(0,0,wd,ht);
    bool col = false;
    int previousMove = 0;
    int previousRotation = 0;
    Sphere player;

    do {
        glClear( GL_COLOR_BUFFER_BIT );
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        WaitForFixedFPS(1.0/60); //same speed for all devices
        //cameraShader.bindProgram();
        processInput(); //keyboardinput
        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),1.0f,0.1f,100.0f);
        camera.setUniformMat4(3, projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        camera.setUniformMat4(4, view);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        player.draw();
        AGLErrors("main-afterdraw");

        // input/output events
        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();

        if (glfwGetKey(win(), GLFW_KEY_T) == GLFW_PRESS){
            std::cout << glm::to_string(view) << std::endl;
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
