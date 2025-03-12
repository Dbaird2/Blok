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
};
extern MenuBox boxes[];
extern MenuBox particles[];

class Player {
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
        Player() 
        {
            width = 15;
            height = 15;
            vel[0] = vel[1] = 0.0f;
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
        int stop_w; int stop_a; int stop_s; int stop_d;
        int menu_box_amt[4];
        int game_state;
        int credit;
        int tempx;
        int tempy;
        bool key_states[256];
        float save_w;
        float ydir;
        float ypos[2];
        int red, blue, green;
        int xres, yres;
        float gravity;
        Global() {
            stop_w = stop_a = stop_s = stop_d = 0;
            key_states[256] = {false};
            tempx = 450;
            tempy = 200;
            gravity = -0.5f;
            credit = 0;
            game_state = 1;
            /* The number of boxes to produce using game_state  */
            menu_box_amt[0] = {2};
            menu_box_amt[1] = {5};
            xres = 900;
            yres = 500;
            ydir = 30.0f;
            red = 100;
            green = 120;
            blue = 220;

            ren.backgroundImage = nullptr;
        }
} ;

extern Global g;
#endif // GLOBAL_h
