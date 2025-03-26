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
void drawRect();
void updateEnemies();
void SeanDrawRect();
#endif
