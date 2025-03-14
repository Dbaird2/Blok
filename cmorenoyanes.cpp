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

    float triangleSize = 13.0f;
    float margin = 40.0f;

    float positions[4][2] = {
        {margin, margin},                           // Bottom-left
        {static_cast<float>(g.xres) - margin, margin},     // Bottom-right
        {margin, static_cast<float>(g.yres) - margin},     // Top-left
        {static_cast<float>(g.xres) - margin, static_cast<float>(g.yres) - margin} 
        // Top-right
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(positions[i][0], positions[i][1], 0.0f);
        
        glBegin(GL_TRIANGLES);
        glVertex2f(-triangleSize, -triangleSize);
        glVertex2f(triangleSize, -triangleSize);
        glVertex2f(0.0, triangleSize);
        glEnd();

        glPopMatrix();
    }

    glPopMatrix();
}



