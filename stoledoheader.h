#ifndef SEAN_H
#define SEAN_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <iostream>
#include "Global.h"

struct Entity {
    float x, y;
    float width, height;  
    float speed;
    float dir;
};
extern Entity enemies[30];
extern Entity goal;




void seanrungame();
void updateEnemies();
void SeanDrawRect();
void SeanEnemyMovementVertical();
void SeanEnemyMovementHorizontal();
void SeanCheckCollision((float x, float y, float width, float height, float r, float g, float b));
#endif
