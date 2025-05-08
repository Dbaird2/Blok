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
 int RB_CheckCoinCollection(std::vector<Coin>& coins, int collectedCoins);
 
 // Power-up system
 struct PowerUp;
 void UpdatePowerUps();
 void DrawPowerUps();
 
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

//-----------------------------------------------------------------------
//lvl 10 types
struct L10_Projectile {
    float x,y,w,h, dx,dy, speed;
    bool  active;
};

struct L10_PowerUp {
    float x,y;
    int   type;
    bool  active, collected;
};

struct L10_Laser {
    float x,y,length,angle;
    float toggleRate, phase;
    bool  on;
};

//lvl 10 globals 
extern std::vector<L10_Projectile> projectiles10;
extern std::vector<RB_Entity>      triangleEnemies10;
extern std::vector<L10_PowerUp>    powerUps10;
extern std::vector<Coin>           coins10;
extern RB_Entity                   portalA10, portalB10;
extern std::vector<L10_Laser>      lasers10;
extern RB_Entity                   growingBox10;
extern float                       growFactor10;
extern bool                        growing10;
extern RB_Entity                   goal10;

// The wall for Level 10 
extern Grid        level10Grid[];
extern const int   LEVEL10_GRID_SIZE;

// LEVEL 10 API
void InitLevel10();
void UpdateLevel10(double dt);
void DrawLevel10();
 
 #endif // RBARREYRO_HEADER_H
