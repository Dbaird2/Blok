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
#include "caroline.h"
#include "dbairdheader.h"
using namespace std;

// Game Constants
#define RUSS_GRID_SIZE 20
#define NUM_PORTALS 4
const int POWERUP_SIZE = 10;
const int NUM_ENEMIES = 4;
const int NUM_COINS   = 10;
const float GOAL_X    = 800.0f;
const float GOAL_Y    = 250.0f;

// Entity structure
struct RB_Entity {
    float x, y;
    float width, height;
    float speed;
    bool isActive;
};

// Enemy types
enum EnemyType { CHASER, ORBITER };

// Enemy structure
struct Enemy {
    RB_Entity base;
    EnemyType type;
    float centerX, centerY;
    float angle;
    float detectionRadius;
};

// Coin structure
struct Coin {
    float x, y;
    bool collected;
    float baseY;
    float amplitude;
    float frequency;
    float angle;
    int value;
};

// Power-up System
enum PowerUpType { SPEED };

struct PowerUp {
    float x, y;
    PowerUpType type;
    bool active;
    bool collected;

    PowerUp(float x_, float y_, PowerUpType t, bool a)
        : x(x_), y(y_), type(t), active(a), collected(false) {}
};

// Portal declarations
extern Teleportal portals[NUM_PORTALS];

// Global Game State
extern std::vector<PowerUp> powerUps;
extern float gAnimTime;
extern RB_Entity Russ_goal;
extern std::vector<Enemy> rb_enemies;
extern std::vector<Coin> coins;
extern int collectedCoins;
extern int Deathcounter;

// Core game functions
void rbarreyroRunGame();
void rjEndCredit();
void russLevel();
void check_quit(XEvent *e);

// Level setup & draw utilities
void RB_InitializeLevel();
void RB_DrawColoredRect(float x, float y, float w, float h,
                       float r, float g, float b);
bool RB_CheckEntityCollision(const RB_Entity& a,
                             const RB_Entity& b);
void RB_DrawWalls();
void RB_CheckWallCollision();
void DrawDeathCounter(int DeathCount);
void DrawBorder(float x, float y, float w, float h, float thickness);

// Enemy & coin logic
void RB_UpdateEnemies();
void RB_DrawEnemies();
void RB_UpdateCoins(std::vector<Coin>& coins);
void RB_DrawCoins(std::vector<Coin>& coins);
int RB_CheckCoinCollection(std::vector<Coin>& coins, int collectedCoins);

// Power-up system
void UpdatePowerUps();
void DrawPowerUps();

// Physics and collision
void RB_Physics(int wall_size, int growing_size, int growing_enemy_check, Grid grid[]);
void RB_CheckBorderCollision();

// Maze structure
extern Grid russ_grid[RUSS_GRID_SIZE];
extern int russ_height[RUSS_GRID_SIZE];
extern int russ_width[RUSS_GRID_SIZE];
extern int russxpos[RUSS_GRID_SIZE];
extern int russypos[RUSS_GRID_SIZE];

#endif // RBARREYRO_HEADER_H
