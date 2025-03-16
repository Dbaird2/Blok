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
//#include <cmath>
//#include <X11/Xlib.h>
//#include <X11/keysym.h>
//#include <GL/glx.h>
#include "fonts.h"
//#include <pthread.h>
//#include "functions.h"
#include "dbairdheader.h"
#include "Global.h"
//#include "Image.h"



void seanEndCredit(void) {
    Rect title;
    title.bot = 18;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 5: Sean Toledo");
}

//Entity
struct Entity {
    float x, y;
    float width, height;  
    float speed;
};
//Goal
Entity goal = {800, 250, 25, 25, 0};

//Enemies
Entity enemies[] = {
    {300, 200, 20, 20, 0.5},
    {500, 400, 20, 20, 0.5},
    {300, 200, 20, 20, 0.5},
    {500, 400, 20, 20, 0.5}
};

//Enemy Direction
float enemydir[] = {1, -1};

//Draw Enemy Rectangle
void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);                   
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}


//Enemy Movement
void updateEnemies() {
    for (int i = 0; i < 2; i++) {
        enemies[i].y += enemydir[i] * enemies[i].speed;
        if (enemies[i].y <= 100 || enemies[i].y >= g.yres - 100)
            enemydir[i] *= -1; 
    
    }
    for (int j = 2; j < 4; j++) {
        enemies[j].x += enemydir[j] * enemies[j].speed;
        if (enemies[j].x <= 100 || enemies[j].x >= g.xres - 100)
            enemydir[j] *= -1; 
    
    }
}


//Collision
bool checkCollision(Entity &enemy) {
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



void seanrungame() {
    if (g.game_state == 4) {
        static bool initialized = false;
        if (!initialized) {
            player.tempx = 50;
            player.tempy = 250;
            initialized = true;
        }
        // Draw Player Box
        drawPlayerBox();
        updateEnemies();
        for (int i = 0; i < 4; i++) {
            if (checkCollision(enemies[i])) {
                cout << "Collision detected with enemy " << i << "!\n";
                player.tempx = 50;
                player.tempy = 250; 
            }
        }
        drawRect(goal.x, goal.y, goal.width, goal.height, 0, 1, 0);
        for (int i = 0; i < 4; i++)
        drawRect(enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height,1, 0, 0);
        
        if (checkCollision(goal)) {
            cout << "You Win!" << endl;
            g.game_state = 2;
    }
    }
}

 