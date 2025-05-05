//Caroline Contreras Header
#ifndef caroline_H
#define caroline_H

#include <iostream>
#include "functions.h"
#include "fonts.h"
#include <math.h>
#include <GL/glut.h>
#include "Global.h"

class Teleportal {
    public:
        float PI;
        float cX;
        float cY;
        float r;
        int portal_id;
        float circleSegments;
        Teleportal() {
            portal_id = 1;
            PI = 3.14159265358979323846f;
            cX = 100.0f;
            cY = 100.0f;
            r = 10.0f;
            circleSegments = 20;
        }
        Teleportal(float circleX, float circleY, float rad, int cirSegments) {
            cX = circleX;
            cY = circleY;
            r = rad;
            circleSegments = cirSegments;
        }
};
bool isCircleCollidingWithSquare(Teleportal portals[], int size);
void initSound();
void cleanupSound();
//void playSound(ALuint source);
void carolineDrawCircle(Teleportal portal[], int isze);
void carolineEndCredit (void);
void carolineDisplayWinScreen();
void carolinePhysics(void);
void carolineRender(void);
void carolineLevel(void);
void initIntroLevel();
void introRender();
void introPhysics();
void playSound(ALuint alSource);
void cleanupSound(ALuint alSource, ALuint alBuffer);

#endif
