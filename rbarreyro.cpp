// Name: Russell Barreyro
// Date: Spring 2025
// Enhanced Game Level Implementation

#include <iostream>
#include <vector>
#include <cmath>
using namespace std; 
#include "fonts.h"
#include "dbairdheader.h"
#include "rbarreyroheader.h"
#include "Global.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

//End Credit
void rjEndCredit (void)
{
    Rect title;
    title.bot = 82;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 1: Russell Barreyro");
}

// Constants
const int NUM_ENEMIES = 4;
const int NUM_COINS = 5;
const float GOAL_X = 800.0f;
const float GOAL_Y = 250.0f;
const float COIN_RADIUS = 10.0f;

// Game State Variables
RB_Entity Russ_goal = {GOAL_X, GOAL_Y, 25, 25, 0, true};
std::vector<Coin> coins;
int collectedCoins = 0;

// Enemies
RB_Entity Russ_enemies[NUM_ENEMIES] = {
    {300, 200, 20, 20, 30.0, true},
    {500, 400, 20, 20, 30.0, true},
    {300, 200, 20, 20, 30.0, true},
    {500, 400, 20, 20, 30.0, true}
};

float enemyDirections[NUM_ENEMIES] = {1, -1, 1, -1};

// Initialize Level
void RB_InitializeLevel() {
    // Initialize coins
    coins.clear();
    coins = {
        {150.0f, 300.0f, false, 1.0f},
        {400.0f, 150.0f, false, -1.0f},
        {600.0f, 350.0f, false, 1.0f},
        {250.0f, 450.0f, false, -1.0f},
        {700.0f, 200.0f, false, 1.0f}
    };
    collectedCoins = 0;
}

// Draw Colored Rectangle
void RB_DrawColoredRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// Draw Coin
void RB_DrawCoin(float x, float y) {
    glColor3f(1.0f, 0.85f, 0.0f); // Gold color
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < 360; i += 15) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(x + COIN_RADIUS * cos(angle), 
                y + COIN_RADIUS * sin(angle));
    }
    glEnd();
}

// Update Moving Coins
void RB_UpdateSpecialCoins() {
    for (auto& coin : coins) {
        if (!coin.collected && rand() % 100 < 2) { // 2% chance to change direction
            coin.direction *= -1;
        }
        coin.y += coin.direction * 2.0f;
        if (coin.y <= 100 || coin.y >= g.yres - 100) {
            coin.direction *= -1;
        }
    }
}

// Check Entity Collision
bool RB_CheckEntityCollision(const RB_Entity& a, const RB_Entity& b) {
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}

// Check Coin Collection
void RB_CheckCoinCollection() {
    RB_Entity playerBounds = {
        player.pos[0] - player.width/2,
        player.pos[1] - player.height/2,
        (float)player.width,
        (float)player.height,
        0.0f,
        true
    };

    for (auto& coin : coins) {
        if (!coin.collected) {
            RB_Entity coinBounds = {coin.x - COIN_RADIUS, coin.y - COIN_RADIUS,
                COIN_RADIUS*2, COIN_RADIUS*2, 0.0f, true};

            if (RB_CheckEntityCollision(playerBounds, coinBounds)) {
                coin.collected = true;
                collectedCoins++;
                // Add sound effect here if available
            }
        }
    }
}

// Update Enemies
void RB_UpdateEnemyMovement() {
    for (int i = 0; i < NUM_ENEMIES; ++i) {
        if (i < 2) { // Vertical movers
            Russ_enemies[i].y += enemyDirections[i] * Russ_enemies[i].speed;
            if (Russ_enemies[i].y <= 100 || Russ_enemies[i].y >= g.yres - 100)
                enemyDirections[i] *= -1;
        } else { // Horizontal movers
            Russ_enemies[i].x += enemyDirections[i] * Russ_enemies[i].speed;
            if (Russ_enemies[i].x <= 100 || Russ_enemies[i].x >= g.xres - 100)
                enemyDirections[i] *= -1;
        }
    }
}

// Draw Coin Counter
void RB_DrawCoinCounter() {
    Rect r;
    r.bot = g.yres - 30;
    r.left = 20;
    r.center = 0;
    ggprint8b(&r, 16, 0x00FF00, "Coins: %d/%zu", collectedCoins, coins.size());
}

// Main Game Loop
void rbarreyroRunGame() {
    if (g.game_state == 5) {
        static bool initialized = false;
        if (!initialized) {
            player.tempx = 50;
            player.tempy = 250;
            RB_InitializeLevel();
            initialized = true;
        }

        // Update game elements
        RB_UpdateEnemyMovement();
        RB_UpdateSpecialCoins();
        RB_CheckCoinCollection();

        // Draw elements
        drawPlayerBox();

        // Draw goal
        RB_DrawColoredRect(Russ_goal.x, Russ_goal.y, Russ_goal.width, Russ_goal.height, 0, 1, 0);

        // Draw enemies
        for (int i = 0; i < NUM_ENEMIES; ++i) {
            if (Russ_enemies[i].isActive) {
                RB_DrawColoredRect(Russ_enemies[i].x, Russ_enemies[i].y, 
                        Russ_enemies[i].width, Russ_enemies[i].height, 1, 0, 0);
            }
        }

        // Draw coins
        for (const auto& coin : coins) {
            if (!coin.collected) {
                RB_DrawCoin(coin.x, coin.y);
            }
        }

        // Draw UI
        RB_DrawCoinCounter();

        // Check collisions
        RB_Entity playerBounds = {
            player.pos[0] - static_cast<float>(player.width)/2.0f,
            player.pos[1] - static_cast<float>(player.height)/2.0f,
            static_cast<float>(player.width),
            static_cast<float>(player.height),
            0.0f,
            true
        };

        // Enemy collision check
        for (const auto& enemy : Russ_enemies) {
            if (enemy.isActive && RB_CheckEntityCollision(playerBounds, enemy)) {
                player.tempx = 50;
                player.tempy = 250;
            }
        }

        // Goal collision check (only if all coins collected)
        if (static_cast<size_t>(collectedCoins) >= coins.size() &&
                RB_CheckEntityCollision(playerBounds, Russ_goal)) {
            cout << "Victory! All coins collected!" << endl;
            g.game_state = 2;
        }

    }
}
