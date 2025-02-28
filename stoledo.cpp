//Sean Toledo
//Date : Spring 2025
//Blok
//
#include<iostream>
#include"fonts.h"
#include"function.h"
using namespace std;

void seanEndCredit (void)
{
    Rect title;
    title.bot = 18;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 5: Sean Toledo");
}
