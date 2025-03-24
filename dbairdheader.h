#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Global.h"

struct Grid {
    int state;
    int size;
    int height;
    int width;
    int x;
    int y;
    int r;
    int g;
    int b;
};
extern Grid growing_box[10];

void makeStartScreen();
void dasonEndCredit();
void drawBoxes();
void dasonMenuButtonPress(int x, int y);
void dasonRenderBackground();
void drawPlayerBox();
void handleKeyPress(XKeyEvent *event);
void handleKeyRelease(XKeyEvent *event);
void processMovement();
void dasonMazeLevelBackground();
void init_dasonMazePlayer();
void dasonLoadStruct(Grid grid[], int height[], int width[], 
        int x[], int y[], int size);
void dasonDrawGrowingBoxes(Grid grid[], int size);
void dasonDrawWalls(Grid grid[], int size);
void dasonPhysics(int n, int l, int check, Grid grid[]);
void dasonMazeRender();

#endif
