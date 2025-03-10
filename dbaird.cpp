// Name: Dason Baird
// Date: Spring 2025
//
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <random>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <pthread.h>
#include "functions.h"
#include "dbairdheader.h"
#include "Global.h"
#include "Image.h"

using namespace std;
void dasonEndCredit (void)
{
    Rect title;
    title.bot = 50;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 3: Dason Baird");
}
#define STATE_1 2
#define STATE_2 5
#define STATE_3 2
#define STATE_4 2

void deleteParticle(int a, int n) 
{
    if (n == 0)
        return;
    particles[a] = particles[--n];

}

void dasonRenderBackground() 
{
    ren.backgroundImage = &img[0];
    glGenTextures(1, &ren.backgroundTexture);
    int w = ren.backgroundImage->width;
    int h = ren.backgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, ren.backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, ren.backgroundImage->data);
}

void dasonMenuButtonPress(int x, int y) 
{
    if (g.game_state == 1) {
        for (int j = 0; j < STATE_1; j++) {
            Box *c = &boxes[j];
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

    } else if (g.game_state == 2) {
        for (int j = 0; j < STATE_2; j++) {
            Box *c = &boxes[j];
            // Colision detection
            float cx = c->pos[0];
            float cy = c->pos[1];
            float ch = c->height;
            float cw = c->width;
            if (y <= (cy + ch) &&
                    (x >= (cx - cw)) &&
                    (x <= (cx + cw)) &&
                    (y >= (cy - ch))) {
                if (j == 4) {
                    g.game_state = 1;
                }
                if (j == 2) {
                    // HARD
                    g.game_state = 3;
                    glDeleteTextures(1, &ren.backgroundTexture);

                } else if (j == 1) {
                    // NORMAL
                    g.game_state = 3;
                    glDeleteTextures(1, &ren.backgroundTexture);
                } else if (j == 0){
                    // EASY
                    g.game_state = 3;
                    glDeleteTextures(1, &ren.backgroundTexture);

                }
            }
        }

    } else if (g.game_state == 3) {
        glDeleteTextures(1, &ren.backgroundTexture);
    }
}

int b = 0;
void defineBox() 
{
    if (g.game_state == 1) {
        if (b >= STATE_1)
            return;
    } else if (g.game_state == 2) {
        if (b >= STATE_2)
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
    for (int i = 0; i < n; i++) {
        Box *p = &particles[i];
        float px = p->pos[0];
        float py = p->pos[1];
        float vx = p->vel[0];
        float vy = p->vel[1];
        p->prev[0] = p->pos[0];
        p->prev[1] = p->pos[1];
        // Move the particle;
        vy += g.gravity;
        py += vy;
        vx += p->force[0];
        px += vx;
        p->force[1] = 0.0f;
        // Get Box dimensions for detections
        if (g.game_state == 1) {
            for (int j = 0; j < STATE_1; j++) {
                Box *c = &boxes[j];
                // Colision detection
                float cx = c->pos[0];
                float cy = c->pos[1];
                float ch = c->height;
                float cw = c->width;
                if (py <= (cy + ch) &&
                        (px >= (cx - cw)) &&
                        (px <= (cx + cw)) &&
                        (py >= (cy - ch))) {

                    py = p->prev[1];
                    vy = -vy * 0.5;
                    p->force[0] += 0.00001;

                }
            }
        } else if (g.game_state == 2) {
            for (int j = 0; j < STATE_2; j++) {
                Box *c = &boxes[j];
                // Colision detection
                float cx = c->pos[0];
                float cy = c->pos[1];
                float ch = c->height;
                float cw = c->width;
                if (py <= (cy + ch) &&
                        (px >= (cx - cw)) &&
                        (px <= (cx + cw)) &&
                        (py >= (cy - ch))) {

                    py = p->prev[1];
                    vy = -vy * 0.5;
                    p->force[0] += 0.00001;

                }
            }
        }
        p->pos[0] = px;
        p->pos[1] = py;
        p->vel[0] = vx;
        p->vel[1] = vy;
        if (p->pos[1] < -4.0f) 
            deleteParticle(i, n);
    }

}
void makeStartScreen() 
{
    float imageAspect = 
        static_cast<float>(ren.backgroundImage->width) 
        / ren.backgroundImage->height;
    float screenAspect = static_cast<float>(g.xres) / g.yres;
    float quadWidth = g.xres;
    float quadHeight = g.yres;

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
    glTexCoord2f(1.0, 0.0); glVertex2f(xOffset + quadWidth, yOffset + quadHeight+30); 
    glTexCoord2f(1.0, 1.0); glVertex2f(xOffset + quadWidth, yOffset-30);

    glEnd();
}
float animationTime = 0.0f; 
float bounceHeight = 0.5f;

void drawBoxes() 
{
    //draw the boxes
    Rect rect;

    int box_text = 1;
    if (g.game_state == 1) {
        defineBox(); 
        for (int i = 0; i <= STATE_1-1; i++) {
            Box *b = &boxes[i];

            float bounceOffset = sin(animationTime) * bounceHeight;
            if (i == 0)
                b->pos[1] += bounceOffset;
            if (i == 1)
                b->pos[1] += bounceOffset+sin(animationTime);


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
                case 2:
                    rect.left = b->pos[0] - 17;
                    ggprint8b(&rect, 0, 0x00D30000, "Start");
                    break;
                case 1:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Credit");
                    break;
            }
            ++box_text;
        }
                animationTime += 0.3f;
    } else if (g.game_state == 2) {
        defineBox(); 
        for (int i = 0; i <= STATE_2-1; i++) {
            Box *b = &boxes[i];
            
            float bounceOffset = sin(animationTime) * bounceHeight;
            if (i == 0)
                b->pos[1] += bounceOffset+sin(animationTime)*0.2;
            if (i == 1)
                b->pos[1] += bounceOffset+sin(animationTime)*0.1;
            if (i == 2)
                b->pos[1] += bounceOffset+sin(animationTime)*0.1;
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
                case 5:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Back");
                    break;
                case 4:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Difficulty");
                    break;
                case 3:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x000000ff, "Easy");
                    break;
                case 2:
                    rect.left = b->pos[0] -23;
                    ggprint8b(&rect, 0, 0x00ffffff, "Normal");
                    break;
                case 1:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00FF0000, "Hard");
                    break;
            }
            ++box_text;
        }
                animationTime += 0.6f;
    } 
}
