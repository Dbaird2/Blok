// Updated rbarreyroheader.h
#ifndef RBARREYRO_HEADER_H
#define RBARREYRO_HEADER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "Global.h"
using namespace std;

// Coin structure: pulsating & rotating collectible
struct Coin {
    float x, y;
    bool collected;
    // animation parameters
    float baseY;
    float amplitude;
    float frequency;
    float angle;
    int value;
};
//extern vector<Coin> coins;
// Forward declarations
struct RB_Entity;
struct Enemy;
struct Coin;

// Core game loop
void rbarreyroRunGame();
void rjEndCredit();
void russLevel();
// Level setup & draw utilities
void RB_InitializeLevel();
void RB_DrawColoredRect(float x, float y, float w, float h,
                       float r, float g, float b);
bool RB_CheckEntityCollision(const RB_Entity& a,
                             const RB_Entity& b);

// Updated enemy & coin logic
void RB_UpdateEnemies();
void RB_DrawEnemies();
void RB_UpdateCoins(std::vector<Coin>& coins);
void RB_DrawCoins(std::vector<Coin>& coins);
void RB_CheckCoinCollection(std::vector<Coin>& coins);

// Entity structure
struct RB_Entity {
    float x, y;
    float width, height;
    float speed;
    bool isActive;
};

// Enemy types
enum EnemyType { CHASER, ORBITER };

// Enemy structure: chasers pursue player, orbiters circle a center
struct Enemy {
    RB_Entity base;
    EnemyType type;
    // ORBITER-specific
    float centerX, centerY;
    float angle;
    // CHASER-specific
    float detectionRadius;
};


#endif // RBARREYRO_HEADER_H
