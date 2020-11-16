//
// Created by dominik on 16.11.2020.
//

#ifndef LISTA4_SPHERE_H
#define LISTA4_SPHERE_H
#include <AGL3Drawable.hpp>
#include <glm/glm.hpp>

class Sphere: public AGLDrawable{
public:
    Sphere() : AGLDrawable(0) {
        setBuffers();
        setShaders();
    }

    void setBuffers();
    void setShaders();
    void draw();
private:
    unsigned int VBO, VAO;
    glm::vec3 spherePosition[1] = {glm::vec3(0.0f, 0.0f, 0.0f)};
};


#endif //LISTA4_SPHERE_H
