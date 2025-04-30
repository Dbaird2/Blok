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

//===========================================================
// Code below should display the win screen for when the 
// the player beats all levels
//===========================================================
void carolineDisplayWinScreen() {
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
