#ifndef GLOBAL_H
#define GLOBAL_H

#include "Image.h"
#include <GL/glx.h>

class MenuBox {
    public:
        int id;
        int width;
        int height;
        float pos[2];
        float color[3];
        char text[100];
        MenuBox () 
        {
            width = 5;
            height = 30;
            pos[0] = 400;
            pos[1] = 200;
            color[0] = 0.5f;
            color[1] = 0.0f;
            color[2] = 1.0f;
        }
};
extern MenuBox boxes[];
extern MenuBox particles[];

class Wall {
    public:
        int id;
        int width;
        int height;
        float pos[2];
        float color[3];
        Wall () 
        {
            width = 5;
            height = 50;
            color[0] = 0.5f;
            color[1] = 0.0f;
            color[2] = 1.0f;
        }
        Wall (int x, int y) 
        {
            width = x;
            height = y;
        }
};
extern Wall walls[];


class Player {
    public:
        int id;
        int stop_w; int stop_a; int stop_s; int stop_d;
        int tempx;
        int tempy;
        int width;
        int height;
        float pos[2];
        //float prev[2];
        //float vel[2];
        //float force[2];
        float color[3];
        char text[100];
        Player() 
        {
            stop_w = stop_a = stop_s = stop_d = 0;
            tempx = 450;
            tempy = 200;
            width = 15;
            height = 15;
            //vel[0] = vel[1] = 0.0f;
            color[0] = 0.5f;
            color[1] = 0.0f;
            color[2] = 0.5f;
        }
        Player(int w, int h) 
        {
            Player();
            width = w;
            height = h;
        }
};
extern Player player;




class ImageRenderer {
    public:
        // Background Start Screen
        GLuint backgroundTexture;
        Image *backgroundImage;
        
        // Maze level
        GLuint dasonLevelBackgroundTexture;
        Image *dasonLevelBackgroundImage;

};
extern ImageRenderer ren;


class Global {
    public:
        int menu_box_amt[4];
        int game_state;
        int credit;
        bool key_states[256];
        int xres, yres;
        float gravity;
        Global() {
            key_states[256] = {false};
            gravity = -0.5f;
            credit = 0;
            game_state = 1;
            /* The number of boxes to produce using game_state 
             * Will move to Menu Box */
            menu_box_amt[0] = {2};
            menu_box_amt[1] = {6};
            xres = 900;
            yres = 500;

            ren.backgroundImage = nullptr;
            ren.dasonLevelBackgroundImage = nullptr;
        }
} ;

extern Global g;
#endif // GLOBAL_H
