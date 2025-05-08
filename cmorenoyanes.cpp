/**********************************
 * Author: Carlos Moreno          *
 * Date:   Spring 2025            *
 * Assignment: blok               *
 **********************************/
#include "fonts.h"
#include "caroline.h"
#include "cmorenoyanesheader.h"
#include "dbairdheader.h"
#include "rbarreyroheader.h"
#include "stoledoheader.h"
#include <math.h>
#include <vector>
#include <ctime>
using namespace std;

#define CARLOS_GRID_SIZE 100
#define MAX_CARLOS_PROJECTILES 20
#define CARLOS_NUM_COINS 3

struct Projectile {
    float x, y;
    float dx, dy;
    float speed;
    float width, height;
    bool active;
};

float circleOffset = 0.0f;
bool direction = true;
Teleportal carlosPortals[4];
Entity carlosTriangles[3];
float triangleCooldown[2] = {0.0f, 0.0f};
Entity carlosCircles[4];
Projectile carlosProjectiles[MAX_CARLOS_PROJECTILES];
vector<Coin> carlosCoins;
Entity carlosGoal = {265, 270, 20, 20, 0, 0};
int carlosCollectedCoins = 0;

Grid carlos_walls[CARLOS_GRID_SIZE];
int wall_height[CARLOS_GRID_SIZE] = {
    5, 500, 5, 500, 5, 50, 5, 25, 5, 25, 50, 5, 25, 5, 5, 50, 55, 5, 30
};
int wall_width[CARLOS_GRID_SIZE] = {
    900, 5, 900, 5, 450, 5, 100, 5, 40, 5, 5, 50, 5, 30, 40, 5, 5, 40, 5
};
int wall_coordinate_x[CARLOS_GRID_SIZE] = {
    0, 895, 0, 5, 0, 445, 350, 255, 290, 325, 700, 745, 795, 770, 45, 80, 750, 790, 830
};
int wall_coordinate_y[CARLOS_GRID_SIZE] = {
    5, 0, 495, 0, 200, 250, 300, 275, 250, 275, 450, 400, 420, 445, 400, 450, 55, 105, 80
};


void carlosEndCredit() {
    Rect r;
    r.bot = 66;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Author 2: Carlos Moreno");
}

void carlosMaze() {
    dasonLoadStruct(carlos_walls, wall_height, wall_width,
        wall_coordinate_x, wall_coordinate_y, CARLOS_GRID_SIZE);

    carlosCoins.clear();

    carlosTriangles[0] = {180.0f, 400.0f, 20.0f, 20.0f, 0.0f, 0.0f};
    carlosTriangles[1] = {200.0f, 100.0f, 20.0f, 20.0f, 0.0f, 0.0f};
    carlosTriangles[2] = {800.0f, 360.0f, 20.0f, 20.0f, 0.0f, 0.0f};
    triangleCooldown[0] = triangleCooldown[1] = 0.0f;

    carlosCircles[0] = {400, 380, 20, 20, 2.0f, 1};
    carlosCircles[1] = {500, 20, 20, 20, 2.0f, -1};
    carlosCircles[2] = {600, 140, 20, 20, 2.0f, 1};
    carlosCircles[3] = {700, 250, 20, 20, 2.0f, -1};

    carlosCoins.push_back({400, 230, false, 250, 20.0f, 2.0f, 0.0f, 10});
    carlosCoins.push_back({350, 400, false, 350, 20.0f, 2.5f, 0.0f, 10});
    carlosCoins.push_back({600, 100, false, 100, 20.0f, 2.8f, 0.0f, 10});
    carlosCoins.push_back({30, 470, false, 470, 20.0f, 2.8f, 0.0f, 10});

    player.tempx = 20;
    player.tempy = 20;
    player.width = 10;
    player.height = 10;
    carlosCollectedCoins = 0;

    // portal id, PI, cX, cY, r, segments;
    for (int i = 0; i < 4; i++) {
        carlosPortals[i].r = 5.0f;
    }
    carlosPortals[0].cX = 310;
    carlosPortals[0].cY = 265;
    carlosPortals[1].cX = 775;
    carlosPortals[1].cY = 425;
    carlosPortals[2].cX = 60;
    carlosPortals[2].cY = 430;
    carlosPortals[3].portal_id = 2;
    carlosPortals[3].cX = 780;
    carlosPortals[3].cY = 80;
    carlosPortals[2].portal_id = 2;
}

void drawTriangles() {
    for (int i = 0; i < 3; ++i) {
        Entity &e = carlosTriangles[i];
        glColor3f(1.0, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(e.x, e.y);
        glVertex2f(e.x + e.width, e.y);
        glVertex2f(e.x + e.width / 2, e.y + e.height);
        glEnd();
    }
}

void drawCircles() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < i; j++) {
            float dx = carlosCircles[i].x - carlosCircles[j].x;
            if (fabs(dx) < 20.0f) {
                carlosCircles[i].dir *= -1;
                break;
            }
        }

        carlosCircles[i].x += carlosCircles[i].speed * carlosCircles[i].dir;
        if (carlosCircles[i].x < 100 || carlosCircles[i].x > 800)
            carlosCircles[i].dir *= -1;

        glColor3f(0.5, 0.5, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(carlosCircles[i].x, carlosCircles[i].y);
        for (int j = 0; j <= 360; j += 10) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(carlosCircles[i].x + cos(angle)*10,
                    carlosCircles[i].y + sin(angle)*10);
        }
        glEnd();
    }
}

void fireCarlosProjectile(float x, float y) {
    for (int i = 0; i < MAX_CARLOS_PROJECTILES; i++) {
        if (!carlosProjectiles[i].active) {
            float dx = player.pos[0] - x;
            float dy = player.pos[1] - y;
            float dist = sqrt(dx*dx + dy*dy);
            dx /= dist; dy /= dist;
            carlosProjectiles[i] = {x, y, dx, dy, 3.0f, 5.0f, 5.0f, true};
            break;
        }
    }
}

void updateCarlosProjectiles() {
    for (int i = 0; i < MAX_CARLOS_PROJECTILES; i++) {
        if (carlosProjectiles[i].active) {
            carlosProjectiles[i].x += carlosProjectiles[i].dx 
                * carlosProjectiles[i].speed;
            carlosProjectiles[i].y += carlosProjectiles[i].dy 
                * carlosProjectiles[i].speed;
            if (carlosProjectiles[i].x < 0 || 
                    carlosProjectiles[i].x > g.xres ||
                    carlosProjectiles[i].y < 0 || 
                    carlosProjectiles[i].y > g.yres)
                carlosProjectiles[i].active = false;

            float px = player.pos[0] - player.width / 2;
            float py = player.pos[1] - player.height / 2;
            if (px < carlosProjectiles[i].x + carlosProjectiles[i].width &&
                px + player.width > carlosProjectiles[i].x &&
                py < carlosProjectiles[i].y + carlosProjectiles[i].height &&
                py + player.height > carlosProjectiles[i].y) {
                dasonPlayerDeath(20, 20);
                carlosMaze();
            }
        }
    }
}

void drawCarlosProjectiles() {
    for (int i = 0; i < MAX_CARLOS_PROJECTILES; i++) {
        if (carlosProjectiles[i].active) {
            SeanDrawRect(carlosProjectiles[i].x, carlosProjectiles[i].y,
                         carlosProjectiles[i].width, 
                         carlosProjectiles[i].height,
                         1.0f, 0.0f, 0.0f);
        }
    }
}

void renderCarlosLevel() {
    dasonDrawWalls(carlos_walls, CARLOS_GRID_SIZE);
    drawTriangles();
    drawCircles();
    drawPlayerBox(0);
    RB_DrawCoins(carlosCoins);
    drawCarlosProjectiles();
    SeanDrawRect(carlosGoal.x, carlosGoal.y, carlosGoal.width,
                         carlosGoal.height, 0.0, 1.0, 0.0);
    carolineDrawCircle(carlosPortals, 4);
}

void carlosPhysics() {
    dasonPhysics(CARLOS_GRID_SIZE, 0, 0, NULL);
    updateCarlosProjectiles();
    RB_UpdateCoins(carlosCoins);
    for (size_t i = 0; i < carlosCoins.size(); i++) {
        if (!carlosCoins[i].collected && RB_CheckEntityCollision(
                {player.pos[0]-5, player.pos[1]-5, 10, 10, 0, true},
                {carlosCoins[i].x-10, carlosCoins[i].y-10, 20, 20, 0, true})) {
            carlosCoins[i].collected = true;
            carlosCollectedCoins += carlosCoins[i].value;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (SeanCheckCollision(carlosCircles[i])) {
            dasonPlayerDeath(20, 20);
            carlosMaze();
        }
    }
    for (int i = 0; i < 2; i++) {
        float dx = player.pos[0] - carlosTriangles[i].x;
        float dy = player.pos[1] - carlosTriangles[i].y;
        float dist = sqrt(dx*dx + dy*dy);
        float now = clock() / (float)CLOCKS_PER_SEC;
        if (dist < 150.0f && now - triangleCooldown[i] > 1.5f) {
            fireCarlosProjectile(carlosTriangles[i].x, carlosTriangles[i].y);
            triangleCooldown[i] = now;
        }
    }
    float px = player.pos[0], py = player.pos[1];
    if (px > carlosGoal.x && px < carlosGoal.x + carlosGoal.width &&
        py > carlosGoal.y && py < carlosGoal.y + carlosGoal.height &&
        carlosCollectedCoins >= 30) {
        carlosEndCredit();
        g.game_state = 9;
    }
    isCircleCollidingWithSquare(carlosPortals, 4);
}

void renderInstructions() {
    Rect title;
    title.bot = 450;
    title.left = 15;
    title.center = 0;
    ggprint8b(&title, 16, 0x00000000, "A - Move Left");
    ggprint8b(&title, 16, 0x00000000, "W - Move Up");
    ggprint8b(&title, 16, 0x00000000, "S - Move Down");
    ggprint8b(&title, 16, 0x00000000, "D - Move Right");
    ggprint8b(&title, 16, 0x00000000, "C - Credits");
    ggprint8b(&title, 16, 0x00000000, "Z - Toggle FPS");
    ggprint8b(&title, 16, 0x00000000, "Up Arrow Key - Increase FPS");
    ggprint8b(&title, 16, 0x00000000, "Down Arrow Key - Decrease FPS");
    ggprint8b(&title, 16, 0x00000000, "Q - Quit Level");
}
