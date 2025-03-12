/**********************************
 * Author: Carlos Moreno          *
 * Date:   Spring 2025            *
 * Assignment: cmorenoyanes.cpp   *
 **********************************/

#include <iostream>
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
#include "cmorenoyanesheader.h"
#include "Global.h"
#include "Image.h"

using namespace std;

void carlosEndCredit (void)
{
    Rect credit;
    credit.bot = 66;
    credit.left = 10;
    credit.center = 0;
    ggprint8b(&credit, 16, 0x00ff0000, "Author 2: Carlos Moreno");
}

/*void defineTriangle()
{
    if (g.game_state >= 2)
}*/

void drawTriangles ()
{
    //draw the triangles
    if (g.game_state == 3) {
        //defineTriangle();
        glPushMatrix();
        glColor3f(1.0, 0, 0);
        glDisable(GL_DEPTH_TEST);
        // glTranslatef(t->pos[0], t->pos[1], t->post[2], 0.0f);
        glTranslatef(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(100, 100, 0);
        glVertex3f(150, 100, 0);
        glVertex3f(100, 150, 0);
        glEnd();
        glPopMatrix();
    }
}



