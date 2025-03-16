#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <unistd.h> 
//#include <iostream>
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
void dasonMazeLevelBackground();
void init_dasonMazePlayer();

struct Grid {
    int state;
    int height;
    int width;
    int x;
    int y;
};

void dasonLoadStruct(Grid grid[], int height[], int width[], 
        int x[], int y[], int size);
void dasonDrawWalls(Grid grid[], int size);

#endif
