/**********************************
 * Author: Carlos Moreno          *
 * Date:   Spring 2025            *
 * Assignment: cmorenoyanes.cpp   *
 **********************************/

#include "fonts.h"
#include "cmorenoyanesheader.h"

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
    // Draw the triangles
    glPushMatrix();
    glColor3f(1.0, 0, 0);
    glDisable(GL_DEPTH_TEST);
    glTranslatef(g.xres/2, g.yres/2, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-17.32, -10.0);
    glVertex2f(17.32, -10.0);
    glVertex2f(0.0, 20.0);
    glEnd();
    glPopMatrix();
}



