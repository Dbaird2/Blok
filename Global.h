#ifndef GLOBAL_H
#define GLOBAL_H

#include "Image.h"
#include <GL/glx.h>

class MenuBox {
    public:
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
            color[0] = 1.0f;
            color[1] = 1.0f;
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
            height = 5;
            pos[0] = 0;
            pos[1] = 0;
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
        }
        Wall (int x, int y) 
        {
            width = x;
            height = y;
        }
};
extern Wall walls[];
extern Wall growing_boxes[];


class Player {
    public:
        int big;
        int death_count, dead;
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
            death_count = 0;
            dead = 0;
            stop_w = stop_a = stop_s = stop_d = 0;
            tempx = 400;
            tempy = 200;
            width = 15;
            height = 15;
            //vel[0] = vel[1] = 0.0f;
            big = 0;
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
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
        int vsync;
        float gravity;
        float grow_animation;
        float animationTime;
        float bounceHeight;

        Global() {
            key_states[256] = {false};
            //gravity = -0.5f;
            credit = 0;
            vsync = 1;
            game_state = 1;
            grow_animation = 0.0f;
            animationTime = 0.0f;
            bounceHeight = 0.5;
            /* The number of boxes to produce using game_state 
             * Will move to Menu Box */
            menu_box_amt[0] = {2};
            menu_box_amt[1] = {7};
            /* WINDOW SIZE */
            xres = 900;
            yres = 500;

            ren.backgroundImage = nullptr;
            ren.dasonLevelBackgroundImage = nullptr;

        }
} ;

extern Global g;
#endif // GLOBAL_H
