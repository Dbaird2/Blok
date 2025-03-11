#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <iostream>
#include "Global.h"

void makeStartScreen();
void dasonEndCredit();
void drawBoxes();
void dasonPhysics(int n);
void dasonMenuButtonPress(int x, int y);
void dasonRenderBackground();
void drawPlayerBox();
void handleKeyPress(XKeyEvent *event);
void handleKeyRelease(XKeyEvent *event);
void processMovement();
/*class Image {
    public:
        int width, height;
        unsigned char *data;

        ~Image();                       
        Image(const char *fname);       
        void render();
};*/

#endif
