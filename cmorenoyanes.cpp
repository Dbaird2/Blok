// Author: Carlos Moreno
// Date:   Spring 2025
//
#include <iostream>
#include "functions.h"
#include "fonts.h"
using namespace std;

void carlosEndCredit (void)
{
    Rect credit;
    credit.bot = 66;
    credit.left = 10;
    credit.center = 0;
    ggprint8b(&credit, 16, 0x00ff0000, "Author 3: Carlos Moreno");
}
