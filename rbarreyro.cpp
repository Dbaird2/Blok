// Name: Russell Barreyro
// Date: Spring 2025

// -----------------------------------------------------------
// Includes and Definitions
// -----------------------------------------------------------
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "fonts.h"
#include "dbairdheader.h"
#include "rbarreyroheader.h"
#include "caroline.h"
#include "cmorenoyanesheader.h"
#include "Global.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#define RUSS_GRID_SIZE 20
const int POWERUP_SIZE = 10;
const int NUM_ENEMIES = 4;
const int NUM_COINS   = 10;
const float GOAL_X    = 800.0f;
const float GOAL_Y    = 250.0f;

// -----------------------------------------------------------
// Global Game State
// -----------------------------------------------------------
std::vector<PowerUp> powerUps;
static float gAnimTime = 0.0f;
static RB_Entity Russ_goal = {GOAL_X, GOAL_Y, 25, 25, 0.0f, true};
static std::vector<Enemy> enemies;
static std::vector<Coin> coins;
static int collectedCoins = 0;
int Deathcounter = 0;

// -----------------------------------------------------------
// Enum and Structs for PowerUps
// -----------------------------------------------------------
enum PowerUpType { SPEED };

struct PowerUp {
    float x, y;
    PowerUpType type;
    bool active;
    bool collected;

    PowerUp(float x_, float y_, PowerUpType t, bool a)
        : x(x_), y(y_), type(t), active(a), collected(false) {}
};

// -----------------------------------------------------------
// Initialization and End Credit
// -----------------------------------------------------------
void rjEndCredit() {
    Rect title = {0};
    title.bot = 82;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author: Russell Barreyro");
}

void DrawDeathCounter(int DeathCount) {
    Rect r;
    r.bot = 50;
    r.left = 100;
    r.center = 0;
    ggprint8b(&r, 16, 0xff0000ff, "Deaths:");
    ggprint8b(&r, 16, 0xff0000ff, "%d", DeathCount);
}

// -----------------------------------------------------------
// Level Setup: enemies, coins, power-ups
// -----------------------------------------------------------
void RB_InitializeLevel() {
    collectedCoins = 0;
    enemies.clear();
    coins.clear();
    powerUps.clear();
    DrawDeathCounter(Deathcounter);

    enemies = {
        {{150, 150, 20, 20, 2.0f, true}, CHASER, 0, 0, 0, 160.0f},
        {{650, 120, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 190.0f},
        {{400, 250, 15, 15, 0.0f, true}, ORBITER, 400.0f, 250.0f, 0.0f, 0.0f},
        {{700, 400, 15, 15, 0.0f, true}, ORBITER, 700.0f, 400.0f, 3.14f, 0.0f},
        {{600, 550, 25, 25, 2.0f, true}, CHASER, 0, 0, 0, 150.0f},
        {{200, 500, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 180.0f}
    };

    powerUps.push_back(PowerUp(220, 280, SPEED, false));
    powerUps.push_back(PowerUp(120, 180, SPEED, false));
    powerUps.push_back(PowerUp(320, 400, SPEED, false));

    float mazeCoins[NUM_COINS][2] = {
        {100, 100}, {180, 220}, {260, 340}, {340, 220},
        {420, 100}, {500, 220}, {580, 340}, {660, 220},
        {150, 450}, {400, 600}
    };

    for (int i = 0; i < NUM_COINS; ++i) {
        coins.push_back({mazeCoins[i][0], mazeCoins[i][1], false,
                mazeCoins[i][1], 20.0f, 2.5f + i * 0.15f, 0.0f, 10});
    }
}

// -----------------------------------------------------------
// Drawing Utilities
// -----------------------------------------------------------
void RB_DrawColoredRect(float x, float y, float w, float h,
        float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + w, y);
    glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();
}

// -----------------------------------------------------------
// Maze Layout
// -----------------------------------------------------------
Grid russ_grid[RUSS_GRID_SIZE];
int russ_height[RUSS_GRID_SIZE] = {5, 5, 100, 100, 100, 100, 100, 100, 100, 100, 50, 100, 5, 100, 100, 50, 100, 100, 5, 5};
int russ_width[RUSS_GRID_SIZE]  = {100, 100, 5, 5, 5, 5, 5, 5, 5, 5, 50, 5, 100, 5, 5, 50, 5, 5, 100, 100};
int russxpos[RUSS_GRID_SIZE]    = {0, 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900};
int russypos[RUSS_GRID_SIZE]    = {0, 100, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950};

void russLevel() {
    dasonLoadStruct(russ_grid, russ_height, russ_width, russxpos, russypos, RUSS_GRID_SIZE);
    dasonDrawWalls(russ_grid, RUSS_GRID_SIZE);
    dasonPhysics(RUSS_GRID_SIZE, 0, 0, NULL);
}

// -----------------------------------------------------------
// Collision Detection
// -----------------------------------------------------------
bool RB_CheckEntityCollision(const RB_Entity& a, const RB_Entity& b) {
    bool collision = (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    if (collision) Deathcounter++;
    return collision;
}

// -----------------------------------------------------------
// Enemy Behavior and Rendering
// -----------------------------------------------------------
void RB_UpdateEnemies() {
    for (auto& e : enemies) {
        if (!e.base.isActive) continue;
        if (e.type == CHASER) {
            float dx = player.pos[0] - e.base.x;
            float dy = player.pos[1] - e.base.y;
            float dist = sqrtf(dx*dx + dy*dy);
            if (dist < e.detectionRadius) {
                e.base.x += (dx/dist) * e.base.speed;
                e.base.y += (dy/dist) * e.base.speed;
            }
        } else {
            e.angle += 0.02f;
            e.base.x = e.centerX + cosf(e.angle) * 100.0f;
            e.base.y = e.centerY + sinf(e.angle) * 75.0f;
        }
    }
}

void RB_DrawEnemies() {
    for (auto& e : enemies) {
        if (!e.base.isActive) continue;
        float r = (e.type == CHASER) ? 1.0f : 0.5f;
        float g = 0.0f;
        float b = (e.type == CHASER) ? 0.0f : 0.5f;
        RB_DrawColoredRect(e.base.x, e.base.y, e.base.width, e.base.height, r, g, b);
    }
}

// -----------------------------------------------------------
// Coin Behavior and Rendering
// -----------------------------------------------------------
void RB_UpdateCoins(std::vector<Coin>& coins) {
    for (auto& c : coins) {
        if (c.collected) continue;
        c.y = c.baseY + sinf(gAnimTime * c.frequency) * c.amplitude;
        c.angle += 0.05f;
    }
}

void RB_DrawCoins(std::vector<Coin>& coins) {
    for (auto& c : coins) {
        if (c.collected) continue;
        glPushMatrix();
        glTranslatef(c.x, c.y, 0.0f);
        glRotatef(c.angle * 180.0f / M_PI, 0.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 0.85f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for (int a = 0; a < 360; a += 30) {
            float rad = a * M_PI / 180.0f;
            glVertex2f(cosf(rad) * 10.0f, sinf(rad) * 10.0f);
        }
        glEnd();
        glPopMatrix();
    }
}

int RB_CheckCoinCollection(std::vector<Coin>& coins, int collectedCoins) {
    RB_Entity pb = {
        player.pos[0] - player.width / 2.0f,
        player.pos[1] - player.height / 2.0f,
        (float)player.width,
        (float)player.height,
        0.0f, true
    };
    for (auto& c : coins) {
        if (c.collected) continue;
        RB_Entity cb = {c.x - 10.0f, c.y - 10.0f, 20.0f, 20.0f, 0.0f, true};
        if (RB_CheckEntityCollision(pb, cb)) {
            c.collected = true;
            collectedCoins += c.value;
        }
    }
    return collectedCoins;
}

// -----------------------------------------------------------
// Power-Up Handling
// -----------------------------------------------------------
void UpdatePowerUps() {
    RB_Entity pb = {
        player.pos[0] - player.width/2.0f,
        player.pos[1] - player.height/2.0f,
        (float)player.width,
        (float)player.height,
        0.0f, true
    };
    for (auto &p : powerUps) {
        if (p.collected) continue;
        RB_Entity pu = { p.x-10, p.y-10, 20.0f, 20.0f, 0.0f, true };
        if (RB_CheckEntityCollision(pb, pu)) {
            p.collected = true;
            if (p.type == SPEED) {
                player.speed *= 1.5f;
            }
        }
    }
}

void DrawPowerUps() {
    for (auto& p : powerUps) {
        if (!p.active) continue;
        RB_DrawColoredRect(p.x, p.y, POWERUP_SIZE, POWERUP_SIZE,
                0.0f, 0.6f, 1.0f);
    }
}

// -----------------------------------------------------------
// Main Level Logic Entry Point
// -----------------------------------------------------------
void rbarreyroRunGame() {
    if (g.game_state != 5) return;
    static bool init = false;
    if (!init) {
        player.tempx = 50;
        player.tempy = 250;
        RB_InitializeLevel();
        init = true;
    }

    gAnimTime += 0.016f;

    RB_UpdateEnemies();
    RB_UpdateCoins(coins);
    UpdatePowerUps();
    DrawPowerUps();
    collectedCoins = RB_CheckCoinCollection(coins, collectedCoins);
    russLevel();

    RB_DrawColoredRect(Russ_goal.x, Russ_goal.y, Russ_goal.width, Russ_goal.height, 0.0f, 1.0f, 0.0f);
    RB_DrawEnemies();
    RB_DrawCoins(coins);

    Rect r = {0}; r.bot = g.yres - 30; r.left = 20; r.center = 0;
    ggprint8b(&r, 16, 0x00FF00, "Score: %d", collectedCoins);

    RB_Entity pb = {
        player.pos[0] - player.width / 2.0f,
        player.pos[1] - player.height / 2.0f,
        (float)player.width,
        (float)player.height,
        0.0f, true
    };

    for (auto& e : enemies) {
        if (e.base.isActive && RB_CheckEntityCollision(pb, e.base)) {
            player.tempx = 50;
            player.tempy = 250;
        }
    }

    if (collectedCoins >= NUM_COINS * 10 &&
<<<<<<< HEAD
        RB_CheckEntityCollision(pb, Russ_goal)) {
        if (g.amt_of_stars[1] < 1)
            g.amt_of_stars[1]++;

=======
            RB_CheckEntityCollision(pb, Russ_goal)) {
>>>>>>> f3fc839 (game state 10)
        g.game_state = 9;
    }
}

// -----------------------------------------------------------
// Quit Handling
// -----------------------------------------------------------
void check_quit(XEvent *e) {
    if (e->type == KeyPress) {
        KeySym key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_q && g.game_state == 5) {
            g.game_state = 2;
        }
    }
}

// -----------------------------------------------------------
// EVEL 10 GLOBAL DEFINITIONS 
// -----------------------------------------------------------
std::vector<L10_Projectile> projectiles10;
std::vector<RB_Entity>      triangleEnemies10;
std::vector<L10_PowerUp>    powerUps10;
std::vector<Coin>           coins10;
RB_Entity                   portalA10, portalB10;
std::vector<L10_Laser>      lasers10;
RB_Entity                   growingBox10;
float                       growFactor10;
bool                        growing10;
RB_Entity                   goal10;

// -----------------------------------------------------------
// LEVEL 10 IMPLEMENTATION 
// -----------------------------------------------------------
void InitLevel10() 
{
    projectiles10.clear();
    triangleEnemies10.clear();
    powerUps10.clear();
    coins10.clear();
    lasers10.clear();
    growFactor10 = 1.0f;
    growing10   = true;

    // Player spawn
    player.tempx = 60; player.tempy = 60;

    // Portals
    portalA10 = {100,100,20,20,0.0f,true};
    portalB10 = {700,400,20,20,0.0f,true};

    // Goal
    goal10 = {820,250,25,25,0.0f,true};

    // Coins (reuse your Coin struct)
    float pts[10][2] = {
        {120,120},{240,200},{360,280},{480,360},{600,440},
        {720,360},{840,280},{360,120},{600,120},{120,360}
    };
    for (int i = 0; i < 10; ++i) {
        coins10.push_back({
                pts[i][0], pts[i][1],
                false,        // collected
                pts[i][1],    // baseY
                15.0f,        // amplitude
                3.0f + i*0.2f,// frequency
                0.0f,         // angle
                10            // value
                });
    }

    // Shooters
    triangleEnemies10.push_back({
            float(g.xres - 40),
            float(g.yres - 40),
            20.0f, 20.0f, 0.0f, true
            });
    triangleEnemies10.push_back({
            float(g.xres - 40),
            40.0f,
            20.0f, 20.0f, 0.0f, true
            });
    // Lasers
    lasers10.push_back({200,150,300,0.0f, 1.0f, 0.0f, true});
    lasers10.push_back({500,350,250,3.14159f/2,0.75f,0.5f, true});
    lasers10.push_back({700,200,200,0.0f, 1.25f,1.0f, true});

    // Growing box
    growingBox10 = {600,150,30,30, 0.0f, true};
}

void UpdateLevel10(double dt) {
    // 1) Toggle lasers
    for (auto &L : lasers10) {
        L.phase += dt;
        if (L.phase > L.toggleRate) {
            L.phase -= L.toggleRate;
            L.on = !L.on;
        }
    }
    // 2) Growing box
    growFactor10 += (growing10 ? 1 : -1) * dt * 0.5f;
    if (growFactor10 > 2.0f || growFactor10 < 1.0f)
        growing10 = !growing10;

    // 3) Shooters fire every second
    static double fireTimer = 0.0;
    fireTimer += dt;
    if (fireTimer >= 1.0) {
        for (auto &e : triangleEnemies10) {
            float dx = player.pos[0] - e.x;
            float dy = player.pos[1] - e.y;
            float d  = sqrtf(dx*dx + dy*dy);
            projectiles10.push_back({
                    e.x, e.y, 5,5,
                    dx/d, dy/d,
                    200.0f, true
                    });
        }
        fireTimer = 0.0;
    }

    // 4) Move projectiles
    for (auto &p : projectiles10) {
        if (!p.active) continue;
        p.x += p.dx * p.speed * dt;
        p.y += p.dy * p.speed * dt;
        if (p.x < 0 || p.x > g.xres ||
                p.y < 0 || p.y > g.yres)
            p.active = false;
    }

}

void DrawLevel10() {
    dasonDrawWalls(level10Grid, LEVEL10_GRID_SIZE);

    // Portals
    RB_DrawColoredRect(portalA10.x, portalA10.y,
            portalA10.width, portalA10.height,
            0.5f, 0.0f, 1.0f);
    RB_DrawColoredRect(portalB10.x, portalB10.y,
            portalB10.width, portalB10.height,
            0.5f, 0.0f, 1.0f);

    // Coins
    for (auto &c : coins10)
        if (!c.collected)
            RB_DrawColoredRect(c.x, c.y, 20, 20,
                    1.0f, 0.85f, 0.0f);

    // Power-ups
    for (auto &p : powerUps10)
        if (p.active && !p.collected)
            RB_DrawColoredRect(p.x, p.y, 15, 15,
                    0.0f, 1.0f, 0.0f);

    // Shooters
    for (auto &e : triangleEnemies10)
        RB_DrawColoredRect(e.x, e.y, e.width, e.height,
                0.0f, 0.0f, 1.0f);

    // Lasers
    glLineWidth(4.0f);
    for (auto &L : lasers10) if (L.on) {
        glColor3f(0.0f, 1.0f, 1.0f);
        float dx = cosf(L.angle)*L.length/2;
        float dy = sinf(L.angle)*L.length/2;
        glBegin(GL_LINES);
        glVertex2f(L.x-dx, L.y-dy);
        glVertex2f(L.x+dx, L.y+dy);
        glEnd();
    }

    // Growing box
    float s = growingBox10.width * growFactor10;
    RB_DrawColoredRect(growingBox10.x, growingBox10.y, s, s,
            0.4f, 0.9f, 0.2f);

    // Projectiles
    for (auto &p : projectiles10)
        if (p.active)
            RB_DrawColoredRect(p.x, p.y, p.w, p.h,
                    1.0f, 1.0f, 0.0f);

    // Goal
    RB_DrawColoredRect(goal10.x, goal10.y,
            goal10.width, goal10.height,
            0.0f, 1.0f, 0.0f);
}

Grid      level10Grid[1];           
const int LEVEL10_GRID_SIZE = 0;
