//
//modified by: Dason B
//date: Spring 2025
//
//original author: Gordon Griesel
//date:            2025
//purpose:         OpenGL sample program
//
//This program needs some refactoring.
//We will do this in class together.
//
// 1/31/2025 add some text
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

// macro
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 1000
#define MAX_BOXES 5
//some structures
//
Global g;
Box boxes[MAX_BOXES];
Box particles[MAX_PARTICLES];
Image img[1] = {
    "./background.png" 
};
/*
GLuint backgroundTexture;
Image *backgroundImage = NULL;
*/

/*
class Box {
    public:
        int width;
        int height;
        float pos[2];
        float prev[2];
        float vel[2];
        float force[2];
        float color[3];
        char text[100];
        Box () 
        {
            width = 100;
            height = 50;
            vel[0] = vel[1] = 0.0f;
            color[0] = 0.5f;
            color[1] = 0.0f;
            color[2] = 0.5f;
        }
        Box(int w, int h) 
        {
            Box();
            width = w;
            height = h;
        }
} box, particle(4,4);
*/
//Box box;
int n = 0;
//int b = 0;
float spd = 0;



void makeParticle(int x, int y) 
{
    if (n >= MAX_PARTICLES)
        return;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-3.0, 6.0);
    double random_num = dis(gen);

    particles[n].width = 4;
    particles[n].height = 4;
    particles[n].pos[0] = x;
    particles[n].pos[1] = y;
    particles[n].vel[0] = random_num * 0.3f;
    particles[n].vel[1] = random_num * 0.1f - 1.0;
    ++n;
}
/*
void deleteParticle(int a) 
{
    if (n == 0)
        return;
    // optimized 2 lines below
    // particles[a] = particles[n-1];
    // --n;
    particles[a] = particles[--n];

}
*/
class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
    public:
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            int w = g.xres, h = g.yres;
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                cout << "\n\tcannot connect to X server\n" << endl;
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                cout << "\n\tno appropriate visual found\n" << endl;
                exit(EXIT_FAILURE);
            } 
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            XSetWindowAttributes swa;
            swa.colormap = cmap;
            swa.event_mask =
                ExposureMask | KeyPressMask | KeyReleaseMask |
                ButtonPress | ButtonReleaseMask |
                PointerMotionMask |
                StructureNotifyMask | SubstructureNotifyMask;
            
            win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
                    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
            set_title();
            glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
        }
        void set_title() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "3350 Lab-6 - Esc to exit");
        }
        bool getXPending() {
            //See if there are pending events.
            return XPending(dpy);
        }
        XEvent getXNextEvent() {
            //Get a pending event.
            XEvent e;
            XNextEvent(dpy, &e);
            return e;
        }
        void swapBuffers() {
            glXSwapBuffers(dpy, win);
        }
        void reshape_window(int width, int height) {
            //Window has been resized.
            g.xres = width;
            g.yres = height;
            //
            glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, g.xres, 0, g.yres, -1, 1);
        }
        void check_resize(XEvent *e) {
            //The ConfigureNotify is sent by the
            //server if the window is resized.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            if (xce.width != g.xres || xce.height != g.yres) {
                //Window size did change.
                reshape_window(xce.width, xce.height);
            }
        }
} x11;

/*void init_box()
{
    box.pos[0] = g.xres/2;
    box.pos[1] = g.yres/2;
}*/
//Function prototypes
void init_opengl(void);
void* physics(void *arg);
void render(void);
//void init_box(void);
//void makeBox();
void check_mouse(XEvent *e);
int check_keys(XEvent *e);

int main()
{
    pthread_t p_thread[2];
    init_opengl();
    //init_box();
    int value = 0;
    int done = 0;
    //main game loop
    while (!done) {
        //look for external events such as keyboard, mouse.
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            check_mouse(&e);
            done = check_keys(&e);
        }
        pthread_create(&p_thread[0], nullptr, physics, (void *)&value);
        render();
        x11.swapBuffers();
        pthread_join(p_thread[0], nullptr);
    }
    cleanup_fonts();
    return 0;
}

//-------------------------------------------------------------------------

void check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;

    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
            e->type != ButtonPress &&
            e->type != MotionNotify) {
        //This is not a mouse event that we care about.
        return;
    }
    //
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button was pressed.
            //int y = g.yres - e->xbutton.y;
            int y = g.yres - e->xbutton.y;
            int x = e->xbutton.x;
            //particle.pos[0] = e->xbutton.x;
            //particle.pos[1] = y;
            dasonMenuButtonPress(x, y);
            for (int i = 0; i < 10; i++) {
                spd = 0;
                makeParticle(e->xbutton.x, y);
            }
            return;
        }
        if (e->xbutton.button==3) {
            //Right button was pressed.
            return;
        }
    }
    if (e->type == MotionNotify) {
        //The mouse moved!
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            if (savex > e->xbutton.x) {
                spd = -(savex + e->xbutton.x)/450;
            } else {
                spd = (savex + e->xbutton.x)/450;
            }
            savex = e->xbutton.x;
            savey = e->xbutton.y;


            //makeParticle(savex, g.yres-savey);
            //Code placed here will execute whenever the mouse moves.


        }
    }
}

int check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
        switch (key) {
            case XK_a:
                //the 'a' key was pressed
                break;
            case XK_f:
                //the 'f' key was pressed
                break;
            case XK_s:
                break;

            case XK_Escape:
                //Escape key was pressed
                return 1;
        }
    }
    return 0;
}

void init_opengl(void)
{
    //OpenGL initialization
    int w,h;
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow texture maps

    // allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    g.backgroundImage = &img[0];
    glGenTextures(1, &g.backgroundTexture);
    w = g.backgroundImage->width; 
    h = g.backgroundImage->height; 
    glBindTexture(GL_TEXTURE_2D, g.backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, g.backgroundImage->data);

}


void* physics(void *arg)
{
    dasonPhysics(n);
    /*for (int i = 0; i < n; i++) {
        Box *p = &particles[i];
        float px = p->pos[0];
        float py = p->pos[1];
        float vx = p->vel[0];
        float vy = p->vel[1];
        p->prev[0] = p->pos[0];
        p->prev[1] = p->pos[1];
        // Move the particle;
        p->force[1] += GRAVITY;
        p->vel[1] += p->force[1];
        p->pos[1] += p->vel[1];

        p->vel[0] += p->force[0];
        p->pos[0] += p->vel[0];
        vy += GRAVITY;
        py += vy;
        vx += p->force[0];
        px += vx;
        p->force[1] = 0.0f;
        // Get Box dimensions for detections
        for (int j = 0; j < MAX_BOXES; j++) {
            Box *c = &boxes[j];
            //p->force[0] = 0.0f;
            // Colision detection
            float cx = c->pos[0];
            float cy = c->pos[1];
            float ch = c->height;
            float cw = c->width;
            if (p->pos[1] <= (c->pos[1] + c->height) &&
                    (p->pos[0] >= (c->pos[0] - c->width)) &&
                    (p->pos[0] <= (c->pos[0] + c->width)) &&
                    (p->pos[1] >= (c->pos[1] - c->height))) {

                //p->pos[0] = p->prev[0];
                p->pos[1] = p->prev[1];
                p->vel[1] = -p->vel[1] * 0.5;
                //printf("p->vel[1]: %f\n", p->vel[1]);
                if (py <= (cy + ch) &&
                        (px >= (cx - cw)) &&
                        (px <= (cx + cw)) &&
                        (py >= (cy - ch))) {

                    //p->pos[0] = p->prev[0];
                    py = p->prev[1];
                    vy = -vy * 0.5;
                    p->force[0] += 0.00001;

                }
            }
            p->pos[0] = px;
            p->pos[1] = py;
            p->vel[0] = vx;
            p->vel[1] = vy;
            if (p->pos[1] < -4.0f) 
                deleteParticle(i);
        }

    }
    */
    return 0;
}
//void startScreenTexture(Image *backgroundImage, 
void render()
{

    //clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    makeStartScreen();

    // DRAW ALL BOXES
    drawBoxes();

    // DRAW ALL PARTICLES
    for (int i = 0; i < n; i++ ) {
        glPushMatrix();
        glColor3ub(50, 120, 220);
        Box *p = &particles[i];
        // 

        glTranslatef(p->pos[0], p->pos[1], 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-p->width, -p->height);
        glVertex2f(-p->width,  p->height);
        glVertex2f( p->width,  p->height);
        glVertex2f( p->width, -p->height);
        glEnd();
        glPopMatrix();
    }
     if (g.credit == 1) {
        dasonEndCredit();
        carlosEndCredit();
        carolineEndCredit();
        rjEndCredit();
        seanEndCredit();
    }
}

