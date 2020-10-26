#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>

using namespace glm;

#include "common/shader.hpp"
int main(void)
{

    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Zadanie1", NULL, NULL);

    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Initialize GLEW

    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //Vertex Array Object
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //Load shaders
    GLuint shaders = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    static const GLfloat g_triangles_buffer_data[] = {
        //first triangle
        -0.75f, -1.0f, 0.0f,
        0.75f, -1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        //second triangle
        -0.75f, 0.0f, 0.0f,
        0.75f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    GLuint triangles_buffer;
    glGenBuffers(1, &triangles_buffer);

    GLuint indices_buffer;
    glGenBuffers(1, &indices_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, triangles_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangles_buffer_data), g_triangles_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
       0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
       3,                  // size
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       0,                  // stride
       (void*)0            // array buffer offset
    );
    glEnableVertexAttribArray(0);

    //MAIN LOOP
    do{
        // Clear the screen.
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT);
        glUseProgram(shaders);
        glBindVertexArray(triangles_buffer);

        // Draw the triangles !
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
	return 0;
}

