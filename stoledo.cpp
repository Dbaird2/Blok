//Sean Toledo
//Date : Spring 2025
//Blok
//
#include <iostream>
using namespace std;
//#include <stdio.h>
//#include <random>
//#include <unistd.h>
//#include <cstdlib>
//#include <ctime>
//#include <cstring>
#include <cmath>
//#include <X11/Xlib.h>
//#include <X11/keysym.h>
//#include <GL/glx.h>
#include "fonts.h"
//#include <pthread.h>
//#include "functions.h"
#include "dbairdheader.h"
#include "Global.h"
//#include "Image.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "stoledoheader.h"
#include "rbarreyroheader.h"
#include <chrono>
//Wall
#define SEAN2_GRID_SIZE 1 
#define SEAN_GRID_SIZE 4
#define SEAN_NUM_COINS 10
#define SNAKE_LENGTH 20
float edge = 100.0f;
struct SnakeSegment {
    float x, y;
};
SnakeSegment snake[SNAKE_LENGTH];
float snakeSpeed = 2.0f;
float snakeBaseSpeed = 2.0f; 
int snakeSpeedTimer = 0;     
#define SNAKE2_LENGTH 20
SnakeSegment snake2[SNAKE2_LENGTH];
float snake2Speed = 2.0f;
float snake2BaseSpeed = 2.0f;
int snake2SpeedTimer = 0;
Grid sean_grid[SEAN_GRID_SIZE];
Grid sean2_grid[SEAN2_GRID_SIZE];
int wallX = g.xres / 2 - 50;  
int wallY = g.yres / 2 - 50;       
int topBarWidth = 100;
int topBarHeight = 5;
int verticalHeight = 100;
int verticalWidth = 5;
bool showPowerUpMessage = false;
bool powerUpActive = false;
int seanCollectedCoins = 0;
vector<Coin> seanCoins;
//SlowMo
Entity slowmoPowerUp = {g.xres / 2.0f - 10, g.yres / 2.0f - 10, 20, 20, 0, 0};
bool powerUpCollected = false;
int powerUpTimer = 0;
const int POWERUP_DURATION = 300; // ~5 seconds
//Goal
Entity goal = {800, 250, 25, 25, 0, 0};
Entity goal2 = {800, 250, 25, 25, 0, 0};
//Enemies
Entity enemies[30] = {
    {300, 200, 20, 20, 30.0, 1},
    {500, 400, 20, 20, 30.0, -1},
    {300, 200, 20, 20, 30.0, 1},
    {500, 400, 20, 20, 30.0, -1}
};
int sean2_width[SEAN2_GRID_SIZE] = {
    5
};

int sean2_height[SEAN2_GRID_SIZE] = {
    40
};

int sean2xpos[SEAN2_GRID_SIZE] = {
    static_cast<int>(slowmoPowerUp.x + 50) 
};

int sean2ypos[SEAN2_GRID_SIZE] = {
    static_cast<int>(slowmoPowerUp.y) 
};
int sean_width[SEAN_GRID_SIZE] = {
    g.xres / 2,
    g.xres / 2,
    5,
    5
};
int seanxpos[SEAN_GRID_SIZE] = {
    g.xres / 2,
    g.xres / 2,
    5,
    g.xres - 5
};
int sean_height[SEAN_GRID_SIZE] = {
    5,
    5,
    g.yres,     
    g.yres      
};
int seanypos[SEAN_GRID_SIZE] = {
    g.yres - 5,
    0,
    0,
    0
};
void seanLevel()
{

    dasonLoadStruct(sean_grid, sean_height, sean_width, seanxpos, seanypos, SEAN_GRID_SIZE);
    dasonLoadStruct(sean2_grid, sean2_height, sean2_width, sean2xpos, sean2ypos, SEAN2_GRID_SIZE);
}

struct Projectile {
    float x, y;
    float dx, dy;
    float speed;
    float width, height;
    bool active;
};
using _clock        = std::chrono::steady_clock;
using _elapsed      = std::chrono::duration<double>;
using _time         = std::chrono::time_point<_clock, _elapsed>;
inline time_t current_time_t()
{
    return chrono::system_clock::to_time_t(chrono::system_clock::now());
}
const int MAX_PROJECTILES = 100;
Projectile projectiles[MAX_PROJECTILES];
Entity triangleEnemies[6] = {
    {
        static_cast<float>(g.xres) - edge - 20.0f,
        static_cast<float>(g.yres) - edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    },
    {
        static_cast<float>(g.xres) - edge - 20.0f, 
        edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    },
     {
        static_cast<float>(g.xres) / 2 ,
        static_cast<float>(g.yres) - edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    },
    {
        static_cast<float>(g.xres) / 2,
        edge, 
        20.0f, 20.0f,
        0.0f, 0.0f
    },
    {
        edge,
        static_cast<float>(g.yres) - edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    },
    {
        edge,
        edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    }
};

void SeanUpdateProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (!projectiles[i].active)
            continue;

        // Move projectile
        float speed = projectiles[i].speed * (powerUpActive ? 0.5f : 1.0f);
        projectiles[i].x += projectiles[i].dx * speed;
        projectiles[i].y += projectiles[i].dy * speed;
        float px = projectiles[i].x;
        float py = projectiles[i].y;

        // Check for collision with all walls in sean2_grid
        for (int j = 0; j < SEAN2_GRID_SIZE; ++j) {
            Grid &wall = sean2_grid[j];
            float wallLeft   = wall.x;
            float wallRight  = wall.x + wall.width;
            float wallTop    = wall.y + wall.height;
            float wallBottom = wall.y;
            // Point vs AABB collision
            bool collision = (px >= wallLeft && px <= wallRight &&
                              py >= wallBottom && py <= wallTop);
            if (collision) {
                //printf("Projectile %d hit wall %d at (%.1f, %.1f)\n", i, j, px, py);
                projectiles[i].active = false;
                break; // No need to check other walls
            }
        }
        // Kill projectile if off-screen
        if (px < 0 || px > g.xres || py < 0 || py > g.yres) {
            projectiles[i].active = false;
        }
    }
}
bool SeanCheckProjectileCollision(Projectile &p) {
    float playerLeft = player.pos[0] - player.width / 2;
    float playerRight = player.pos[0] + player.width / 2;
    float playerTop = player.pos[1] - player.height / 2;
    float playerBottom = player.pos[1] + player.height / 2;
    float pLeft = p.x;
    float pRight = p.x + p.width;
    float pTop = p.y;
    float pBottom = p.y + p.height;
    return (playerLeft < pRight &&
            playerRight > pLeft &&
            playerTop < pBottom &&
            playerBottom > pTop);
}
void SeanDrawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (projectiles[i].active) {
            SeanDrawRect(projectiles[i].x, projectiles[i].y,
                         projectiles[i].width, projectiles[i].height,
                         0.0f, .0f, 1.0f); 
        }
    }
}
void SeanFireProjectileAtPlayer(Entity &enemy) {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (!projectiles[i].active) {
            float dx = player.pos[0] - enemy.x;
            float dy = player.pos[1] - enemy.y;
            float dist = sqrt(dx * dx + dy * dy);
            dx /= dist;
            dy /= dist;
            projectiles[i] = {
                enemy.x, enemy.y,
                dx, dy,
                5.0f * (powerUpActive ? 0.5f : 1.0f), 
                5.0f, 5.0f,
                true
            };
            break;
        }
    }
}
//Clear Particles
void clearAllProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        projectiles[i].active = false;
    }
}

void SeanDrawTriangleEnemy(Entity &e) {
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(e.x, e.y);
    glVertex2f(e.x + e.width, e.y);
    glVertex2f(e.x + e.width / 2, e.y + e.height);
    glEnd();
}
//End Credit
void seanEndCredit(void) {
    Rect title;
    title.bot = 18;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 5: Sean Toledo");
}
//Draw Rectangle 
void SeanDrawRect(float x, float y, float width, float height,
                  float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);                   
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
//Enemy Movement
void SeanEnemiesVertical(int start, int end, 
        int yBoundary, int margin, Entity enemies[]) {
    for (int i = start; i < end; ++i) {
        float actualSpeed = enemies[i].speed * (powerUpActive ? 0.5f :1.0f);
        enemies[i].y += enemies[i].dir * actualSpeed;
        if (enemies[i].y <= margin || enemies[i].y >= yBoundary)
            enemies[i].dir *= -1;
    }
}
void SeanEnemiesHorizontal(int start, int end, 
        int xBoundary, int margin, Entity enemies[]) {
    for (int i = start; i < end; ++i) {
        float actualSpeed = enemies[i].speed * (powerUpActive ? 0.5f : 1.0f);
        enemies[i].x += enemies[i].dir * actualSpeed;
        if (enemies[i].x <= margin || enemies[i].x >= xBoundary - margin)
            enemies[i].dir *= -1;
    }
}
//Collision
bool SeanCheckCollision(Entity &enemy) {
    float playerLeft = player.pos[0] - player.width / 2;
    float playerRight = player.pos[0] + player.width / 2;
    float playerTop = player.pos[1] - player.height / 2;
    float playerBottom = player.pos[1] + player.height / 2;
    float enemyLeft = enemy.x;
    float enemyRight = enemy.x + enemy.width;
    float enemyTop = enemy.y;
    float enemyBottom = enemy.y + enemy.height;
    return (playerLeft < enemyRight &&
            playerRight > enemyLeft &&  
            playerTop < enemyBottom &&
            playerBottom > enemyTop);
}
void SeandrawDeathCounter(int deathCount) {
    Rect r;
    r.bot = 50;        
    r.left = 100;       
    r.center = 0;
    ggprint8b(&r, 16, 0xff0000ff, "Deaths:");
    ggprint8b(&r, 16, 0xff0000ff, "%d", deathCount);
}
int deathcounter = 0;
// in frames (3 seconds = 180 at 60 FPS)
int triangleShootingCooldownFrames = 0; 
void seanrungame() {
    //initAudio("background.wav");
    //dasonTimer(490, 840, 60);
    static bool initialized = false;
    //cout << "seanrungame" << endl;
    if (g.game_state == 4) {
        if (!initialized) {
            player.tempx = 50;
            player.tempy = 250;
            initialized = true;
            player.width = 15;
            player.height = 15;
        }
        if (!powerUpCollected) {
        SeanDrawRect(slowmoPowerUp.x, slowmoPowerUp.y,
                 slowmoPowerUp.width, slowmoPowerUp.height,
                 0.5f, 0.5f, 1.0f); // Blue rectangle
            }
        // Draw Player Box
        SeandrawDeathCounter(deathcounter);
        drawPlayerBox(0);
        SeanEnemiesVertical(0, 2, g.yres, edge, enemies);
        SeanEnemiesHorizontal(2, 4, g.xres, edge, enemies);
        SeanDrawProjectiles();
        for (int i = 0; i < 6; ++i) {
        SeanDrawTriangleEnemy(triangleEnemies[i]);
        SeanDrawRect(goal.x, goal.y, goal.width, goal.height, 0, 1, 0);
        for (int i = 0; i < 4; ++i)
        SeanDrawRect(enemies[i].x, enemies[i].y, enemies[i].width, 
                enemies[i].height,1, 0, 0);
        }
        SeanUpdateProjectiles();
        dasonDrawWalls(sean_grid, SEAN_GRID_SIZE);
        dasonPhysics(SEAN_GRID_SIZE, 0, 0, NULL);
        dasonDrawWalls(sean2_grid, SEAN2_GRID_SIZE);
        dasonPhysics(SEAN2_GRID_SIZE, 0, 0, NULL);
        if (showPowerUpMessage && !powerUpActive) {
        static int flashTimer = 0;
        flashTimer++;
        if ((flashTimer / 30) % 2 == 0) { // flashes every 30 frames
            Rect r;
            r.bot = g.yres / 2;
            r.left = g.xres / 2 - 100;
            r.center = 0;
            ggprint8b(&r, 16, 0x001269F, "Press SPACEBAR to use power up");
        }
        }
        if (g.key_states[XK_space]) {
        if (powerUpCollected && !powerUpActive) {
            powerUpActive = true;
            powerUpTimer = POWERUP_DURATION;
            powerUpCollected = false;
            showPowerUpMessage = false; 
        }
        }
        // Fire projectile every few frames
        if (!powerUpCollected && SeanCheckCollision(slowmoPowerUp)) {
        powerUpCollected = true;
        showPowerUpMessage = true;
        }
        static int fireCooldown = 0;
        if (triangleShootingCooldownFrames <= 0 &&
            !(player.pos[0] == 50 && player.pos[1] == 250)) {
            if (++fireCooldown > 15) {
                for (int i = 0; i < 6; ++i) {
                    SeanFireProjectileAtPlayer(triangleEnemies[i]);
                }
                fireCooldown = 0;
            }
        }
            for (int i = 0; i < 6; i++) {
                if (SeanCheckCollision(triangleEnemies[i])) {
                player.tempx = 50;
                player.tempy = 250; 
                deathcounter++;
                powerUpActive = false;
                clearAllProjectiles();
                triangleShootingCooldownFrames = 180; // RESET COOLDOWN
                
                }
            }
            for (int i = 0; i < 4; i++) {
            if (SeanCheckCollision(enemies[i])) {
            player.tempx = 50;
            player.tempy = 250; 
            deathcounter++;
            powerUpActive = false;
            clearAllProjectiles();
            triangleShootingCooldownFrames = 180; // RESET COOLDOWN
            
            }
            }
            if (SeanCheckCollision(goal)) {
                g.game_state = 12;
                if (g.amt_of_stars[2] < 1)
                    g.amt_of_stars[2]++;
            }
            for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (projectiles[i].active && 
                    SeanCheckProjectileCollision(projectiles[i])) {
            projectiles[i].active = false;
            player.tempx = 50;
            player.tempy = 250;
            deathcounter++;
            powerUpActive = false;
            clearAllProjectiles();
            triangleShootingCooldownFrames = 180; // RESET COOLDOWN
            }
            if (triangleShootingCooldownFrames > 0) {
            triangleShootingCooldownFrames--;
            }
            }
}
}
void seanrungame2()
{   
    static bool initialized2 = false;
    if (g.game_state == 12) {
        if (!initialized2) {
                player.tempx = 50;
                player.tempy = 250;
                initialized2 = true;
                seanCoins.clear();
                seanCollectedCoins = 0;
                player.width = 15;
                player.height = 15;
                for (int i = 0; i < SNAKE_LENGTH; ++i) {
            snake[i].x = 700 - i * 20;
            snake[i].y = 100;
            }
            for (int i = 0; i < SNAKE2_LENGTH; ++i) {
                snake2[i].x = 700 - i * 20;
                snake2[i].y =  g.yres - 100;
            }            
            seanCoins.push_back({150, 250, false, 250, 20.0f, 2.0f, 0.0f, 10});
            seanCoins.push_back({300, 350, false, 350, 20.0f, 2.5f, 0.0f, 10});
            seanCoins.push_back({600, 100, false, 100, 20.0f, 2.8f, 0.0f, 10});
            seanCoins.push_back({750, 250, false, 550, 20.0f, 2.0f, 0.0f, 10});
            seanCoins.push_back({900, 350, false, 150, 20.0f, 2.5f, 0.0f, 10});
            seanCoins.push_back({100, 100, false, 200, 20.0f, 2.8f, 0.0f, 10});
            seanCoins.push_back({300, 350, false, 350, 20.0f, 2.5f, 0.0f, 10});
            seanCoins.push_back({600, 100, false, 100, 20.0f, 2.8f, 0.0f, 10});
            seanCoins.push_back({750, 250, false, 550, 20.0f, 2.0f, 0.0f, 10});
            seanCoins.push_back({900, 350, false, 150, 20.0f, 2.5f, 0.0f, 10});
            seanCoins.push_back({100, 100, false, 200, 20.0f, 2.8f, 0.0f, 10});
        }
            drawPlayerBox(0);
            for (int i = 0; i < SNAKE_LENGTH; ++i) {
            SeanDrawRect(snake[i].x, snake[i].y, 10, 10, 0.0f, 0.0f, 0.0f); 
            }
            for (int i = 0; i < SNAKE2_LENGTH; ++i) {
                SeanDrawRect(snake2[i].x, snake2[i].y, 10, 10, 0.0f, 0.0f, 0.0f); 
            }
        
        Rect r;
        r.bot = 80;          
        r.left = 100;        
        r.center = 0;
        ggprint8b(&r, 16, 0x000000, "Coins: %d", seanCollectedCoins);
        SeanDrawRect(goal2.x, goal2.y, goal2.width, goal2.height, 0, 1, 0);
        if (SeanCheckCollision(goal2)&& seanCollectedCoins >= 50) {
                    g.game_state = 9;
                    if (g.amt_of_stars[2] < 1)
                        g.amt_of_stars[2]++;
                }
        RB_DrawCoins(seanCoins);
        RB_UpdateCoins(seanCoins);
        for (size_t i = 0; i < seanCoins.size(); i++) {
            if (!seanCoins[i].collected && RB_CheckEntityCollision(
                    {player.pos[0]-5, player.pos[1]-5, 10, 10, 0, true},
                    {seanCoins[i].x-10, seanCoins[i].y-10, 20, 20, 0, true})) {
                seanCoins[i].collected = true;
                seanCollectedCoins += seanCoins[i].value;
            }
        }
        for (int i = 0; i < SNAKE_LENGTH; ++i) {
        Entity snakeSegment;
        snakeSegment.x = snake[i].x;
        snakeSegment.y = snake[i].y;
        snakeSegment.width = 10;
        snakeSegment.height = 10;
        // Increase speed every second 
        snakeSpeedTimer++;
        if (snakeSpeedTimer >= 355) {
            snakeSpeed += 0.2f;
            snakeSpeedTimer = 0;
        }
        snake2SpeedTimer++;
        if (snake2SpeedTimer >= 355) {
            snake2Speed += 0.2f;
            snake2SpeedTimer = 0;
        }
        if (SeanCheckCollision(snakeSegment)) {
        player.tempx = 50;
        player.tempy = 250;
        deathcounter++;
        g.game_state = 4;
        clearAllProjectiles();
        initialized2 = false;
        powerUpActive = false;
        snake2Speed = snake2BaseSpeed;
        snake2SpeedTimer = 0;
        snakeSpeed = snakeBaseSpeed;
        snakeSpeedTimer = 0;
        }

        }
        for (int i = 0; i < SNAKE2_LENGTH; ++i) {
            Entity snakeSegment;
            snakeSegment.x = snake2[i].x;
            snakeSegment.y = snake2[i].y;
            snakeSegment.width = 10;
            snakeSegment.height = 10;
        
        if (SeanCheckCollision(snakeSegment)) {
                player.tempx = 50;
                player.tempy = 250;
                deathcounter++;
                g.game_state = 4;
                clearAllProjectiles();
                initialized2 = false;
                powerUpActive = false;
                snake2Speed = snake2BaseSpeed;
                snake2SpeedTimer = 0;
                snakeSpeed = snakeBaseSpeed;
                snakeSpeedTimer = 0;
            }
        }
        float dx = player.pos[0] - snake[0].x;
        float dy = player.pos[1] - snake[0].y;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist > 1.0f) {
            dx = dx / dist * snakeSpeed;
            dy = dy / dist * snakeSpeed;
            snake[0].x += dx;
            snake[0].y += dy;
        }
        for (int i = SNAKE_LENGTH - 1; i > 0; --i) {
            snake[i].x = snake[i - 1].x;
            snake[i].y = snake[i - 1].y;
        }
        float dx2 = player.pos[0] - snake2[0].x;
        float dy2 = player.pos[1] - snake2[0].y;
        float dist2 = sqrt(dx2*dx2 + dy2*dy2);
        if (dist2 > 1.0f) {
            dx2 = dx2 / dist2 * snake2Speed;
            dy2 = dy2 / dist2 * snake2Speed;
            snake2[0].x += dx2;
            snake2[0].y += dy2;
        }
        for (int i = SNAKE2_LENGTH - 1; i > 0; --i) {
            snake2[i].x = snake2[i - 1].x;
            snake2[i].y = snake2[i - 1].y;
        }

        dasonDrawWalls(sean_grid, SEAN_GRID_SIZE);
        dasonPhysics(SEAN_GRID_SIZE, 0, 0, NULL);
    }
}
void seanrungame3()
{

}
        
          

    
    

    


 
