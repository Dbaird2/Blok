//Sean Toledo
//Date : Spring 2025
//Blok
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

void seanEndCredit (void)
{
    Rect title;
    title.bot = 18;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 5: Sean Toledo");
}
void seanrungame()
{
    if(g.game_state == 4)
    {
        
        drawPlayerBox();

    }
}
