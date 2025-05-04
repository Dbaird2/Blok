// Name: Russell Barreyro
// Date: Spring 2025

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
std::vector<PowerUp> powerUps;

static float gAnimTime = 0.0f;

static RB_Entity Russ_goal = {GOAL_X, GOAL_Y, 25, 25, 0.0f, true};
static std::vector<Enemy> enemies;
static std::vector<Coin> coins;
static int collectedCoins = 0;

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

int Deathcounter = 0;

enum PowerUpType {SPEED};

struct PowerUp {
    float x, y;
    PowerUpType type;
    bool active;
    bool collected;

    PowerUp(float x_, float y_, PowerUpType t, bool a)
        : x(x_), y(y_), type(t), active(a) {}
};

void RB_InitializeLevel() {
    collectedCoins = 0;
    enemies.clear();
    coins.clear();
    DrawDeathCounter(Deathcounter);

    enemies = {
        {{150, 150, 20, 20, 2.0f, true}, CHASER, 0, 0, 0, 160.0f},
        {{650, 120, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 190.0f},
        {{400, 250, 15, 15, 0.0f, true}, ORBITER, 400.0f, 250.0f, 0.0f, 0.0f},
        {{700, 400, 15, 15, 0.0f, true}, ORBITER, 700.0f, 400.0f, 3.14f, 0.0f},
        {{600, 550, 25, 25, 2.0f, true}, CHASER, 0, 0, 0, 150.0f},
        {{200, 500, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 180.0f}
    };

  powerUps.push_back(PowerUp(220, 280, static_cast<PowerUpType>(0), false));
  powerUps.push_back(PowerUp(120, 180, static_cast<PowerUpType>(0), false));
  powerUps.push_back(PowerUp(320, 400, static_cast<PowerUpType>(0), false));

    // coin placement
    float mazeCoins[NUM_COINS][2] = {
        {100, 100}, {180, 220}, {260, 340}, {340, 220},
        {420, 100}, {500, 220}, {580, 340}, {660, 220},
        {150, 450}, {400, 600}
    };    

    for (int i = 0; i < NUM_COINS; ++i) {
        float x = mazeCoins[i][0];
        float y = mazeCoins[i][1];
        coins.push_back({x, y, false, y, 20.0f, 2.5f + i * 0.15f, 0.0f, 10});
    }
}

void RB_DrawColoredRect(float x, float y, float w, float h,
        float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

Grid russ_grid[RUSS_GRID_SIZE];

int russ_height[RUSS_GRID_SIZE] = {
    5, 5, 100, 100, 100, 100, 100, 100, 100, 100, 50, 100, 5, 100, 100, 50, 100, 100, 5, 5
};

int russ_width[RUSS_GRID_SIZE] = {
    100, 100, 5, 5, 5, 5, 5, 5, 5, 5, 50, 5, 100, 5, 5, 50, 5, 5, 100, 100
};

int russxpos[RUSS_GRID_SIZE] = {
    0, 0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900
};

int russypos[RUSS_GRID_SIZE] = {
    0, 100, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950
};

void russLevel()
{
    dasonLoadStruct(russ_grid, russ_height, russ_width, 
            russxpos, russypos, RUSS_GRID_SIZE);
    dasonDrawWalls(russ_grid, RUSS_GRID_SIZE);
    dasonPhysics(RUSS_GRID_SIZE, 0, 0, NULL);
}

bool RB_CheckEntityCollision(const RB_Entity& a,
        const RB_Entity& b) {

    bool collision = (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
    if (collision) Deathcounter++;
    return collision;
}


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
        if (e.type == CHASER) {
            RB_DrawColoredRect(e.base.x, e.base.y,
                    e.base.width, e.base.height,
                    1.0f, 0.0f, 0.0f);
        } else {
            RB_DrawColoredRect(e.base.x, e.base.y,
                    e.base.width, e.base.height,
                    0.5f, 0.0f, 0.5f);
        }
    }
}

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
        player.pos[0] - static_cast<float>(player.width) / 2.0f,
        player.pos[1] - static_cast<float>(player.height) / 2.0f,
        static_cast<float>(player.width),
        static_cast<float>(player.height),
        0.0f,
        true
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
        // collision bbox
        RB_Entity pu = { p.x-10, p.y-10, 20.0f, 20.0f, 0.0f, true };
        if (RB_CheckEntityCollision(pb, pu)) {
            p.collected = true;
            if (p.type == 0) {
                player.speed *= 1.5f;     
            } 
        }
    }
}

// ----------------------------------------------------------------------------
// 3) Draw routine
void DrawPowerUps() {
    for (auto& p : powerUps) {
        if (!p.active) continue;
        RB_DrawColoredRect(p.x, p.y, POWERUP_SIZE, POWERUP_SIZE,
            p.type == SPEED ? 0.0f : 0.2f,
            p.type == SPEED ? 0.6f : 0.8f,
            1.0f);
    }
}


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

    RB_DrawColoredRect(Russ_goal.x, Russ_goal.y,
            Russ_goal.width, Russ_goal.height,
            0.0f, 1.0f, 0.0f);

    RB_DrawEnemies();
    RB_DrawCoins(coins);
    powerUps.clear();

    Rect r = {0}; r.bot = g.yres - 30; r.left = 20; r.center = 0;
    ggprint8b(&r, 16, 0x00FF00, "Score: %d", collectedCoins);

    RB_Entity pb = {
        player.pos[0] - static_cast<float>(player.width) / 2.0f,
        player.pos[1] - static_cast<float>(player.height) / 2.0f,
        static_cast<float>(player.width),
        static_cast<float>(player.height),
        0.0f,
        true
    };
    for (auto& e : enemies) {
        if (e.base.isActive && RB_CheckEntityCollision(pb, e.base)) {
            player.tempx = 50;
            player.tempy = 250;
        }
    }
    if (collectedCoins >= NUM_COINS * 10 &&
            RB_CheckEntityCollision(pb, Russ_goal)) {
        //std::cout << "Victory! Score: " << collectedCoins << std::endl;
        g.game_state = 9;
    }
}

void check_quit(XEvent *e){
    if (e->type == KeyPress) {
        KeySym key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_q && g.game_state == 5) {
            g.game_state = 2;
        }
    }
}

