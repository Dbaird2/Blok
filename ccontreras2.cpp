//Caroline Contreras
//Spring 2025

#include <iostream>
#include "functions.h"
#include "fonts.h"
#include <math.h>
#include <GL/glut.h>

const double PI = 3.14159265358979323846;

//below I declare the variables for coordinates x and y, radius
//and the amount of segments the circle will have

float cX = 0.0f;
float cY = 0.0f;
float r = 0.5f;
float circleSegments = 360;

void carolineDrawCircle(float centerX, float centerY, float radius, 
		int segments) {
	glBegin(GL_TRIANGLE_FAN);//basically uses a bunch of triangles 
							 //to make a circle
	glVertex2f(centerX, centerY); // Center of the circle
	for (int i = 0; i <= segments; ++i) {
		float angle = 2.0 * PI * float(i) / float(segments);
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex2f(centerX + x, centerY + y);
	}
	glEnd();
}
//carolineDrawCircle(cX, cY, r, circleSegments);

using namespace std;
//=========================================================
// GOAL: Render a circle
//=========================================================

void carolineEndCredit (void)
{
    Rect title;
    title.bot = 34;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 4: Caroline Contreras");
}
