// Name: Russell Barreyro
// Date: Spring 2025

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "fonts.h"
#include "dbairdheader.h"
#include "rbarreyroheader.h"
#include "Global.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

// Constants & globals
const int NUM_ENEMIES = 4;
const int NUM_COINS   = 8;
const float GOAL_X    = 800.0f;
const float GOAL_Y    = 250.0f;

static float gAnimTime = 0.0f; // global animation timer

// Make globals static to avoid multiple-definition linker errors
static RB_Entity Russ_goal = {GOAL_X, GOAL_Y, 25, 25, 0.0f, true};
static std::vector<Enemy> enemies;
static std::vector<Coin> coins;
static int collectedCoins = 0;

// End Credit
void rjEndCredit() {
    Rect title = {0};
    title.bot = 82;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author: Russell Barreyro");
}

// Initialize Level: spawn enemies & coins
void RB_InitializeLevel() {
    collectedCoins = 0;
    enemies.clear();
    coins.clear();

    // Spawn enemies with varied behaviors
    enemies = {
        {{200, 200, 20, 20, 2.0f, true}, CHASER, 0,0,0, 150.0f},
        {{600, 100, 20, 20, 2.5f, true}, CHASER, 0,0,0, 200.0f},
        {{400,300,15,15,0.0f,true}, ORBITER, 400.0f,300.0f, 0.0f, 0.0f},
        {{700,350,15,15,0.0f,true}, ORBITER, 700.0f,350.0f, 3.14f, 0.0f}
    };

    for (int i = 0; i < NUM_COINS; ++i) {
        float theta = (2.0f * M_PI / NUM_COINS) * i;
        float cx = 400.0f + cosf(theta) * 200.0f;
        float cy = 300.0f + sinf(theta) * 150.0f;
        coins.push_back({cx, cy, false, cy, 20.0f, 2.0f + i*0.2f, theta, 10});
    }
}

// Draw utility
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

// Collision detection
bool RB_CheckEntityCollision(const RB_Entity& a,
                             const RB_Entity& b) {
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

// Update enemy behaviors
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

// Draw enemies
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

// Update coins (animation)
void RB_UpdateCoins(vector<Coin> coins) {
    for (auto& c : coins) {
        if (c.collected) continue;
        c.y = c.baseY + sinf(gAnimTime * c.frequency) * c.amplitude;
        c.angle += 0.05f;
    }
}

// Draw coins with rotation
void RB_DrawCoins(vector<Coin> coins) {
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

// Check coin collection
void RB_CheckCoinCollection(vector<Coin> coins) {
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
}

// Main loop
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
    RB_CheckCoinCollection(coins);

    // draw goal
    RB_DrawColoredRect(
        Russ_goal.x, Russ_goal.y,
        Russ_goal.width, Russ_goal.height,
        0.0f, 1.0f, 0.0f
    );

    RB_DrawEnemies();
    RB_DrawCoins(coins);

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
        std::cout << "Victory! Score: " << collectedCoins << std::endl;
        g.game_state = 2;
    }
}
