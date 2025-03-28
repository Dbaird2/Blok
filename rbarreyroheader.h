#ifndef RBARREYRO_HEADER_H
#define RBARREYRO_HEADER_H


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <iostream>
#include "Global.h"

struct Entity;
struct Coin;

void rbarreyroRunGame();
void RussdrawRect();
void RussupdateEnemies();
void rjEndCredit();
void RB_InitializeLevel();
void RB_DrawColoredRect(float x, float y, float w, float h, float r, float g, float b);
void RB_UpdateEnemyMovement();
bool RB_CheckEntityCollision(const Entity& a, const Entity& b);


// Entity structure declaration
struct Entity {
    float x, y;
    float width, height;
    float speed;
    bool isActive;
};

// Coin structure declaration
struct Coin {
    float x, y;
    bool collected;
    float direction;
};

#endif
