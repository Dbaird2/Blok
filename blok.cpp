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
#include "rbarreyroheader.h"
#include "caroline.h"
#include "Global.h"
#include "Image.h"
#include "stoledoheader.h"
// macro
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 1000
#define MAX_BOXES 7

//some structures
//
Global g;
Wall walls[100];
Wall growing_boxes[10];
Grid growing_box[15];
ImageRenderer ren;
MenuBox boxes[MAX_BOXES];
Player player;
Image img[2] = {
    "./background.png",
    "./capybara.gif"
};
void init_opengl(void);
void physics(void);
void render(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);

int n = 0;
float spd = 0;
/*
class X11_wrapper {
    private:
    public:
        Display *dpy;
        Window win;
        GLXContext glc;
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
            XStoreName(dpy, win, "World's Easiest Game");
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
            cout << w << " " << h << endl;
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
*/
class X11_wrapper {
private:
        //X Windows variables
public:
        Display *dpy;
        Window win;
        GLXContext glc;
        X11_wrapper() {
                GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
                //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
                XSetWindowAttributes swa;
                setup_screen_res(g.xres, g.yres);
                dpy = XOpenDisplay(NULL);
                if (dpy == NULL) {
                        printf("\n\tcannot connect to X server\n\n");
                        exit(EXIT_FAILURE);
                }
                Window root = DefaultRootWindow(dpy);
                XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
                if (vi == NULL) {
                        printf("\n\tno appropriate visual found\n\n");
                        exit(EXIT_FAILURE);
                }
                Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
                swa.colormap = cmap;
                swa.event_mask = ExposureMask |
                                                KeyPressMask |
                                                KeyReleaseMask |
                                                ButtonPressMask |
                                                ButtonReleaseMask |
                                                PointerMotionMask |
                                                StructureNotifyMask |
                                                SubstructureNotifyMask;
                win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
                                                                vi->depth, InputOutput, vi->visual,
                                                                CWColormap | CWEventMask, &swa);
                set_title();
                glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
                glXMakeCurrent(dpy, win, glc);

        }
        ~X11_wrapper() {
                XDestroyWindow(dpy, win);
                XCloseDisplay(dpy);
        }
        void set_title() {
                //Set the window title bar.
                XMapWindow(dpy, win);
                XStoreName(dpy, win, "World's Easiest Game, Blok");
        }
        void setup_screen_res(const int w, const int h) {
                g.xres = w;
                g.yres = h;
        }
        void reshape_window(int width, int height) {
                //window has been resized.
                g.xres = width;
                g.yres = height;
                setup_screen_res(width, height);
                //
                glViewport(0, 0, (GLint)width, (GLint)height);
                glMatrixMode(GL_PROJECTION); glLoadIdentity();
                glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                glOrtho(0, g.xres, 0, g.yres, -1, 1);
                set_title();
                init_opengl();
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
        bool getXPending() {
                return XPending(dpy);
        }
        XEvent getXNextEvent() {
                XEvent e;
                XNextEvent(dpy, &e);
                return e;
        }
        void swapBuffers() {
                glXSwapBuffers(dpy, win);
        }
} x11;
//Function prototypes

int main()
{
    //pthread_t p_thread[2];
    init_opengl();
    //int value = 0;
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
        //pthread_create(&p_thread[0], nullptr, physics, (void *)&value);
        physics();
        render();
        x11.swapBuffers();
        //pthread_join(p_thread[0], nullptr);
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
            if(e->xbutton.button == 7) {
				carolineDisplayWinScreen();
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
                init_dasonMazePlayer();
            case XK_v:
                g.vsync ^= 1;
                //vertical synchronization
                //https://github.com/godotengine/godot/blob/master/platform/
                //x11/context_gl_x11.cpp
                static PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = NULL;
                glXSwapIntervalEXT =
                    (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB(
                            (const GLubyte *)"glXSwapIntervalEXT");
                GLXDrawable drawable = glXGetCurrentDrawable();
                if (g.vsync) {
                    glXSwapIntervalEXT(x11.dpy, drawable, 1);
                } else {
                    glXSwapIntervalEXT(x11.dpy, drawable, 0);
                }
                break;

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
    glShadeModel( GL_SMOOTH );
    //Do this to allow texture maps

    // allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();

    dasonMazeLevelBackground();
    dasonRenderBackground();
}


void physics(void)
{
    if (g.game_state == 6)
        dasonPhysics(58, 10, 1, growing_box);
    //return 0;
}
int i = 0;
void render()
{

    //clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    if (g.game_state == 6) {

        makeStartScreen();
        dasonMazeRender();
    }
    if (g.game_state > 2)  {
        drawPlayerBox();
#ifdef MAP_HELP
        if ( i % 10  == 0) {
            cout << "x " << player.pos[0] << " y " << player.pos[1] << endl;
        }
        i++;
#endif
    }
    // DRAW ALL BOXES
    if ((g.game_state == 1) || (g.game_state == 2)) {
        makeStartScreen();
        drawBoxes();
    }
    if (g.game_state == 3) {
        drawTriangles();
        drawCircles();
    }
    if(g.game_state == 7) {
        //carolineDrawCircle();
    	carolineDisplayWinScreen();
	}
    seanrungame();
    rbarreyroRunGame();

    if (g.credit == 1) {
        dasonEndCredit();
        carlosEndCredit();
        carolineEndCredit();
        rjEndCredit();
        seanEndCredit();
    }
}

