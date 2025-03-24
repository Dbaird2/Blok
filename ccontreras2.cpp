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
using namespace std;

//=========================================================
#ifdef USE_OPENAL_SOUND
#include "src/setup/oal.h"
#include </usr/include/AL/alut.h>
#endif 


#ifdef USE_OPENAL_SOUND

//void Sound::initSound()
void initSound()
{
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	alGetError();
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	
	//Buffer holds the sound information.
	//FIX THIS CODE BELOW
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./winningScreenAudio.wav");
	winScreen = alutCreateBufferFromFile("./src/sounds/");
	
	//Source refers to the sound.
	/*ALunit alSource;*/
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceCoin, AL_GAIN, 1.0f);
	alSourcef(g.alSourceCoin, AL_PITCH, 1.0f);
	alSourcei(g.alSourceCoin, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	#endif //USE_OPENAL_SOUND
}

//void Sound::cleanupSound()
void cleanupSound()
{
        //First delete the source.
        alDeleteSources(1, &alSource);
        //Delete the buffer.
        alDeleteBuffers(1, &alBuffer);
        //Close out OpenAL itself.
        //Get active context.
        ALCcontext *Context = alcGetCurrentContext();
        //Get device for active context.
        ALCdevice *Device = alcGetContextsDevice(Context);
        //Disable context.
        alcMakeContextCurrent(NULL);
        //Release context(s).
        alcDestroyContext(Context);
        //Close device.
        alcCloseDevice(Device);
}

//void Sound::playSound(ALuint source)
void playSound(ALuint source)
{
#ifdef USE_OPENAL_SOUND
	alSourcePlay(source);
#endif
}
#endif //USE_OPENAL_SOUND
//=========================================================


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
