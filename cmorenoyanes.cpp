/**********************************
 * Author: Carlos Moreno          *
 * Date:   Spring 2025            *
 * Assignment: blok               *
 **********************************/

#include "fonts.h"
#include "cmorenoyanesheader.h"
#include <math.h>

using namespace std;
float circleOffset = 0.0f;
bool direction = true;

void carlosEndCredit (void)
{
    Rect credit;
    credit.bot = 66;
    credit.left = 10;
    credit.center = 0;
    ggprint8b(&credit, 16, 0x00ff0000, "Author 2: Carlos Moreno");
}

void renderInstructions()
{
    Rect title;
    title.bot = 450;
    title.left = 800;
    title.center = 0;
    ggprint8b(&title, 16, 0x00000000, "A - Move Left");
    ggprint8b(&title, 16, 0x00000000, "W - Move Up");
    ggprint8b(&title, 16, 0x00000000, "S - Move Down");
    ggprint8b(&title, 16, 0x00000000, "D - Move Right");
    ggprint8b(&title, 16, 0x00000000, "C - Credits");
    ggprint8b(&title, 16, 0x00000000, "Q - Quit Level");
}

void drawTriangles ()
{
    // Draw the triangles
    glPushMatrix();
    glColor3f(1.0, 0, 0);
    glDisable(GL_DEPTH_TEST);

    float triangleSize = 13.0f;
    float margin = 40.0f;

    float positions[4][2] = {
        // Bottom-left
        {margin, margin},
        // Bottom-right
        {static_cast<float>(g.xres) - margin, margin},
        // Top-left
        {margin, static_cast<float>(g.yres) - margin},     
        // Top-right
        {static_cast<float>(g.xres) - margin, 
            static_cast<float>(g.yres) - margin} 
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

void drawCircles ()
{
    // Move the circles back and forth
    if (direction)
        circleOffset += 5.0f;
    else
        circleOffset -= 5.0f;

    if (circleOffset > 50.0f || circleOffset < -50.0f)
        direction = !direction;

    glPushMatrix();
    glColor3f(1.0, 0, 0);

    float radius = 13.0f;
    float circleMargin = 80.0f;
    float positions[4][2] = {
        {static_cast<float>(g.xres / 2 - circleMargin),
            static_cast<float>(g.yres / 2)},
        {static_cast<float>(g.xres / 2 + circleMargin),
            static_cast<float>(g.yres / 2)},
        {static_cast<float>(g.xres / 2),
            static_cast<float>(g.yres / 2 - circleMargin)},
        {static_cast<float>(g.xres / 2),
            static_cast<float>(g.yres / 2 + circleMargin)}
    };

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(positions[i][0] + circleOffset, positions[i][1], 0.0f);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0);
        for (int j = 0; j <= 360; j += 10) {
            float angle = j * M_PI / 180.0f;
            glVertex2f(cos(angle) * radius, sin(angle) * radius);
        }
        glEnd();

        glPopMatrix();
    }

    glPopMatrix();
}

void renderCarlosLevel()
{
    drawCircles();
    drawTriangles();

}

