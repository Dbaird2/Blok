//
//modified by: Dason B
//Caroline C
//Carlos M
//Sean
//RJ
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
#include "cmorenoyanesheader.h"
#include "Global.h"
#include "Image.h"
#include "stoledoheader.h"
// macro
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 1000
#define MAX_BOXES 6
//some structures
//
Global g;
#define GRID_SIZE 10 
Grid grid[GRID_SIZE];
Wall walls[100];
ImageRenderer ren;
MenuBox boxes[MAX_BOXES];
Player player;
Image img[2] = {
    "./background.png",
    "./wip.png"
};

int height[10] = {25, 5, 20, 25, 30, 35, 40, 5, 10, 20};
int width[10] = {5, 25, 10, 60, 10, 20, 90, 15, 10, 20};
int x[10] = {20, 0, 20, 250, 300, 350, 400, 500, 150, -200};
int y[10] = {100, 100, 200, 250, 300, 350, 40, 50, 100, 200};


int n = 0;
float spd = 0;

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
            XSelectInput(dpy, win, KeyPressMask | KeyReleaseMask | 
                    ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
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
        void setupScreenRes(const int w, const int h) {
            g.xres = w;
            g.yres = h;
        }
        void reshape_window(int width, int height) {
            //Window has been resized.
            setupScreenRes(width, height);
            glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, g.xres, 0, g.yres, -1, 1);
            set_title();
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

//Function prototypes
void init_opengl(void);
void* physics(void *arg);
void render(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);

int main()
{
    dasonLoadStruct(grid, height, width, x, y, GRID_SIZE);
    pthread_t p_thread[2];
    init_opengl();
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
        processMovement();
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
            int y = g.yres - e->xbutton.y;
            int x = e->xbutton.x;
            if (g.game_state < 3) {
                dasonMenuButtonPress(x, y);
            } else {
                // MAIN MENU OVER GAME START   

            }


            /*for (int i = 0; i < 10; i++) {
                spd = 0;
                makeParticle(e->xbutton.x, y);
            }*/
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
#define KEY_MAX 256  

int check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    if (e->type == KeyPress || e->type == KeyRelease) {
        switch (e->type) {
            case KeyPress:
                //Any Key is Pressed
                //Function to add pressed button into array keysym
                //This allows multiple button presses at the sametime
                handleKeyPress(&e->xkey);
                break;
            case KeyRelease:
                handleKeyRelease(&e->xkey);
                break;
        }
    }
    if (e->type == KeyPress ) {
        int key = XLookupKeysym(&e->xkey, 0);
        switch (key) {
            case XK_Escape:
                return 1;
            case XK_c:
                g.credit = !g.credit;
                break;
            case XK_l:
                g.game_state = 6;
                glDeleteTextures(1, &ren.backgroundTexture);
                init_dasonMazePlayer();
                //init_opengl();
        }
    }
    return 0;
}

void init_opengl(void)
{
    //OpenGL initialization
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

    dasonMazeLevelBackground();
    dasonRenderBackground();
}


void* physics(void *arg)
{
    dasonPhysics(n);
    return 0;
}
int i = 0;
void render()
{

    //clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    makeStartScreen();

    if (g.game_state == 6) {
        dasonDrawWalls(grid, GRID_SIZE);
    }
    if (g.game_state > 2)  {
        drawPlayerBox();
#ifdef MAP_HELP
        if ( i % 10  == 0) {
            cout << player.pos[0] << " " << player.pos[1] << endl;
        }
        i++;
#endif
    }
    // DRAW ALL BOXES
    if ((g.game_state == 1) || (g.game_state == 2))
        drawBoxes();
    if (g.game_state == 3) {
        drawTriangles();
    }
    seanrungame();

     if (g.credit == 1) {
        dasonEndCredit();
        carlosEndCredit();
        carolineEndCredit();
        rjEndCredit();
        seanEndCredit();
    }
}

