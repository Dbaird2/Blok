#ifndef GLOBAL_H
#define GLOBAL_H

#include "Image.h"

class MenuBox {
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
        MenuBox () 
        {
            width = 100;
            height = 50;
            vel[0] = vel[1] = 0.0f;
            color[0] = 0.5f;
            color[1] = 0.0f;
            color[2] = 0.5f;
        }
        MenuBox(int w, int h) 
        {
            MenuBox();
            width = w;
            height = h;
        }
};
extern MenuBox boxes[];
extern MenuBox particles[];
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
            width = 15;
            height = 15;
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
};
extern Box box;

class ImageRenderer {
    public:
        // Background Start Screen
        GLuint backgroundTexture;
        Image *backgroundImage;
};
extern ImageRenderer ren;


class Global {
    public:

        int menu_box_amt[4];
        int game_state;
        int credit;
        float save_w;
        float ydir;
        float ypos[2];
        int red, blue, green;
        int xres, yres;
        float gravity;
        Global() {
            gravity = -0.5f;
            credit = 0;
            game_state = 1;
            /* The number of boxes to produce using game_state  */
            menu_box_amt[0] = {2};
            menu_box_amt[1] = {5};
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

            ren.backgroundImage = nullptr;
        }
} ;

extern Global g;
#endif // GLOBAL_h
