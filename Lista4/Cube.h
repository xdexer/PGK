//
// Created by dominik on 16.11.2020.
//

#ifndef LISTA4_CUBE_H
#define LISTA4_CUBE_H
#include <AGL3Drawable.hpp>
#include <glm/glm.hpp>

class Cube: public AGLDrawable{
public:
    Cube() : AGLDrawable(0) {
        setBuffers();
        setShaders();
    }

    void setBuffers();
    void setShaders();
    void draw();
private:
    unsigned int VBO, VAO;
    glm::vec3 cubePosition[1] = {glm::vec3(0.0f, 0.0f, 0.0f)}; //center of "sphere"
};


#endif //LISTA4_CUBE_H
