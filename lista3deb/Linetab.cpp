//
// Created by dominik on 05.11.2020.
//

#include "Linetab.h"

Linetab::Linetab(unsigned int seed, unsigned int n): num(n){
    step = 2.0f / (2*num); //needed to estimate center of the box where line will be put in
    plx = -1.0f + step;
    ply = -1.0f + step;
    tab.resize(num);
    angles.push_back(90);
    srand(seed);

    for(int i = 0; i < num; ++i){
        for(int j = 0; j < num; ++j) {
            std::unique_ptr<MyLine> ptr(new MyLine());
            tab[i].push_back(std::move(ptr));
        }
    }
    for(int i = 1; i < num*num; ++i){
        angles.push_back(rand()%360);
    }
    absoluteCoords();
    tab[0][0]->setColor(1,0,0);
    // printPositions();
}

void Linetab::setAngles()
{
    for(int i = 0; i < num; i++){
        for(int j = 0; j < num; j++) {
            tab[i][j]->rotation(angles[i * num + j]);
        }
    }
}

void Linetab::setPlayerAngle(int x)
{
    angles[0] += x;
    tab[0][0]->rotation(angles[0]);
}

void Linetab::movePlayerUp()
{
    plx += 0.01f * cos(angles[0] * (M_PI/180)) - 0.0f * sin(angles[0] * (M_PI/180));
    ply += 0.01f * sin(angles[0] * (M_PI/180)) + 0.0f * cos(angles[0] * (M_PI/180));
}

void Linetab::movePlayerDown()
{
    plx -= 0.01f * cos(angles[0] * (M_PI/180)) - 0.0f * sin(angles[0] * (M_PI/180));
    ply -= 0.01f * sin(angles[0] * (M_PI/180)) + 0.0f * cos(angles[0] * (M_PI/180));
}

void Linetab::drawLines(){
    float lx = -1.0f + step;
    float ly = -1.0f + step;
    for(int i = 0; i < num; i++){
        for(int j = 0; j < num; j++){
            if(i != 0 || j != 0) {
                tab[i][j]->draw(lx,ly, num);
            }
            else{
                tab[0][0]->draw(plx,ply,num);
            }
            lx += 2*step; // we multiply step by 2 to determine the center of the next box
        }
        lx = -1.0f + step;
        ly += 2*step;
    }
}

void Linetab::absoluteCoords(){
    float midx = -1.0f + step;
    float midy = -1.0f + step;//center of lines
    for(int i = 0; i < num; i++){
        std::vector<std::pair<float,float>> row;
        for(int j = 0; j < num; j++){//midx, midy to 0 dla wzoru
            float rotatedx = step * cos(angles[i * num + j] * (M_PI/180)) - 0.0f * sin(angles[i * num + j] * (M_PI/180));
            float rotatedy = step * sin(angles[i * num + j] * (M_PI/180)) + 0.0f * cos(angles[i * num + j] * (M_PI/180));
            rotatedx += midx;
            rotatedy += midy;
            //std::cout << i << j << "  " << rotatedx << " " << rotatedy << " : "  << std::endl;
            row.push_back(std::make_pair(rotatedx,rotatedy));
            midx += 2*step; // we multiply step by 2 to determine the center of the next box
        }
        positions.push_back(row);
        midx = -1.0f + step;
        midy += 2*step;
    }
}

float Linetab::vectorProduct(float x1, float y1, float x2, float y2, float x3, float y3){
    float xres1 = x3 - x1, yres1 = y3 - y1, xres2 = x2 - x1, yres2 = y2 - y1;
    return xres1*yres2 - xres2*yres1;
}


bool Linetab::lineIntersection(float midenemyx, float midenemyy, float enemyrx, float enemyry){//enemy rotated mają już prawidłowe pozycje
    float plrotatedx = step * cos(angles[0] * (M_PI/180)) - 0.0f * sin(angles[0] * (M_PI/180));
    float plrotatedy = step * sin(angles[0] * (M_PI/180)) + 0.0f * cos(angles[0] * (M_PI/180));

    float plrx = plx + plrotatedx;//punkt A
    float plry = ply + plrotatedy;

    float plrminusx = plx - plrotatedx; //punkt B
    float plrminusy = ply - plrotatedy;
    //enemyrx, enemyry punkt C

    float enemyrminusx = midenemyx - (enemyrx - midenemyx);  //punkt D
    float enemyrminusy = midenemyy - (enemyry - midenemyy);

    float v1 = vectorProduct(enemyrminusx, enemyrminusy, enemyrx, enemyry, plrminusx, plrminusy);
    float v2 = vectorProduct(enemyrminusx, enemyrminusy, enemyrx, enemyry, plrx, plry);
    float v3 = vectorProduct(plrminusx, plrminusy, plrx, plry, enemyrminusx, enemyrminusy);
    float v4 = vectorProduct(plrminusx, plrminusy, plrx, plry, enemyrx, enemyry);

    if(v1 * v2 < 0 && v3 * v4 < 0){
        return true;
    }else{
        return false;
    }
}

bool Linetab::collision(){
    float midenemyx = -1.0f + step;
    float midenemyy = -1.0f + step;//rysuje środki odcinków
    for(int i = 0; i < num; i++){
        for(int j = 0; j < num; j++){
            if(i!= 0 || j != 0){
                if(lineIntersection(midenemyx, midenemyy, positions[i][j].first, positions[i][j].second)){
                    if(i == num - 1 && j == num - 1){
                        std::cout << "Win :" << glfwGetTime() <<std::endl;
                        return true;
                    }else{
                        std::cout << "Lost" << std::endl;
                        return true;
                    }
                    std::cout << "Intersection with: " << i << j << std::endl;
                }
            }
            midenemyx += 2*step;
        }
        midenemyx = -1.0f + step;
        midenemyy += 2*step;
    }
    return false;
}

void Linetab::playerPosition(){ //player dobrze obliczany
    float plrotatedx = step * cos(angles[0] * (M_PI/180)) - 0.0f * sin(angles[0] * (M_PI/180));
    float plrotatedy = step * sin(angles[0] * (M_PI/180)) + 0.0f * cos(angles[0] * (M_PI/180));
    float plrx = plx + plrotatedx;//punkt A
    float plry = ply + plrotatedy;
    float plrminusx = plx - plrotatedx; //punkt B
    float plrminusy = ply - plrotatedy;
    std::cout << "Player : " << plrx << " " << plry << " : " << plrminusx << " " << plrminusy << std::endl;
}

void Linetab::printPositions(){
    float midenemyx = -1.0f + step;
    float midenemyy = -1.0f + step;//rysuje środki odcinków
    for(int i = 0; i < num; i++){
        for(int j = 0; j < num; j++){
            float enemyrminusx = midenemyx - (positions[i][j].first - midenemyx);  //punkt D
            float enemyrminusy = midenemyy - (positions[i][j].second - midenemyy);
            std::cout << i << j << " " <<  positions[i][j].first << " " << positions[i][j].second << " : " << enemyrminusx << " " << enemyrminusy << std::endl;
            midenemyx += 2*step;
        }
        midenemyx = -1.0f + step;
        midenemyy += 2*step;
    }
}
