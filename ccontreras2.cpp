//Caroline Contreras
//Spring 2025
//last upload to github: 3/4/25
#include <iostream>
#include "functions.h"
#include "fonts.h"
using namespace std;

void carolineEndCredit (void)
{
    Rect title;
    title.bot = 34;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 4: Caroline Contreras");
}
