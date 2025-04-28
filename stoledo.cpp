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
#include <chrono>

//Music
/* ALuint buffer, source;
void initAudio(const char* filename) {
    alutInit(0, NULL); // Initialize OpenAL
    buffer = alutCreateBufferFromFile(filename); // Load audio file

    if (buffer == AL_NONE) {
        cerr << "Error loading sound file: " << alutGetErrorString(alutGetError()) << endl;
        return;
    }

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE); // Enable looping
    alSourcePlay(source);
} 

// Function to Cleanup OpenAL
void cleanupAudio() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alutExit();
} */
//Wall
#define SEAN_GRID_SIZE 4
Grid sean_grid[SEAN_GRID_SIZE];

int sean_height[SEAN_GRID_SIZE] = {
    5,
    5,
    g.yres / 2,
    g.yres / 2
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

int seanypos[SEAN_GRID_SIZE] = {
    g.yres - 5,
    5,
    g.yres / 2,
    g.yres / 2
};


void seanLevel()
{
    dasonLoadStruct(sean_grid, sean_height, sean_width, seanxpos, seanypos, SEAN_GRID_SIZE);
}



//Margin
float edge = 100.0f;

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
Entity triangleEnemies[2] = {
    {
        static_cast<float>(g.xres) - edge - 20.0f, // Top-right
        static_cast<float>(g.yres) - edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    },
    {
        static_cast<float>(g.xres) - edge - 20.0f, // Bottom-right
        edge,
        20.0f, 20.0f,
        0.0f, 0.0f
    }
};

void UpdateProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (projectiles[i].active) {
            projectiles[i].x += projectiles[i].dx * projectiles[i].speed;
            projectiles[i].y += projectiles[i].dy * projectiles[i].speed;

            // Check bounds
            if (projectiles[i].x < 0 || projectiles[i].x > g.xres ||
                projectiles[i].y < 0 || projectiles[i].y > g.yres)
                projectiles[i].active = false;
        }
    }
}

bool CheckProjectileCollision(Projectile &p) {
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

void DrawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (projectiles[i].active) {
            SeanDrawRect(projectiles[i].x, projectiles[i].y,
                         projectiles[i].width, projectiles[i].height,
                         0.0f, .0f, 1.0f); // Yellow projectile
        }
    }
}


void FireProjectileAtPlayer(Entity &enemy) {
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
                5.0f,  // speed
                5.0f, 5.0f,
                true
            };
            break;
        }
    }
}


void DrawTriangleEnemy(Entity &e) {
    /*
    printf("TRIANGLE: (%.1f, %.1f) -> (%.1f, %.1f) -> (%.1f, %.1f)\n",
        e.x, e.y,
        e.x + e.width, e.y,
        e.x + e.width / 2, e.y + e.height);
        */
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

//Entity
/*struct Entity {
    float x, y;
    float width, height;  
    float speed;
};*/
//Goal
Entity goal = {800, 250, 25, 25, 0, 0};


//Enemies
Entity enemies[30] = {
    {300, 200, 20, 20, 30.0, 1},
    {500, 400, 20, 20, 30.0, -1},
    {300, 200, 20, 20, 30.0, 1},
    {500, 400, 20, 20, 30.0, -1}
};


//Enemy Direction

//Draw Enemy Rectangle
void SeanDrawRect(float x, float y, float width, float height,
                  float r, float g, float b) {
   /* printf("RECT: x=%.1f y=%.1f w=%.1f h=%.1f color=(%.1f, %.1f, %.1f)\n",
           x, y, width, height, r, g, b);
           */
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);                   
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
//Enemy Movement
void SeanEnemiesVertical(int start, int end, int yBoundary, int margin, Entity enemies[]) {
    for (int i = start; i < end; i++) {
        enemies[i].y += enemies[i].dir * enemies[i].speed;
        if (enemies[i].y <= margin || enemies[i].y >= yBoundary)
            enemies[i].dir *= -1;
    }
}

void SeanEnemiesHorizontal(int start, int end, int xBoundary, int margin, Entity enemies[]) {
    for (int i = start; i < end; i++) {
        enemies[i].x += enemies[i].dir * enemies[i].speed;
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
void drawDeathCounter(int deathCount) {
    Rect r;
    r.bot = 50;        
    r.left = 100;       
    r.center = 0;
    ggprint8b(&r, 16, 0xff0000ff, "Deaths:");
    ggprint8b(&r, 16, 0xff0000ff, "%d", deathCount);
}

int deathcounter = 0;
int triangleShootingCooldownFrames = 0; // in frames (3 seconds = 180 at 60 FPS)
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
        }
        // Draw Player Box
        drawDeathCounter(deathcounter);
        drawPlayerBox(0);
        SeanEnemiesVertical(0, 2, g.yres, edge, enemies);
        SeanEnemiesHorizontal(2, 4, g.xres, edge, enemies);
        UpdateProjectiles();
        DrawProjectiles();
        for (int i = 0; i < 2; ++i) {
        DrawTriangleEnemy(triangleEnemies[i]);
        SeanDrawRect(goal.x, goal.y, goal.width, goal.height, 0, 1, 0);
        for (int i = 0; i < 4; i++)
        SeanDrawRect(enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height,1, 0, 0);
        }
        dasonDrawWalls(sean_grid, SEAN_GRID_SIZE);
        dasonPhysics(SEAN_GRID_SIZE, 0, 0, NULL);

        


    // Fire projectile every few frames
    static int fireCooldown = 0;
    if (triangleShootingCooldownFrames <= 0) {
    if (++fireCooldown > 15) {
        for (int i = 0; i < 2; ++i) {
            FireProjectileAtPlayer(triangleEnemies[i]);
        }
        fireCooldown = 0;
        }
        }
        }

        for (int i = 0; i < 4; i++) {
        if (SeanCheckCollision(enemies[i])) {
        player.tempx = 50;
        player.tempy = 250; 
        deathcounter++;
        triangleShootingCooldownFrames = 180; // RESET COOLDOWN
        }

        }
        
         if (SeanCheckCollision(goal)) {
            cout << "You Win!" << endl;
            g.game_state = 0 ;
         }
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
        if (projectiles[i].active && CheckProjectileCollision(projectiles[i])) {
        projectiles[i].active = false;
        player.tempx = 50;
        player.tempy = 250;
        deathcounter++;
        triangleShootingCooldownFrames = 180; // RESET COOLDOWN
        }
        if (triangleShootingCooldownFrames > 0) {
        triangleShootingCooldownFrames--;
        }

        }




        
}

        
          

    
    

    


 
