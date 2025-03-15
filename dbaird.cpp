// Name: Dason Baird
// Date: Spring 2025
//
//
using namespace std;
#include <iostream>
#include <random>
#include <GL/glx.h>
#include "fonts.h"
#include "Global.h"
#include "dbairdheader.h"

int WALL_AMOUNT = 0;

void dasonLoadStruct(Grid grid[], int height[], int width[], 
        int x[], int y[], int size) 
{
    WALL_AMOUNT = size;
    for (int i = 0; i < size; i++) {
        grid[i].height = height[i];
        grid[i].width = width[i];
        grid[i].x = x[i];
        grid[i].y = y[i];
    }
}

void dasonEndCredit(void)
{
    Rect title;
    title.bot = 50;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 3: Dason Baird");
}
/*
void deleteParticle(int a, int n) 
{
    if (n == 0)
        return;
    particles[a] = particles[--n];

}
*/
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
                    glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 1) {
                    // SEAN
                    g.game_state = 4;
                    glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 2) {
                    // CARLOS
                    g.game_state = 3;
                    glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 3) {
                    // DASON
                    g.game_state = 6;
                    glDeleteTextures(1, &ren.backgroundTexture);
                    init_dasonMazePlayer();
                } else if (j == 4) {
                    // RJ
                    g.game_state = 5;
                    glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 5) {
                    g.game_state = 1;
                    b = 0;
                 }
            }
        }

    } else if (g.game_state == 3) {
        //glDeleteTextures(1, &ren.backgroundTexture);
    }
}


void init_dasonMazePlayer() 
{
    player.tempx = g.xres/2+75;
    player.tempy = g.yres/2+5;
    player.width = 5;
    player.height = 5;
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
    boxes[b].color[0] = 1.0f;  // Example: Red
    boxes[b].color[1] = 0.0f;
    boxes[b].color[2] = 1.0f;

    /*cout << "b " << b;
    cout << " Color1 " << boxes[b].color[0];
    cout << " Color2 " << boxes[b].color[1];
    cout << " Color3 " << boxes[b].color[2] << endl;*/
    ++b;
}

void dasonPhysics(int size)
{
    
    if (g.game_state == 6) {
        glClear(GL_COLOR_BUFFER_BIT);
        for (int i = 0; i < WALL_AMOUNT; i++) {
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
#ifdef MAP_HELP
                /*cout << box_top << "top " << box_bot << " bot" << endl;
                cout << box_left << " left " << box_right << " right\n" << endl;*/
                cout << w->pos[1] << " " << w->pos[0] << endl;
#endif
            } /*else { p->stop_a = 0; }*/
        }
    }

}

/* COMBINE INTO ONE FUNCTION */
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
    /*unsigned char *dasonMenuBackground = 
     * ren.backgroundImage->buildAlphaData(&img[1]);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
     GL_RGB, GL_UNSIGNED_BYTE, dasonMenuBackground);
     free(dasonMenuBackground);*/

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
    } else if (g.game_state == 6) { 
        //glDeleteTextures(1, &ren.backgroundTexture);

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

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, ren.dasonLevelBackgroundTexture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 1.0); glVertex2f(xOffset, yOffset);
        glTexCoord2f(0.0, 0.0); glVertex2f(xOffset, yOffset + quadHeight); 
        glTexCoord2f(1.0, 0.0); glVertex2f(xOffset + quadWidth, 
        yOffset + quadHeight); 
        glTexCoord2f(1.0, 1.0); glVertex2f(xOffset + quadWidth, yOffset);

        glEnd();
        glPopMatrix();
        */
    }
}

float animationTime = 0.0f; 
float bounceHeight = 0.5f;

void dasonDrawWalls(Grid grid[], int size) 
{
    for ( int i = 0; i < WALL_AMOUNT; i++) {

        glPushMatrix();
        glColor3ub(50, 120, 220);
        Wall *w = &walls[i];

        w->width = grid[i].width;
        w->height = grid[i].height;
        w->pos[0] = grid[i].x;
        w->pos[1] = grid[i].y;

        int width = w->width;
        int height = w->height;
        //w->pos[0] = g.xres/2 + i*6*width;
        //w->pos[1] = g.yres/2;
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

/*----------------------------------------------------*/
/* START MENU BOXEES */
        int j = 0;
void drawBoxes() 
{
    //draw the boxes
    Rect rect;

    defineBox(); 
    if (g.game_state == 1) {
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *box = &boxes[i];

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glPushMatrix();
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
            float bounceOffset = sin(animationTime) * bounceHeight;
            if (i == 0)
                box->pos[1] += bounceOffset;
            if (i == 1) {
                box->pos[1] += bounceOffset+sin(animationTime);
            }
            switch (i)
            {
                case 1:
                    rect.left = box->pos[0] - 17;
                    ggprint8b(&rect, 0, 0x00D30000, "Start!");
                    break;
                case 0:
                    rect.left = box->pos[0] - 20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Credit");
                    break;
            }
        }
        animationTime += 0.3f;
    } else if (g.game_state == 2) {
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *box = &boxes[i];

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glDisable(GL_BLEND);
            glColor3f(1.0f, 1.0f, 1.0f);
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
                    rect.left = box->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00ffffff, "Back");
                    break;
                case 4:
                    rect.left = box->pos[0] -25;
                    ggprint8b(&rect, 0, 0x00ffffff, "Rj's Level");
                    break;
                case 3:
                    rect.left = box->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00FFffff, "Dason's Level");
                    break;
                case 2:
                    rect.left = box->pos[0] -30;
                    ggprint8b(&rect, 0, 0x00ffffff, "Carlos's Level");
                    break;
                case 1:
                    rect.left = box->pos[0] -30;
                    ggprint8b(&rect, 0, 0x00ffffff, "Sean's Level");
                    break;
                case 0:
                    rect.left = box->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00FFffff, "Caroline's Level");
                    break;
            }
        }
        animationTime += 0.6f;
    } 
}
/*----------------------------------------------------*/

/*----------------------------------------------------*/
/* DRAW PLAYER BOX */
void drawPlayerBox () {
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
    if (g.key_states[XK_w])
        if (!player.stop_w) 
            player.tempy += 5;
    if (g.key_states[XK_a])
        if (!player.stop_a)
            player.tempx -= 5;
    if (g.key_states[XK_s])
        if (!player.stop_s)
            player.tempy -= 5;
    if (g.key_states[XK_d])
        if (!player.stop_d)
            player.tempx += 5;
    //cout << g.stop_d << endl;
}
/*----------------------------------------------------*/
