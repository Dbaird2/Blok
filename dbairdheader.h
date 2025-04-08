#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Global.h"

struct Grid {
    int state;
    int size;
    int height;
    int width;
    float x;
    float y;
    int r;
    int g;
    int b;
};
extern Grid growing_box[15];

void makeStartScreen();
void dasonEndCredit();
void drawBoxes();
void dasonMenuButtonPress(int x, int y);
void dasonRenderBackground();
void drawPlayerBox(int hard_mode);
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
void dasonTimer(int y, int x, float time_out);
void dasonPlayerDeath(int x_spawn, int y_spawn);

#endif
