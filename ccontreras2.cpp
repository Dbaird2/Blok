//Caroline Contreras
//Spring 2025
//3350 Project

#include <iostream>
#include "functions.h"
#include "fonts.h"
#include <math.h>
#include <GL/gl.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "caroline.h"
#include <vector>
#include "dbairdheader.h"
#include "rbarreyroheader.h"
#include "stoledoheader.h"
#include "cmorenoyanesheader.h"
using namespace std;

//GOALS: Make walls, fix sound in code, and finish win screen 

//====================4.22.25 Making Walls==========================
#define CAROLINE_GRID_SIZE 5

Grid caroWalls[CAROLINE_GRID_SIZE];
int caro_height[5] = {10, 490, 490, 10, 10};
int caro_width[5] = {890, 10, 10, 890, 300};
int caro_x[5] = {10, 10, 890, 890, 420};
int caro_y[5] = {10, 10, 10, 490, 60};

//==================================================================

#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif 
ALuint alBuffer;
ALuint alSource;
#ifdef USE_OPENAL_SOUND

void initSound()
{
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	alGetError();
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	
	//Buffer holds the sound information.
	//FIX THIS CODE BELOW
	alBuffer = alutCreateBufferFromFile("./winningScreenAudio.wav");
	//winScreen = alutCreateBufferFromFile("./src/sounds/");
	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	#endif //USE_OPENAL_SOUND
}

void cleanupSound(ALuint alSource, ALuint alBuffer)
{
        //First delete the source.
        alDeleteSources(1, &alSource);
        //Delete the buffer.
        alDeleteBuffers(1, &alBuffer);
        //Close out OpenAL itself.
        //Get active context.
        ALCcontext *Context = alcGetCurrentContext();
        //Get device for active context.
        ALCdevice *Device = alcGetContextsDevice(Context);
        //Disable context.
        alcMakeContextCurrent(NULL);
        //Release context(s).
        alcDestroyContext(Context);
        //Close device.
        alcCloseDevice(Device);
}

void playSound(ALuint alSource)
{
#ifdef USE_OPENAL_SOUND
	alSourcePlay(alSource);
#endif
}
#endif //USE_OPENAL_SOUND
//=========================================================

Teleportal portal[2];
void carolineDrawCircle(Teleportal portal[], int array_size) {
    for (int i = 0; i < array_size; i++) {
        glPushMatrix();
        glColor3f(0.5f, 0.5f, 0.5f);
        glTranslatef(portal[i].cX, portal[i].cY, 0.0f);

        glBegin(GL_TRIANGLE_FAN);//basically uses a bunch of triangles 
                                 //to make a circle
        glVertex2f(0.0f, 0.0f); // Center of the circle
        for (int j = 0; j <= portal[i].circleSegments; ++j) {
            float angle = 2.0f * portal[i].PI * 
                float(j) / float(portal[i].circleSegments);
            float x = portal[i].r * cos(angle);
            float y = portal[i].r * sin(angle);
            glVertex2f(x, y);
        }
	glEnd();
	glPopMatrix();
    }
}


bool isCircleCollidingWithSquare(Teleportal portal[], int array_size) {
    bool check = false;
    for (int i = 0; i < array_size; i++) {
        float closestX = max(player.pos[0], 
                min(portal[i].cX, player.pos[0] + player.width));
        float closestY = max(player.pos[1], 
                min(portal[i].cY, player.pos[1] + player.height));

        float dx = portal[i].cX - closestX;
        float dy = portal[i].cY - closestY;
        int port_val = portal[i].portal_id;
        int save_i = i;

        if ((dx * dx + dy * dy) <= (portal[i].r * portal[i].r)) {
            check = true;
            for (int j = 0; j < array_size; j++) {
                if (save_i != j && port_val == portal[j].portal_id) {
                    player.tempx = portal[j].cX;
                    player.tempy = portal[j].cY + portal[j].r + player.height;
                    break;
                }
            }
        }
    }
    return check;
}

void carolineDisplayWinScreen() {
	/*
	ren.backgroundImage = &img[2];
	glGenTextures(1, &ren.backgroundTexture);
	int w = ren.backgroundImage->width;
	int h = ren.backgroundImage->height;
	glBindTexture(GL_TEXTURE_2D, ren.backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, ren.backgroundImage->data);
	//playSound(alSource);
	//
	*/
}

//below is my function for my end credit
void carolineEndCredit (void)
{
	Rect title;
	title.bot = 34;
	title.left = 10;
	title.center = 0;
	ggprint8b(&title, 16, 0x00ff0000, "Author 4: Caroline Contreras");
}

void carolinePhysics(void) {
	dasonPhysics(CAROLINE_GRID_SIZE, 0, 0, NULL);
    isCircleCollidingWithSquare(portal, 2);
}

void carolineRender(void) {
	carolineDrawCircle(portal, 2);
	dasonDrawWalls(caroWalls, CAROLINE_GRID_SIZE);
	
}

void carolineLevel(void) {
	dasonLoadStruct(caroWalls, caro_height, caro_width,
			caro_x, caro_y, CAROLINE_GRID_SIZE);
	player.width = 45;
	player.height = 15;
}
/* START OF ANIMATED INTRO */
#define NUM_COINS 3
#define INTRO_GRID_SIZE 6

static vector<Coin> intro_coins;
int coins_collected;
int coins_score;
Entity intro_enemies[8] = {
    {700.0, 300.0, 20.0, 20.0, 7.0, 1},
    {120.0, 250.0, 20.0, 20.0, 7.0, -1},
    {200.0, 180.0, 20.0, 20.0, 7.0, 1},
    {400.0, 50.0, 20.0, 20.0, 7.0, -1},
    {750.0, 400.0, 20.0, 20.0, 7.0, -1},
    {300.0, 150.0, 20.0, 20.0, 7.0, 1},
    {765.0, 350.0, 20.0, 20.0, 7.0, 1},
    {420.0, 35.0, 20.0, 20.0, 7.0, -1},
};
Entity intro_goal = {750, 450, (float)player.width, (float)player.height, 0, 0};

Grid intro_grid[INTRO_GRID_SIZE];
int intro_height[INTRO_GRID_SIZE] = {5, 5, 250, 250};
int intro_width[INTRO_GRID_SIZE] = {450, 450, 5, 5};
int intro_x[INTRO_GRID_SIZE] = {450, 450, 5, 895};
int intro_y[INTRO_GRID_SIZE] = {5, 495, 250, 250};

void initIntroLevel()
{
    intro_coins.clear();
    coins_score = 0;
    coins_collected = 0;
    float cx[3] = {420.0f, 150.0f, 765.0f};
    float cy[3] = {35.0f, 250.0f,350.0f};
    for (int i = 0; i < NUM_COINS; i++) {
        float theta= (2.0f * M_PI / 8) * i;
        intro_coins.push_back({cx[i], cy[i], false, cy[i],
                20.0f, 2.0f + i * 0.2f, theta, 10});
    }
    player.tempx = 40;
    player.tempy = 40;
    dasonLoadStruct(intro_grid, intro_height, intro_width,
            intro_x, intro_y, INTRO_GRID_SIZE);

}

void introRender()
{
    for (int i = 0; i < 8; i++)
        SeanDrawRect(intro_enemies[i].x, intro_enemies[i].y,
                intro_enemies[i].width, intro_enemies[i].height,
                1.0f, 0.0f, 0.0f);

    SeanDrawRect(intro_goal.x, intro_goal.y,
            intro_goal.width, intro_goal.height, 0, 1, 0);

    dasonDrawWalls(intro_grid, INTRO_GRID_SIZE);
    renderDeathCount();
    RB_DrawCoins(intro_coins);
    dasonTimer(10, 530, 490, 830, 120.0);

    g.animationTime += 0.3f;
    Rect r;
    r.bot = g.yres/2 + sin(g.animationTime) * 10;
    r.left = g.xres/2 + cos(g.animationTime) * 10;
    r.center = 0 ;
    ggprint8b(&r, 20, 0xFF0000, "P R E S S ' F ' T O S T A R T");

}
int intro_lower[2] = {55, 250};
int intro_upper[2] = {215, 350};
void introPhysics()
{

    coins_score = RB_CheckCoinCollection(intro_coins, coins_score);
    coins_collected = coins_score / 10;
    SeanEnemiesVertical(0, 1, 330, 270, intro_enemies);
    SeanEnemiesVertical(1, 2, 285, 150, intro_enemies);
    SeanEnemiesVertical(2, 3, 230, 140, intro_enemies);
    SeanEnemiesVertical(3, 4, 110, 5, intro_enemies);
    SeanEnemiesHorizontal(4, 5, 1400, 500, intro_enemies);
    SeanEnemiesHorizontal(5, 6, 600, 200, intro_enemies);
    SeanEnemiesHorizontal(6, 7, 1530, 715, intro_enemies);
    SeanEnemiesHorizontal(7, 8, 840, 370, intro_enemies);
    for (int i = 0; i < 8; i++) {
        Player *p = &player;
        int x_offset = p->width;
        int y_offset = p->height;
        int box_top = intro_enemies[i].y + intro_enemies[i].height + 5;
        int box_bot = intro_enemies[i].y - intro_enemies[i].height - 5;
        int box_left = intro_enemies[i].x - intro_enemies[i].width - 5;
        int box_right = intro_enemies[i].x + intro_enemies[i].width + 5;

        if ((p->pos[1] <= box_top + y_offset)
                && (p->pos[1] >= box_bot - y_offset)
                && (p->pos[0] >= box_left - x_offset)
                && (p->pos[0] <= box_right + x_offset)) {
            if (p->pos[1] >= box_top - y_offset/4) {
                p->tempy += 10;
            }
            else if (p->pos[1] <= box_bot+y_offset/10) {
                p->tempy -= 10;
            }
            else if (p->pos[0] >= box_right) {
                p->tempx += 10;
            }
            else if (p->pos[0] <= box_left) {
                p->tempx -= 10;
            }
            if (SeanCheckCollision(intro_enemies[i])) {
                dasonPlayerDeath(40, 40);
                initIntroLevel();
            }
        }
    }

    if (SeanCheckCollision(intro_goal) &&
            coins_collected >= NUM_COINS) {
        player.death_count = 0;
        initIntroLevel();
    }
    if (coins_collected < NUM_COINS) {
        // COINS NOT COLLECTED

        if (player.tempx < intro_coins[coins_collected].x) {
            player.tempx += 5;
        } else if (player.tempx > intro_coins[coins_collected].x) {
            player.tempx -= 5;
        }

        if (player.tempy > intro_coins[coins_collected].y) {
            player.tempy -= 5;
        } else if (player.tempy < intro_coins[coins_collected].y) {
            player.tempy += 5;
        }

    } else {
        if (player.tempx < intro_goal.x) {
            player.tempx += 5;
        } else if (player.tempx > intro_goal.x){
            player.tempx -= 5;
        }

        if (player.tempy < intro_goal.y) {
            player.tempy += 5;
        } else if (player.tempy > intro_goal.y) {
            player.tempy -= 5;
        }
    }
}
