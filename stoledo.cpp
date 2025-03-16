//Sean Toledo
//Date : Spring 2025
//Blok
//
//#include <iostream>
//using namespace std;
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


//Enemies
Entity enemies[] = {
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
}


//Collision
bool checkCollision(Entity &enemy) {
    return (player.pos[0] < enemy.x + enemy.width &&
            player.pos[0] + player.width > enemy.x &&  
            player.pos[1] < enemy.y + enemy.height &&
            player.pos[1] + player.height > enemy.y);
}


void seanrungame() {
    if (g.game_state == 4) {
        // Draw Player Box
        drawPlayerBox();
        updateEnemies();
        for (int i = 0; i < 2; i++) {
            if (checkCollision(enemies[i])) {
                player.pos[0] = 100;
                player.pos[1] = 300; // Reset player on collision
            }
        }
        for (int i = 0; i < 2; i++)
        drawRect(enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height,1, 0, 0);
        
        
    }
}

 