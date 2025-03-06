// Name: Dason Baird
// Date: Spring 2025
//
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

using namespace std;
void dasonEndCredit (void)
{
    Rect title;
    title.bot = 50;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 3: Dason Baird");
}
// macro
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 1000
#define STATE_1 2
#define STATE_2 4
#define STATE_3 2
#define STATE_4 2
//some structures
//

int GAMECHANGE[3] = {1, 2, 3};

class Image {   
    public: 
        int width, height;
        unsigned char *data;                                             
        ~Image() { delete [] data; }
        Image(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname)
            int ppmFlag = 0;                
            char name[40]; 
            strcpy(name, fname); 
            int slen = strlen(name);
            char ppmname[80];
            if (strncmp(name+(slen-4), ".ppm", 4) == 0) 
                ppmFlag = 1; 
            if (ppmFlag) { 
                strcpy(ppmname, name); 
            } else { 
                name[slen-4] = '\0';
                //printf("name **%s**\n", name);
                sprintf(ppmname,"%s.ppm", name); 
                //printf("ppmname **%s**\n", ppmname);
                char ts[100];                         
                //system("convert eball.jpg eball.ppm");  
                sprintf(ts, "convert %s %s", fname, ppmname);  
                system(ts);
            }  
            //sprintf(ts, "%s", name); 
            //printf("read ppm **%s**\n", ppmname); fflush(stdout);  
            FILE *fpi = fopen(ppmname, "r");
            if (fpi) {  
                char line[200];  
                fgets(line, 200, fpi);    
                fgets(line, 200, fpi); 
                //skip comments and blank lines    
                while (line[0] == '#' || strlen(line) < 2) 
                    fgets(line, 200, fpi);      
                sscanf(line, "%i %i", &width, &height);   
                fgets(line, 200, fpi);

                //get pixel data
                int n = width * height * 3;
                data = new unsigned char[n];
                for (int i=0; i<n; i++)
                    data[i] = fgetc(fpi);
                fclose(fpi);
            } else {
                printf("ERROR opening image: %s\n",ppmname);
                exit(0);
            }               
            if (!ppmFlag)
                unlink(ppmname);
        }
};
Image img[1] = {
    "./something.png" };
GLuint backgroundTexture;
Image *backgroundImage = NULL;

unsigned char *buildAlphaData(Image *img)                                                       
{
    //add 4th component to RGB stream...  
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    a = *(data+0);
    b = *(data+1);       
    c = *(data+2);       
    *(ptr+0) = a; 
    *(ptr+1) = b;    
    *(ptr+2) = c;
    *(ptr+3) = (a|b|c);                                                                       
    ptr += 4;
    data += 3;
    return newdata;
}

class Box {
    public:
        int id;
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
Box boxes[STATE_2];
Box particles[MAX_PARTICLES];
int n = 0;
int b = 0;
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

void deleteParticle(int a) 
{
    if (n == 0)
        return;
    // optimized 2 lines below
    // particles[a] = particles[n-1];
    // --n;
    particles[a] = particles[--n];

}

class Global {
    public:
        int game_state;
        int credit;
        //        float w;
        float save_w;
        //        float dir;
        float ydir;
        //        float pos[2];
        float ypos[2];
        int red, blue, green;
        int xres, yres;
        Global();
} g;



class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
    public:
        ~X11_wrapper();
        X11_wrapper();
        void set_title();
        bool getXPending();
        XEvent getXNextEvent();
        void swapBuffers();
        void reshape_window(int width, int height);
        void check_resize(XEvent *e);
        void check_mouse(XEvent *e);
        int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void* physics(void *arg);
void render(void);
void init_box(void);
void definBox();


int dasonLevel1Start()
{
    pthread_t p_thread[2];
    init_opengl();
    init_box();
    int value = 0;
    int done = 0;
    //main game loop
    while (!done) {
        //look for external events such as keyboard, mouse.
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            x11.check_mouse(&e);
            done = x11.check_keys(&e);
        }
        pthread_create(&p_thread[0], nullptr, physics, (void *)&value);
        render();
        x11.swapBuffers();
        pthread_join(p_thread[0], nullptr);
    }
    cleanup_fonts();
    return 0;
}

void init_box()
{
    box.pos[0] = g.xres / 2;
    box.pos[1] = g.yres / 2;

}

Global::Global()
{
    credit = 0;
    game_state = 1;
    xres = 900;
    yres = 500;
    //    w = 20.0f;
    //    save_w = w;
    //    dir = 30.0f;
    ydir = 30.0f;
    //    pos[0] = 0.0f+w;
    //    pos[1] = g.yres/2.0f;
    red = 100;
    green = 120;
    blue = 220;

}


void defineBox() 
{
    if (g.game_state == 1) {
        if (b >= STATE_1)
            return;
    } else if (g.game_state == 2) {
        if (b >= STATE_2)
            return;
    }
    boxes[b].width = 55;
    boxes[b].height = 15;
    boxes[b].pos[0] = g.xres /2.5;
    boxes[b].pos[1] = 3*b * 15 +80;
    ++b;
}

X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
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

void X11_wrapper::set_title()
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "3350 Lab-6 - Esc to exit");
}

bool X11_wrapper::getXPending()
{
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
    //Window has been resized.
    g.xres = width;
    g.yres = height;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
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
//-------------------------------------------------------------------------

void X11_wrapper::check_mouse(XEvent *e)
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
            //particle.pos[0] = e->xbutton.x;
            //particle.pos[1] = y;
            int y = g.yres - e->xbutton.y;
            int x = e->xbutton.x;
            if (g.game_state == 1) {
                for (int j = 0; j < STATE_1; j++) {
                    Box *c = &boxes[j];
                    // Colision detection
                    float cx = c->pos[0];
                    float cy = c->pos[1];
                    float ch = c->height;
                    float cw = c->width;
                    if (y <= (cy + ch) &&
                            (x >= (cx - cw)) &&
                            (x <= (cx + cw)) &&
                            (y >= (cy - ch))) {
                        if (j == 1) {
                            g.game_state = 2;
                        } else {
                            g.credit = !g.credit;
                        }
                    }
                }

            } else if (g.game_state == 2) {

            }
            return;
        }
        if (e->xbutton.button==3) {
            //Right button was pressed.
            int y = g.yres - e->xbutton.y;
            for (int i = 0; i < 10; i++) {
                spd = 0;
                makeParticle(e->xbutton.x, y);
            }
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

int X11_wrapper::check_keys(XEvent *e)
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
    glShadeModel(GL_SMOOTH);                                                                     
    glDisable(GL_LIGHTING);    
    glBindTexture(GL_TEXTURE_2D, 0);  
    glEnable(GL_TEXTURE_2D);
    backgroundImage = &img[0];
    glGenTextures(1, &backgroundTexture);
    w = backgroundImage->width; 
    h = backgroundImage->height; 
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, backgroundImage->data);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    // Enables fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

const float GRAVITY = -0.5;

void* physics(void *arg)
{
    for (int i = 0; i < n; i++) {
        Box *p = &particles[i];
        float px = p->pos[0];
        float py = p->pos[1];
        float vx = p->vel[0];
        float vy = p->vel[1];
        p->prev[0] = p->pos[0];
        p->prev[1] = p->pos[1];
        // Move the particle;
        vy += GRAVITY;
        py += vy;
        vx += p->force[0];
        px += vx;
        p->force[1] = 0.0f;
        // Get Box dimensions for detections
        if (g.game_state == 1) {
            for (int j = 0; j < STATE_1; j++) {
                Box *c = &boxes[j];
                // Colision detection
                float cx = c->pos[0];
                float cy = c->pos[1];
                float ch = c->height;
                float cw = c->width;
                if (py <= (cy + ch) &&
                        (px >= (cx - cw)) &&
                        (px <= (cx + cw)) &&
                        (py >= (cy - ch))) {

                    py = p->prev[1];
                    vy = -vy * 0.5;
                    p->force[0] += 0.00001;

                }
            }
        } else if (g.game_state == 2) {
            for (int j = 0; j < STATE_2; j++) {
                Box *c = &boxes[j];
                // Colision detection
                float cx = c->pos[0];
                float cy = c->pos[1];
                float ch = c->height;
                float cw = c->width;
                if (py <= (cy + ch) &&
                        (px >= (cx - cw)) &&
                        (px <= (cx + cw)) &&
                        (py >= (cy - ch))) {

                    py = p->prev[1];
                    vy = -vy * 0.5;
                    p->force[0] += 0.00001;

                }
            }
        }
        p->pos[0] = px;
        p->pos[1] = py;
        p->vel[0] = vx;
        p->vel[1] = vy;
        if (p->pos[1] < -4.0f) 
            deleteParticle(i);
    }

        return 0;
}

void makeStartScreen() 
{

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor3f(0.2f, 0.2f, 0.6f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,    0);
    glTexCoord2f(0.0f, 0.25f); glVertex2i(0,    g.yres);
    glTexCoord2f(1.0f, 0.25f); glVertex2i(g.xres, g.yres);
    glTexCoord2f(1.0f, 1.0f);  glVertex2i(g.xres, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawBoxes() 
{
    //draw the boxes
    Rect rect;
    int box_text = 1;
    if (g.game_state == 1) {
        defineBox(); 
        for (int i = 0; i <= STATE_1-1; i++) {
            Box *b = &boxes[i];
            glPushMatrix();
            glColor3fv(boxes->color);
            glTranslatef(b->pos[0], b->pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-b->width, -b->height);
            glVertex2f(-b->width,  b->height);
            glVertex2f( b->width, b->height);
            glVertex2f( b->width, -b->height);
            glEnd();
            glPopMatrix();
            rect.bot = b->pos[1]-7;
            rect.center = 0;
            switch (box_text)
            {
                case 2:
                    rect.left = b->pos[0] - 17;
                    ggprint8b(&rect, 0, 0x00D30000, "Start");
                    break;
                case 1:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Credit");
                    break;
            }
            ++box_text;
        }
    } else if (g.game_state == 2) {
        defineBox(); 
        for (int i = 0; i <= STATE_2-1; i++) {
            Box *b = &boxes[i];
            glPushMatrix();
            glColor3fv(boxes->color);
            glTranslatef(b->pos[0], b->pos[1], 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(-b->width, -b->height);
            glVertex2f(-b->width,  b->height);
            glVertex2f( b->width, b->height);
            glVertex2f( b->width, -b->height);
            glEnd();
            glPopMatrix();
            rect.bot = b->pos[1]-7;
            rect.center = 0;
            switch (box_text)
            {
                case 4:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00CC8899, "Difficulty");
                    break;
                case 3:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x000000ff, "Easy");
                    break;
                case 2:
                    rect.left = b->pos[0] -23;
                    ggprint8b(&rect, 0, 0x00ffffff, "Normal");
                    break;
                case 1:
                    rect.left = b->pos[0] -20;
                    ggprint8b(&rect, 0, 0x00FF0000, "Hard");
                    break;
            }
            ++box_text;
        }
    } 
}

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

    Rect r;
    // 
    r.bot = g.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "Dason 3350 lab6");
    ggprint8b(&r, 16, 0x00ffff00, "The Waterfall Model");
    //ggprint8b(&r, 16, 0x00ffff00, "f speed up");
    //ggprint8b(&r, 16, 0x00ffff00, "s slow down");
    if (g.credit == 1) {
        dasonEndCredit();
        carlosEndCredit();
        carolineEndCredit();
        rjEndCredit();
        seanEndCredit();
    }

}






