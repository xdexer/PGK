//
// Created by dominik on 05.11.2020.
//

#ifndef LISTA3DEB_LINETAB_H
#define LISTA3DEB_LINETAB_H
#include <vector>
#include <array>
#include <random>
#include <memory>
#include <iomanip>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include <MyLine.h>

class Linetab{
public:
    Linetab(unsigned int seed, unsigned int n);
    void setAngles();
    void setPlayerAngle(int x);
    void movePlayerUp();
    void movePlayerDown();
    void drawLines();
    void absoluteCoords();
    float vectorProduct(float x1, float y1, float x2, float y2, float x3, float y3);
    bool lineIntersection(float midenemyx, float midenemyy, float enemyrx, float enemyry);
    bool collision();
    void playerPosition();
    void printPositions();
    bool getWin();
    void animation();
private:
    std::vector<std::vector<std::unique_ptr<MyLine>>> tab;
    std::vector<int> angles;
    std::vector<std::vector<std::pair<float,float>>> positions;
    int num;
    float plx, ply, step;
    bool win;
};



#endif //LISTA3DEB_LINETAB_H
