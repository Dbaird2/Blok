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

// Global Game State
std::vector<PowerUp> powerUps;
float gAnimTime = 0.0f;
RB_Entity Russ_goal = {GOAL_X, GOAL_Y, 25, 25, 0.0f, true};
std::vector<Enemy> rb_enemies;
std::vector<Coin> coins;
int collectedCoins = 0;
int Deathcounter = 0;

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
    rb_enemies.clear();
    coins.clear();
    powerUps.clear();
    DrawDeathCounter(Deathcounter);

    // Initialize walls using the Wall class from Global.h
    // Outer border
    walls[0] = Wall(); walls[0].width = g.xres; walls[0].height = 20; walls[0].pos[0] = 0; walls[0].pos[1] = 0;                    // Top
    walls[1] = Wall(); walls[1].width = g.xres; walls[1].height = 20; walls[1].pos[0] = 0; walls[1].pos[1] = g.yres - 20;         // Bottom
    walls[2] = Wall(); walls[2].width = 20; walls[2].height = g.yres; walls[2].pos[0] = 0; walls[2].pos[1] = 0;                   // Left
    walls[3] = Wall(); walls[3].width = 20; walls[3].height = g.yres; walls[3].pos[0] = g.xres - 20; walls[3].pos[1] = 0;         // Right

    // Maze walls
    walls[4] = Wall(); walls[4].width = 200; walls[4].height = 20; walls[4].pos[0] = 100; walls[4].pos[1] = 100;                  // Horizontal wall 1
    walls[5] = Wall(); walls[5].width = 200; walls[5].height = 20; walls[5].pos[0] = 400; walls[5].pos[1] = 200;                  // Horizontal wall 2
    walls[6] = Wall(); walls[6].width = 200; walls[6].height = 20; walls[6].pos[0] = 200; walls[6].pos[1] = 300;                  // Horizontal wall 3
    walls[7] = Wall(); walls[7].width = 200; walls[7].height = 20; walls[7].pos[0] = 500; walls[7].pos[1] = 400;                  // Horizontal wall 4
    walls[8] = Wall(); walls[8].width = 200; walls[8].height = 20; walls[8].pos[0] = 100; walls[8].pos[1] = 500;                  // Horizontal wall 5

    walls[9] = Wall(); walls[9].width = 20; walls[9].height = 200; walls[9].pos[0] = 100; walls[9].pos[1] = 100;                  // Vertical wall 1
    walls[10] = Wall(); walls[10].width = 20; walls[10].height = 200; walls[10].pos[0] = 300; walls[10].pos[1] = 200;             // Vertical wall 2
    walls[11] = Wall(); walls[11].width = 20; walls[11].height = 200; walls[11].pos[0] = 500; walls[11].pos[1] = 300;             // Vertical wall 3
    walls[12] = Wall(); walls[12].width = 20; walls[12].height = 200; walls[12].pos[0] = 200; walls[12].pos[1] = 400;             // Vertical wall 4
    walls[13] = Wall(); walls[13].width = 20; walls[13].height = 200; walls[13].pos[0] = 400; walls[13].pos[1] = 500;             // Vertical wall 5

    // Initialize more enemies
    rb_enemies = {
        {{150, 150, 20, 20, 2.0f, true}, CHASER, 0, 0, 0, 160.0f},
        {{650, 120, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 190.0f},
        {{400, 250, 15, 15, 0.0f, true}, ORBITER, 400.0f, 250.0f, 0.0f, 0.0f},
        {{700, 400, 15, 15, 0.0f, true}, ORBITER, 700.0f, 400.0f, 3.14f, 0.0f},
        {{600, 550, 25, 25, 2.0f, true}, CHASER, 0, 0, 0, 150.0f},
        {{200, 500, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 180.0f},
        {{450, 350, 20, 20, 2.0f, true}, CHASER, 0, 0, 0, 170.0f},
        {{300, 200, 15, 15, 0.0f, true}, ORBITER, 300.0f, 200.0f, 1.57f, 0.0f},
        {{550, 300, 20, 20, 2.5f, true}, CHASER, 0, 0, 0, 190.0f}
    };

    // Initialize power-ups
    powerUps.push_back(PowerUp(220, 280, SPEED, true));
    powerUps.push_back(PowerUp(120, 180, SPEED, true));
    powerUps.push_back(PowerUp(320, 400, SPEED, true));

    // Initialize coins
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
    for (auto& e : rb_enemies) {
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
    for (auto& e : rb_enemies) {
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
        if (!p.active || p.collected) continue;
        RB_Entity pu = { p.x-10, p.y-10, 20.0f, 20.0f, 0.0f, true };
        if (RB_CheckEntityCollision(pb, pu)) {
            p.collected = true;
            p.active = false;  // Deactivate the power-up
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

void DrawBorder(float x, float y, float w, float h, float thickness) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(thickness);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void RB_CheckBorderCollision() {
    // Check left and right borders
    if (player.pos[0] - player.width/2.0f < 0.0f) {
        player.pos[0] = player.width/2.0f;
    }
    if (player.pos[0] + player.width/2.0f > g.xres) {
        player.pos[0] = g.xres - player.width/2.0f;
    }
    
    // Check top and bottom borders
    if (player.pos[1] - player.height/2.0f < 0.0f) {
        player.pos[1] = player.height/2.0f;
    }
    if (player.pos[1] + player.height/2.0f > g.yres) {
        player.pos[1] = g.yres - player.height/2.0f;
    }
}

void RB_CheckWallCollision() {
    RB_Entity pb = {
        player.pos[0] - player.width/2.0f,
        player.pos[1] - player.height/2.0f,
        (float)player.width,
        (float)player.height,
        0.0f, true
    };

    for (int i = 0; i < 14; i++) {  // We have 14 walls
        Wall& wall = walls[i];
        RB_Entity wb = {wall.pos[0], wall.pos[1], (float)wall.width, (float)wall.height, 0.0f, true};
        if (RB_CheckEntityCollision(pb, wb)) {
            // Calculate overlap and adjust position
            float overlapX = std::min(pb.x + pb.width - wb.x, wb.x + wb.width - pb.x);
            float overlapY = std::min(pb.y + pb.height - wb.y, wb.y + wb.height - pb.y);
            
            if (overlapX < overlapY) {
                if (pb.x < wb.x) {
                    player.pos[0] = wb.x - player.width/2.0f;
                } else {
                    player.pos[0] = wb.x + wb.width + player.width/2.0f;
                }
            } else {
                if (pb.y < wb.y) {
                    player.pos[1] = wb.y - player.height/2.0f;
                } else {
                    player.pos[1] = wb.y + wb.height + player.height/2.0f;
                }
            }
        }
    }
}

// -----------------------------------------------------------
// Physics and Collision
// -----------------------------------------------------------
void RB_Physics(int wall_size, int growing_size, int growing_enemy_check, Grid grid[]) {
    for (int i = 0; i < wall_size; i++) {
        Wall *w = &walls[i];
        Player *p = &player;

        int x_offset = p->width;
        int y_offset = p->height;
        int box_top = w->pos[1] + w->height;
        int box_bot = w->pos[1] - w->height;
        int box_left = w->pos[0] - w->width;
        int box_right = w->pos[0] + w->width;

        if ((p->pos[1] <= box_top + y_offset)
                && (p->pos[1] >= box_bot - y_offset)
                && (p->pos[0] >= box_left - x_offset) 
                && (p->pos[0] <= box_right + x_offset)) {
            if (p->pos[1] >= box_top - y_offset/4) {
                p->tempy += 2;  
                player.stop_w = 1;
            }
            else if (p->pos[1] <= box_bot+y_offset/10) {
                p->tempy -= 2;  
                player.stop_s = 1;
            }
            else if (p->pos[0] >= box_right) {
                p->tempx += 2;  
                player.stop_a = 1;
            }
            else if (p->pos[0] <= box_left) {
                p->tempx -= 2;  
                player.stop_d = 1;
            }
        } else {
            player.stop_w = 0;
            player.stop_d = 0;
            player.stop_s = 0;
            player.stop_a = 0;
        }
    }
}

// -----------------------------------------------------------
// Main Level Logic Entry Point
// -----------------------------------------------------------
void rbarreyroRunGame() {
    if (g.game_state != 10) return;
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
    
    // Draw walls and check collisions
    RB_DrawWalls();
    
    // Handle wall collisions and portals using modified physics
    RB_Physics(14, 0, 0, NULL);  // Using our modified physics function

    // Draw portals
    carolineDrawCircle(portals, NUM_PORTALS);
    isCircleCollidingWithSquare(portals, NUM_PORTALS);

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

    for (auto& e : rb_enemies) {
        if (e.base.isActive && RB_CheckEntityCollision(pb, e.base)) {
            player.tempx = 50;
            player.tempy = 250;
        }
    }

    if (collectedCoins >= NUM_COINS * 10 &&
            RB_CheckEntityCollision(pb, Russ_goal)) {
        if (g.amt_of_stars[1] < 1) {
            g.amt_of_stars[1]++;
        }
        {
            RB_CheckEntityCollision(pb, Russ_goal); 
            {
                g.game_state = 9;
            }
        }
    }

    DrawBorder(0, 0, g.xres, g.yres, 4.0f);
}

// -----------------------------------------------------------
// Quit Handling
// -----------------------------------------------------------
void check_quit(XEvent *e) {
    if (e->type == KeyPress) {
        KeySym key = XLookupKeysym(&e->xkey, 0);
        if (key == XK_q && g.game_state == 10) {
            g.game_state = 2;
        }
    }
}

void RB_DrawWalls() {
    for (int i = 0; i < 14; i++) {  // We have 14 walls
        Wall& wall = walls[i];
        RB_DrawColoredRect(wall.pos[0], wall.pos[1], wall.width, wall.height, 0.5f, 0.5f, 0.5f);
    }
}

