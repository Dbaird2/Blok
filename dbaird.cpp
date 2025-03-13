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

using namespace std;
void dasonEndCredit (void)
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
                 }
            }
        }

    } else if (g.game_state == 3) {
        //glDeleteTextures(1, &ren.backgroundTexture);
    }
}

int b = 0;

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
    }
    boxes[b].width = 55;
    boxes[b].height = 15;
    boxes[b].pos[0] = g.xres /2.5;
    boxes[b].pos[1] = 3*b * 15 +80;
    ++b;
}

void dasonPhysics(int n)
{
    if (g.game_state == 6) {
        if (player.pos[0] >= 530 && player.pos[0] <= 565 
         && player.pos[1] <= 320 && player.pos[1] - player.height >= 215) {
            if (player.pos[0] >= 555) {
                player.stop_a = 1;
                //cout << player.stop_a << " a" << endl;
            } else {
                player.stop_a = 0;
                //cout << player.stop_a << " a" << endl;
            }
            if (player.pos[1] == 315) {
                player.stop_s = 1;
                //cout << player.stop_s << " s" << endl;
            } else {
                player.stop_s = 0;
                //cout << player.stop_s << " s" << endl;
            }
            if (player.pos[0] >= 540 && player.pos[0] <= 545) {
                player.stop_d = 1;
                //cout << player.stop_d << " d" << endl;
            } else {
                player.stop_d = 0;
                //cout << player.stop_d << " d" << endl;
            }
            if (player.pos[1] <= 225) {
                player.stop_w = 1;
                //cout << player.stop_w << " w" << endl;
            } else {
                player.stop_w = 0;
                //cout << player.stop_w << " w" << endl;
            }
        }
        if (player.pos[0] >= 340 && player.pos[0] <= 545 
         && player.pos[1] <= 295 && player.pos[1] >= 270){
            if (player.pos[1] <= 280) {
                player.stop_w = 1;
            }

        } else {
        }
        /*player.stop_w = 0;
        player.stop_s = 0;
        player.stop_d = 0;
        player.stop_a = 0;*/

    }

}

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
    /*unsigned char *dasonMenuBackground = ren.backgroundImage->buildAlphaData(&img[1]);
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
    /*unsigned char *dasonMazeData = ren.dasonLevelBackgroundImage->buildAlphaData(&img[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, dasonMazeData);
    free(dasonMazeData);*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, ren.dasonLevelBackgroundImage->data);
}

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

        float imageAspect = 
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
    }
}

float animationTime = 0.0f; 
float bounceHeight = 0.5f;

/*----------------------------------------------------*/
/* START MENU BOXEES */
void drawBoxes() 
{
    //draw the boxes
    Rect rect;
    //cout << g.game_state << endl;

    int box_text = 1;
    if (g.game_state == 1) {
        defineBox(); 
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *b = &boxes[i];

            glPushMatrix();
            glColor3fv(boxes->color);
            glTranslatef(b->pos[0], b->pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-b->width, -b->height);
            glVertex2f(-b->width,  b->height);
            glVertex2f( b->width, b->height);
            glVertex2f( b->width, -b->height);
            glEnd();
            glPopMatrix();
            rect.bot = b->pos[1]-7;
            rect.center = 0;
            float bounceOffset = sin(animationTime) * bounceHeight;
            if (i == 0)
                b->pos[1] += bounceOffset;
            if (i == 1) {
                b->pos[1] += bounceOffset+sin(animationTime);
            }
            switch (box_text)
            {
                case 2:
                    rect.left = b->pos[0] - 17;
                    ggprint8b(&rect, 0, 0x00D30000, "Start");
                    break;
                case 1:
                    rect.left = b->pos[0] - 20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Credit");
                    break;
            }
            ++box_text;
        }
        animationTime += 0.3f;
    } else if (g.game_state == 2) {
        //cout << player.stop_w << endl;
        defineBox(); 
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *b = &boxes[i];

            /*float bounceOffset = sin(animationTime) * bounceHeight;
            if (i == 0) {
                bounceHeight = 0.3f;
                b->pos[1] += bounceOffset+sin(animationTime)*(-0.1);
            }
            if (i == 1) {
                bounceHeight = 0.3f;
                b->pos[1] += bounceOffset+sin(animationTime)*0.1;
            }
            if (i == 2 || i == 3 || i == 4) {
                bounceHeight = 0.5f;
                b->pos[1] += bounceOffset+sin(animationTime)*0.1;
            }*/
            glPushMatrix();
            glColor3fv(boxes->color);
            glTranslatef(b->pos[0], b->pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-b->width, -b->height);
            glVertex2f(-b->width,  b->height);
            glVertex2f( b->width, b->height);
            glVertex2f( b->width, -b->height);
            glEnd();
            glPopMatrix();
            rect.bot = b->pos[1]-7;
            rect.center = 0;
            switch (box_text)
            {
                case 6:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Back");
                    break;
                case 5:
                    rect.left = b->pos[0] -25;
                    ggprint8b(&rect, 0, 0x00ffffff, "Rj's Level");
                    break;
                case 4:
                    rect.left = b->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00FFffff, "Dason's Level");
                    break;
                case 3:
                    rect.left = b->pos[0] -30;
                    ggprint8b(&rect, 0, 0x00ffffff, "Carlos's Level");
                    break;
                case 2:
                    rect.left = b->pos[0] -30;
                    ggprint8b(&rect, 0, 0x00ffffff, "Sean's Level");
                    break;
                case 1:
                    rect.left = b->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00FFffff, "Caroline's Level");
                    break;
            }
            ++box_text;
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
    glPushMatrix();
    glColor3fv(player_box->color);
    glTranslatef(player_box->pos[0], player_box->pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-player_box->width, -player_box->height);
    glVertex2f(-player_box->width,  player_box->height);
    glVertex2f( player_box->width, player_box->height);
    glVertex2f( player_box->width, -player_box->height);
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
