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



void seanLevel();
void seanrungame();
void seanrungame2();
void updateEnemies();
void SeanEnemiesVertical(int start, int end, int yBoundary, int margin, Entity enemies[]);
void SeanEnemiesHorizontal(int start, int end, int xBoundary, int margin, Entity enemies[]);
bool SeanCheckCollision(Entity &enemy);
void SeanDrawRect(float x, float y, float width, float height, float r, float g, float b);

#endif
