//Caroline Contreras
//Spring 2025
//3350 Project

#include <iostream>
#include "functions.h"
#include "fonts.h"
#include <math.h>
#include <GL/glut.h>
#include "caroline.h"

using namespace std;
//=================================================================
// GOAL: Render a circle
//=================================================================
const double PI = 3.14159265358979323846;
class Circle {
	public: 
		float cX;
		float cY;
		float r;
		float circleSegments;
		Circle(){
			cX = 0.0f;
			cY = 0.0f;
			r = 0.5f;
			circleSegments = 360;
		}
		Circle(float circleX, float circleY, float rad, int cirSegments) {
			cX = circleX;
			cY = circleY;
			r = rad;
			circleSegments = cirSegments;
		}
};

void carolineDrawCircle() {
	glPushMatrix();
	Circle circle1;
	glColor3f(1.0, 0, 0);
	glTranslatef(g.xres/2, g.yres/2, 0.0f);

	glBegin(GL_TRIANGLE_FAN);//basically uses a bunch of triangles 
							 //to make a circle
	glVertex2f(circle1.cX, circle1.cY); // Center of the circle
	for (int i = 0; i <= circle1.circleSegments; ++i) {
		float angle = 2.0 * PI * float(i) / float(circle1.circleSegments);
		float x = circle1.r * cos(angle);
		float y = circle1.r * sin(angle);
		glVertex2f(circle1.cX + x, circle1.cY + y);
	}
	glEnd();
	glPopMatrix();
}
//below is my function for my end credit
void carolineEndCredit (void)
{
	Rect title;
	title.bot = 34;
	title.left = 10;
	title.center = 0;
	ggprint8b(&title, 16, 0x00ff0000, "Author 4: Caroline Contreras");
}
