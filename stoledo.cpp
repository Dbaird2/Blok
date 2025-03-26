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
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "stoledoheader.h"

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
void SeanDrawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);                   
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

//Enemy Movement
void SeanEnemyMovementVertical() {
    for (int i = 0; i < 2; i++) {
        //Up and Down
        enemies[i].y += enemies[i].dir * enemies[i].speed;
        if (enemies[i].y <= 100 || enemies[i].y >= g.yres - 100)
            enemies[i].dir *= -1; 
    
    }
}
void SeanEnemyMovementHorizontal() {
    for (int j = 2; j < 4; j++) {
        enemies[j].x += enemies[j].dir * enemies[j].speed;

        if (enemies[j].x <= 100 || enemies[j].x >= g.xres - 100)
            enemies[j].dir *= -1;
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
    r.bot = 20;        
    r.left = 10;       
    r.center = 0;

    ggprint8b(&r, 16, 0xff0000ff, "Deaths:");
    ggprint8b(&r, 16, 0xff0000ff, "%d", deathCount);
}
int deathcounter = 0;
void seanrungame() {
    //initAudio("background.wav");
    static bool initialized = false;
    if (g.game_state == 4) {
        if (!initialized) {
            player.tempx = 50;
            player.tempy = 250;
            initialized = true;
        }
        // Draw Player Box
        drawDeathCounter(deathcounter);
        drawPlayerBox();
        SeanEnemyMovementVertical();
        SeanEnemyMovementHorizontal();
        for (int i = 0; i < 4; i++) {
            if (SeanCheckCollision(enemies[i])) {
                //cout << "Collision detected with enemy " << i << "!\n";
                player.tempx = 50;
                player.tempy = 250; 
                deathcounter++;
            }
        }
        SeanDrawRect(goal.x, goal.y, goal.width, goal.height, 0, 1, 0);
        for (int i = 0; i < 4; i++)
        SeanDrawRect(enemies[i].x, enemies[i].y, enemies[i].width, enemies[i].height,1, 0, 0);
        
         if (SeanCheckCollision(goal)) {
            cout << "You Win!" << endl;
            g.game_state = 0 ;
         }
      
        
          

    
    

    }
}

 