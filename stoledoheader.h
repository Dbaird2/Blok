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
<<<<<<< HEAD
void SeanEnemiesVertical(int start, int end, int yBoundary);
void SeanEnemiesHorizontal(int start, int end, int xBoundary);
=======
void SeanDrawRect();
void SeanEnemiesVertical(int start, int end, int yBoundary,Entity &enemy);
void SeanEnemiesHorizontal(int start, int end, int xBoundary,Entity &enemy);
>>>>>>> e95b532 ( CHANGES)
bool SeanCheckCollision(Entity &enemy);
void SeanDrawRect(float x, float y, float width, float height, float r, float g, float b);

#endif
