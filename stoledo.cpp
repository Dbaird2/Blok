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
#include "functions.h"
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
struct Entity{
    float x, y, size;
    float speed;
};

//Enemies
Entity enemies[] = {
    {300, 200, 20, 3},
    {500, 400, 20, 3}
};

//Enemy Direction
float enemydir[] = {1, -1};

//Draw Enemy Rectangle
void drawRect(float x, float y, float size, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);                   
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
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


void seanrungame() {
    if (g.game_state == 4) {
        // Draw Player Box
        drawPlayerBox();
        updateEnemies();
        for (int i = 0; i < 2; i++)
        drawRect(enemies[i].x, enemies[i].y, enemies[i].size, 1, 0, 0);
        
        
    }
}

