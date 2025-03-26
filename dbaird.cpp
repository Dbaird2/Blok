// Name: Dason Baird
// Date: Spring 2025
//
//
/* If you want growing boxes in your levels
 * Then in your own file do
 * Step 1) make a struct Grid name[size],
 * Step 2) call dasonLoadStruct(name_of_grid, height_of_boxes, width_of_boxes,
 *                      xpos_of_boxes, ypos_of_boxes, size);
 * Step 3) call dasonDrawWalls(name_of_grid, size_of_struct);
 *      OR call dasonDrawGrowingBoxes(name_of_grid, size_of_struct);
 * Step 4) lastly call dasonPhysics(amount_of_walls, amount_of_growing_boxes
 *                          , 1, name_of_grid);      
 * If you ALSO want WALLS then do Step 1, Step 2, & Step 3 but for walls.
 *
 * Note: If you choose to not have growing boxes then just do
 * Step 1, 2, 3 for Walls, and call 
 * dasonPhysics(amount_of_walls, 0, 0, NULL)
 *
 * If you want a player box call drawPlayerBox
 * If you want to change it's position change player.tempx or player.tempy
 * for size do player.width, player.height
 */
using namespace std;
#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>
#include <GL/glx.h>
#include "fonts.h"
#include "Global.h"
#include "dbairdheader.h"

/*---------------------------------------------------------------------------*/
/* --------------- MAP WALL STRUCTURES --------------------------------------*/
#define DASON_GRID_SIZE 58
Grid dason_grid[DASON_GRID_SIZE];
int dason_height[58] = {5
    , 5, 20, 5, 60, 5, 60, 5, 45, 50, 250, 5, 5, 
    250,
    115, 5, 130, 5, 90, 5, 5, 5, 80, 5, 60/**/, 5, 5,
    60, 65, 5, 60, 70, 5, 90, 5, 60, 50, 5, 5, 50, 5, 5,
    60, /**/100, 30, 200, 5, 30, 40, 5, 200, /**/
    40, 40, 40, 40, 40, 40, 40};
int dason_width[58] = {250
    , 175, 5, 200, 5, 75, 5, 50, 5, 5, 5, 250, 175,
    5,
    5, 95, 5, 20, 5, 120, 30, 25, 5, 20, 5/**/, 30, 90, 5,
    5, 35, 5, 5, 90, 5, 50, 5, 5, 15, 45, 5, 45, 60, 5,
    /**/5, 100, 5, 90, 95, 5, 60, 20, /**/
    5, 5, 5, 5, 5, 5, 5};
int dason_x[58] = {250
    , 725, 555, 485, 290, 220, 250, 205, 195, 150, 5, 
    660,
    185, 895, 105, 200, 55, 80, 335, 220, 400, 35, 375, 360,
    105/**/, 70, 100, 160, 195, 265, 335, 235, 150, 295, 350,
    375, 410, 395, 425, 465, 425, 440, 445, /**/495, 590, 720,
    625, 595, 670, 725, 805, /**/
    635, 600, 565, 530, 495, 460, 425};
int dason_y[58] = {5
    , 5, 30, 45, 110, 175, 70, 135, 55, 90, 260, 495, 495,
    250, 155, 220, 170, 160, 180, 270, 95, 335, 180, 265,
    365/**/, 385, 430, 335, 370, 310, 335, 385, 460, 405, 430,
    370, 290, 335, 210, 100, 150, 370, 275, /**/ 270, 140,
    210, 245, 345, 415, 450, 255, /**/
    415, 415, 415, 415, 415, 415, 415};
/*---------------------------------------------------------------------------*/

//Grid growing_box[10];
int growing_height[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int growing_width[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5,5};
int growing_x[10] = {40, 350, 110, 200, 320, 440, 160, 230, 410, 200};
int growing_y[10] = {40, 15, 80, 300, 220, 340, 60, 450, 250, 200};

void dasonEndCredit(void)
{
    Rect title;
    title.bot = 50;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ffffff, "Author 3: Dason Baird");
}

int b = 0;
void init_dasonMazePlayer();
void dasonMenuButtonPress(int x, int y) 
{
    /* Start/Credits button Collision Detection */
    if (g.game_state == 1) {
        for (int j = 0; j < g.menu_box_amt[g.game_state-1]; j++) {
            MenuBox *c = &boxes[j];
            // Colision detection
            float cx = c->pos[0];
            float cy = c->pos[1];
            float ch = c->height;
            float cw = c->width;
            if (y <= (cy + ch) &&
                    (x >= (cx - cw)) &&
                    (x <= (cx + cw)) &&
                    (y >= (cy - ch))) {
                if (j == 1) {
                    g.game_state = 2;
                    b = 0;
                } else {
                    g.credit = !g.credit;
                }
            }
        }
        /* Difficulty button Collision Detection */
    } else if (g.game_state == 2) {
        player.stop_w = 0;
        for (int j = 0; j < g.menu_box_amt[g.game_state-1]; j++) {
            MenuBox *c = &boxes[j];
            float cx = c->pos[0];
            float cy = c->pos[1];
            float ch = c->height;
            float cw = c->width;
            // Colision detected
            if (y <= (cy + ch) &&
                    (x >= (cx - cw)) &&
                    (x <= (cx + cw)) &&
                    (y >= (cy - ch))) {
                if (j == 0){
                    // CAROLINE
                    g.game_state = 7;
                    //glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 1) {
                    // SEAN
                    g.game_state = 4;
                    //glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 2) {
                    // CARLOS
                    g.game_state = 3;
                    //glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 3) {
                    // DASON
                    g.game_state = 6;
                    //glDeleteTextures(1, &ren.backgroundTexture);
                    init_dasonMazePlayer();
                } else if (j == 4) {
                    // RJ
                    g.game_state = 5;
                    //glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 5) {
                    g.game_state = 1;
                    b = 0;
                }
            }
        }
    }
}

void renderDeathCount()
{
    Rect title;
    title.bot = 0;
    title.left = 0;
    title.center = 0;
    ggprint8b(&title, 0, 0x00ff0000, "Death count: %i", player.death_count);

}

void init_dasonMazePlayer() 
{
    /* GROWING BOX ENEMIES */
    dasonLoadStruct(growing_box, growing_height, growing_width, 
            growing_x, growing_y, 10);
    /* WALLS */
    dasonLoadStruct(dason_grid, dason_height, dason_width, 
            dason_x, dason_y, DASON_GRID_SIZE);
    player.tempx = 530;
    player.tempy = 5;
    player.width = 5;
    player.height = 5;
}

void dasonMazeRender()
{
    dasonDrawGrowingBoxes(growing_box, 10);
    dasonDrawWalls(dason_grid, DASON_GRID_SIZE);
    renderDeathCount(); 
    if (g.key_states[XK_q]) {
        std::fill(walls, walls + 100, Wall());
        player.death_count = 0;
        g.game_state = 2;
    }
}

void defineBox() 
{
    if (g.game_state == 1) {
        if (b >= g.menu_box_amt[g.game_state-1])
            return;
    } else if (g.game_state == 2) {
        if (b >= g.menu_box_amt[g.game_state-1]) 
            return;
    } else if (g.game_state > 2) {
        return;
    }
    boxes[b].width = 55;
    boxes[b].height = 15;
    boxes[b].pos[0] = g.xres /2.5;
    boxes[b].pos[1] = 3*b * 15 +80;
    boxes[b].color[0] = 1.0;  
    boxes[b].color[1] = 0.0;
    boxes[b].color[2] = 0.0;
    ++b;
}

/*--------------------------------------------------------------------------*/

void dasonLoadStruct(Grid grid[], int height[], int width[], 
        int x[], int y[], int size) 
{
    for (int i = 0; i < size; i++) {
        grid[i].height = height[i];
        grid[i].width = width[i];
        grid[i].x = x[i];
        grid[i].y = y[i];
        grid[i].r = rand() % 256;
        grid[i].g = rand() % 256;
        grid[i].b = rand() % 256;
    }
}

void dasonDrawGrowingBoxes(Grid grid[], int size)
{
    for ( int i = 0; i < size; i++) {

        glPushMatrix();
        glColor3ub(grid[i].r, grid[i].g,grid[i].b);
        Wall *b = &growing_boxes[i];

        b->width = grid[i].width;
        b->height = grid[i].height;
        b->pos[0] = grid[i].x;
        b->pos[1] = grid[i].y;
        b->color[0] = grid[i].r;
        b->color[1] = grid[i].g;
        b->color[2] = grid[i].b;

        int width = b->width;
        int height = b->height;
        glTranslatef(b->pos[0], b->pos[1], 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-width, -height);
        glVertex2f(-width,  height);
        glVertex2f( width,  height);
        glVertex2f( width, -height);
        glEnd();
        glPopMatrix();
    }
}

void dasonDrawWalls(Grid grid[], int size)
{
    for ( int i = 0; i < size; i++) {

        glPushMatrix();
        if (size == 10)
            glColor3ub(grid[i].r, grid[i].g,grid[i].b);
        else
            glColor3ub(0, 0, 0);
        Wall *w = &walls[i];

        w->width = grid[i].width;
        w->height = grid[i].height;
        w->pos[0] = grid[i].x;
        w->pos[1] = grid[i].y;
        w->color[0] = grid[i].r;
        w->color[1] = grid[i].g;
        w->color[2] = grid[i].b;

        int width = w->width;
        int height = w->height;
        glTranslatef(w->pos[0], w->pos[1], 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-width, -height);
        glVertex2f(-width,  height);
        glVertex2f( width,  height);
        glVertex2f( width, -height);
        glEnd();
        glPopMatrix();
    }
}

void growingBoxPhysics(int size, Grid grid[]) 
{
    if (size < 0) 
        return;
    for (int i = 0; i < size; i++) {
        Player *p = &player;
        int box_x = grid[i].x;
        int box_y = grid[i].y;
        int box_h = grid[i].height;
        int box_w = grid[i].width;

        float bounceOffset = sin(g.grow_animation) * g.bounceHeight;
        grid[i].width += bounceOffset+sin(g.grow_animation)*0.1f*8.0;
        grid[i].height += bounceOffset+sin(g.grow_animation)*0.1f*8.0;
        g.grow_animation += 0.005f;

        int x_offset = p->width;
        int y_offset = p->height;
        int box_top = box_y + box_h;
        int box_bot = box_y - box_h;
        int box_left = box_x - box_w;
        int box_right = box_x + box_w;

        if (bounceOffset < 0) {
            if ((p->pos[1] >= box_top - y_offset)
                    && (p->pos[1] <= box_bot + y_offset)
                    && (p->pos[0] <= box_left + x_offset) 
                    && (p->pos[0] >= box_right - x_offset)) {
                p->death_count++;
                if (p->pos[1] <= box_top + y_offset/4) {
                    p->tempy = 5;
                    p->tempx = 530;
                } else if (p->pos[1] >= box_bot - y_offset/10) {
                    p->tempy = 5;
                    p->tempx = 530;
                } else if (p->pos[0] <= box_right) {
                    p->tempx = 530;
                    p->tempy = 5;
                } else if (p->pos[0] >= box_left) {
                    p->tempx = 530;
                    p->tempy = 5;
                }
            } 

        } else {
            if ((p->pos[1] <= box_top + y_offset)
                    && (p->pos[1] >= box_bot - y_offset)
                    && (p->pos[0] >= box_left - x_offset) 
                    && (p->pos[0] <= box_right + x_offset)) {
                p->death_count++;
                if (p->pos[1] >= box_top - y_offset/4) {
                    p->tempy = 5;
                    p->tempx = 530;
                } else if (p->pos[1] <= box_bot+y_offset/10) {
                    p->tempy = 5;
                    p->tempx = 530;
                } else if (p->pos[0] >= box_right) {
                    p->tempx = 530;
                    p->tempy = 5;
                } else if (p->pos[0] <= box_left) {
                    p->tempx = 530;
                    p->tempy = 5;
                }
            } 
        }
    }
}



void dasonPhysics(int wall_size, int growing_size, 
        int growing_enemy_check, Grid grid[])
{
    if (g.game_state == 6) {
        //checkCollision(moving_enemy);
    }

    glClear(GL_COLOR_BUFFER_BIT);

    if (growing_enemy_check) 
        growingBoxPhysics(growing_size, grid);
    for (int i = 0; i < wall_size; i++) {
        Wall *w = &walls[i];
        Player *p = &player;


        int x_offset = p->width;
        int y_offset = p->height;
        int box_top = w->pos[1] + w->height /*- p->height*/;
        int box_bot = w->pos[1] - w->height;
        int box_left = w->pos[0] - w->width;
        int box_right = w->pos[0] + w->width;

        if ((p->pos[1] <= box_top + y_offset)
                && (p->pos[1] >= box_bot - y_offset)
                && (p->pos[0] >= box_left - x_offset) 
                && (p->pos[0] <= box_right + x_offset)) {
            if (p->pos[1] >= box_top - y_offset/4) 
                p->tempy += 5;
            else if (p->pos[1] <= box_bot+y_offset/10) 
                p->tempy -= 5;
            else if (p->pos[0] >= box_right) 
                p->tempx += 5;
            else if (p->pos[0] <= box_left) 
                p->tempx -= 5;
        } 
    }
}
/*--------------------------------------------------------------------------*/

/*  */
void dasonRenderBackground() 
{
    ren.backgroundImage = &img[0];
    glGenTextures(1, &ren.backgroundTexture);
    int w = ren.backgroundImage->width;
    int h = ren.backgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, ren.backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, ren.backgroundImage->data);

}
void dasonMazeLevelBackground() 
{
    ren.dasonLevelBackgroundImage = &img[1];
    glGenTextures(1, &ren.dasonLevelBackgroundTexture);
    int w = ren.dasonLevelBackgroundImage->width;
    int h = ren.dasonLevelBackgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, ren.dasonLevelBackgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*unsigned char *dasonMazeData = 
     * ren.dasonLevelBackgroundImage->buildAlphaData(&img[1]);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
     GL_RGB, GL_UNSIGNED_BYTE, dasonMazeData);
     free(dasonMazeData);*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, ren.dasonLevelBackgroundImage->data);
}
/*---------------------------------------------------*/

void makeStartScreen() 
{
    glClear(GL_COLOR_BUFFER_BIT);
    float quadWidth = g.xres;
    float quadHeight = g.yres;
    float screenAspect = static_cast<float>(g.xres) / g.yres;
    if (g.game_state < 3) {
        float imageAspect = 
            static_cast<float>(ren.backgroundImage->width) 
            / ren.backgroundImage->height;

        // Adjust width/height based on aspect ratio
        if (screenAspect > imageAspect) {
            quadWidth = g.yres * imageAspect;
        } else {
            quadHeight = g.xres / imageAspect;
        }

        // Center the image in the viewport
        float xOffset = (g.xres - quadWidth) / 2.0;
        float yOffset = (g.yres - quadHeight) / 2.0;

        glBindTexture(GL_TEXTURE_2D, ren.backgroundTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(xOffset, yOffset-30);
        glTexCoord2f(0.0, 0.0); glVertex2f(xOffset, yOffset + quadHeight+30); 
        glTexCoord2f(1.0, 0.0); glVertex2f(xOffset + quadWidth, 
                yOffset + quadHeight+30); 
        glTexCoord2f(1.0, 1.0); glVertex2f(xOffset + quadWidth, yOffset-30);

        glEnd();
        glPopMatrix();
    }
    if (g.game_state == 6) {
        /*float imageAspect = 
            static_cast<float>(ren.dasonLevelBackgroundImage->width) 
            / ren.dasonLevelBackgroundImage->height;

        // Adjust width/height based on aspect ratio
        if (screenAspect > imageAspect) {
            quadWidth = g.yres * imageAspect;
        } else {
            quadHeight = g.xres / imageAspect;
        }

        // Center the image in the viewport
        float xOffset = (g.xres - quadWidth) / 2.0;
        float yOffset = (g.yres - quadHeight) / 2.0;

        glBindTexture(GL_TEXTURE_2D, ren.dasonLevelBackgroundTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(xOffset, yOffset-30);
        glTexCoord2f(0.0, 0.0); glVertex2f(xOffset, yOffset + quadHeight+30); 
        glTexCoord2f(1.0, 0.0); glVertex2f(xOffset + quadWidth, 
                yOffset + quadHeight+30); 
        glTexCoord2f(1.0, 1.0); glVertex2f(xOffset + quadWidth, yOffset-30);

        glEnd();
        glPopMatrix();
        */
    }
}



/*----------------------------------------------------*/
/* START MENU BOXEES */
int j = 0;
void drawBoxes() 
{
    //draw the boxes

    defineBox(); 
    if (g.game_state == 1) {
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *box = &boxes[i];

            Rect rect;
            //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glPushMatrix();
            box->color[0] = 1.0f;
            box->color[1] = 0.0f;
            box->color[2] = 0.0f;
            glColor3f(1.0f, 0.0f, 0.0f);
            glDisable(GL_BLEND);
            glTranslatef(box->pos[0], box->pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-box->width, -box->height);
            glVertex2f(-box->width,  box->height);
            glVertex2f( box->width, box->height);
            glVertex2f( box->width, -box->height);
            glEnd();
            glPopMatrix();

            rect.bot = box->pos[1]-7;
            rect.center = 0;
            float bounceOffset = sin(g.animationTime) * g.bounceHeight;
            if (i == 0)
                box->pos[1] += bounceOffset;
            if (i == 1) {
                box->pos[1] += bounceOffset+sin(g.animationTime);
            }

            switch (i)
            {
                case 1:
                    rect.left = box->pos[0] - 17;
                    ggprint8b(&rect, 0, 0x00ffffff, "Start!");
                    break;
                case 0:
                    rect.left = box->pos[0] - 20;
                    ggprint8b(&rect, 0, 0x00D30000, "Credit");
                    break;
            }
        }
        g.animationTime += 0.3f;
    } else if (g.game_state == 2) {
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *box = &boxes[i];

            if (i < g.menu_box_amt[g.game_state-1] -1) {
                box->color[0] = 0.0f;
                box->color[1] = 0.0f;
                box->color[2] = 0.0f;
            }
            Rect rect;
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glDisable(GL_BLEND);
            glColor3fv(box->color);
            glTranslatef(box->pos[0], box->pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-box->width, -box->height);
            glVertex2f(-box->width,  box->height);
            glVertex2f( box->width, box->height);
            glVertex2f( box->width, -box->height);
            glEnd();
            glPopMatrix();

            rect.bot = box->pos[1]-7;
            rect.center = 0;
            switch (i)
            {
                case 5:
                    rect.left = box->pos[0] -17;
                    ggprint8b(&rect, 0, 0x00000000, "Back");
                    break;
                case 4:
                    rect.left = box->pos[0] -25;
                    ggprint8b(&rect, 0, 0x00ffffff, "Rj's Level");
                    break;
                case 3:
                    rect.left = box->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00ffffff, "Dason's Level");
                    break;
                case 2:
                    rect.left = box->pos[0] -37;
                    ggprint8b(&rect, 0, 0x00ffffff, "Carlos's Level");
                    break;
                case 1:
                    rect.left = box->pos[0] -33;
                    ggprint8b(&rect, 0, 0x00ffffff, "Sean's Level");
                    break;
                case 0:
                    rect.left = box->pos[0] -42;
                    ggprint8b(&rect, 0, 0x00ffffff, "Caroline's Level");
                    break;
            }
        }
        g.animationTime += 0.6f;
    } 
}
/*----------------------------------------------------*/

/*----------------------------------------------------*/
/* DRAW PLAYER BOX */
void drawPlayerBox() 
{
    /*if (player.color[0] > 0.95f) {
        player.height = 10;
        player.width = 10;
    } else {
        player.height = 5;
        player.width = 5;
    }*/
    player.pos[0] = player.tempx;
    player.pos[1] = player.tempy;
    Player *player_box = &player;
    int width = player_box->width;
    int height = player_box->height;
    glPushMatrix();
    glColor3fv(player_box->color);
    glTranslatef(player_box->pos[0], player_box->pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-width, -height);
    glVertex2f(-width,  height);
    glVertex2f( width,  height);
    glVertex2f( width, -height);
    glEnd();
    glPopMatrix();
}
/*----------------------------------------------------*/

/*----------------------------------------------------*/
/* PLAYER MOVEMENT FUNCTIONS */
void handleKeyPress(XKeyEvent *event) 
{
    KeySym keysym = XLookupKeysym(event, 0);
    if (keysym < 256)
        g.key_states[keysym] = true;
}

void handleKeyRelease(XKeyEvent *event) 
{
    KeySym keysym = XLookupKeysym(event, 0);
    if (keysym < 256)
        g.key_states[keysym] = false;
}

void processMovement() 
{
    if ((!g.key_states[XK_w] || !g.key_states[XK_s]
            || !g.key_states[XK_a] || !g.key_states[XK_d])
       && player.color[0] > -0.05 ) {
        //player.color[0] -= 0.05f;
    }
    if (g.key_states[XK_w])
        if (!player.stop_w) {
            player.tempy += 5;
            //player.color[0] += 0.07f;
        }
    if (g.key_states[XK_a])
        if (!player.stop_a) {
            player.tempx -= 5;
            //player.color[0] += 0.07f;
        }
    if (g.key_states[XK_s])
        if (!player.stop_s) {
            player.tempy -= 5;
            //player.color[0] += 0.07f;
        }
    if (g.key_states[XK_d])
        if (!player.stop_d) {
            player.tempx += 5;
            //player.color[0] += 0.07f;
        }
}
/*----------------------------------------------------*/
