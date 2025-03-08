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

using namespace std;
void dasonEndCredit (void)
{
    Rect title;
    title.bot = 50;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 3: Dason Baird");
}
// macro
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 1000
#define STATE_1 2
#define STATE_2 4
#define STATE_3 2
#define STATE_4 2
//some structures
//

//int GAMECHANGE[3] = {1, 2, 3};

void deleteParticle(int a, int n, Box* particles) 
{
    if (n == 0)
        return;
    particles[a] = particles[--n];

}

void dasonMenuButtonPress(Box* boxes, int x, int y) 
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

    }
}

// Kind of keep
/*
int dasonLevel1Start()
{
    pthread_t p_thread[2];
    init_opengl();
    init_box();
    int value = 0;
    int done = 0;
    //main game loop
    while (!done) {
        //look for external events such as keyboard, mouse.
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            x11.check_mouse(&e);
            done = x11.check_keys(&e);
        }
        pthread_create(&p_thread[0], nullptr, physics, (void *)&value);
        render();
        x11.swapBuffers();
        pthread_join(p_thread[0], nullptr);
    }
    cleanup_fonts();
    return 0;
}
*/

// Keep

int b = 0;
void defineBox(Box* boxes) 
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
void dasonPhysics(Box* particles, Box* boxes, int n)
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
            deleteParticle(i, n, particles);
    }

}
void makeStartScreen() 
{
    float imageAspect = static_cast<float>(g.backgroundImage->width) / g.backgroundImage->height;
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

    glBindTexture(GL_TEXTURE_2D, g.backgroundTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2f(xOffset, yOffset-30);
    glTexCoord2f(0.0, 0.0); glVertex2f(xOffset, yOffset + quadHeight+30); 
    glTexCoord2f(1.0, 0.0); glVertex2f(xOffset + quadWidth, yOffset + quadHeight+30); 
    glTexCoord2f(1.0, 1.0); glVertex2f(xOffset + quadWidth, yOffset-30);

    glEnd();
}
/*
void makeStartScreen(GLuint backgroundTexture) 
{

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor3f(0.2f, 0.2f, 0.6f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,    0);
    glTexCoord2f(0.0f, 0.25f); glVertex2i(0,    g.yres);
    glTexCoord2f(1.0f, 0.25f); glVertex2i(g.xres, g.yres);
    glTexCoord2f(1.0f, 1.0f);  glVertex2i(g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
*/

void drawBoxes(Box* boxes) 
{
    //draw the boxes
    Rect rect;
    int box_text = 1;
    if (g.game_state == 1) {
        defineBox(boxes); 
        for (int i = 0; i <= STATE_1-1; i++) {
            Box *b = &boxes[i];
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
    } else if (g.game_state == 2) {
        defineBox(boxes); 
        for (int i = 0; i <= STATE_2-1; i++) {
            Box *b = &boxes[i];
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
    } 
}
