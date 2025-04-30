//Caroline Contreras Header
#ifndef caroline_H
#define caroline_H

#include <iostream>
#include "functions.h"
#include "fonts.h"
#include <math.h>
#include <GL/glut.h>
#include "Global.h"

class Circle {
    public:
        float PI;
        float cX;
        float cY;
        float r;
        int portal_id;
        float circleSegments;
        Circle() {
            portal_id = 1;
            PI = 3.14159265358979323846f;
            cX = 100.0f;
            cY = 100.0f;
            r = 10.0f;
            circleSegments = 20;
        }
        Circle(float circleX, float circleY, float rad, int cirSegments) {
            cX = circleX;
            cY = circleY;
            r = rad;
            circleSegments = cirSegments;
        }
};
bool isCircleCollidingWithSquare(Circle circle1[], int size);
void initSound();
void cleanupSound();
//void playSound(ALuint source);
void carolineDrawCircle(float pos_x, float pos_y);
void carolineEndCredit (void);
void carolineDisplayWinScreen();
void carolinePhysics(void);
void carolineRender(void);
void carolineLevel(void);

#endif
