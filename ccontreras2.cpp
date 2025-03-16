//Caroline Contreras
//Spring 2025
//3350 Project
//Date: 3/16/25
#include <iostream>
#include "functions.h"
#include "fonts.h"
#include <math.h>
#include <GL/gl.h>
#include "caroline.h"

#include <vector>
#include "Global.h"

using namespace std;
//=================================================================
// GOAL: Render a circle
//=================================================================
const float PI = 3.14159265358979323846f;
class Circle {
	public: 
		float cX;
		float cY;
		float r;
		float circleSegments;
		Circle(){
			cX = 100.0f;
			cY = 100.0f;
			r = 10.0f;
			circleSegments = 20;
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
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(g.xres/2, g.yres/2, 0.0f);

	glBegin(GL_TRIANGLE_FAN);//basically uses a bunch of triangles 
							 //to make a circle
	glVertex2f(circle1.cX, circle1.cY); // Center of the circle
	for (int i = 0; i <= circle1.circleSegments; ++i) {
		float angle = 2.0f * PI * float(i) / float(circle1.circleSegments);
		float x = circle1.r * cos(angle);
		float y = circle1.r * sin(angle);
		glVertex2f(circle1.cX/1.0f + x, circle1.cY/1.0f + y);
//		cout << "angle " << angle << "x " << x << "y " << y << endl;
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
