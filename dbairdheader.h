#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <iostream>

void dasonEndCredit();

namespace dasonImageClass {
    class Image {
    public:
        int width, height;
        unsigned char *data;

        ~Image() { delete[] data; }

        Image(const char *fname);

        void printInfo() {
            std::cout << "Image Width: " << width << ", Height: " << height << std::endl;
        }
    };
}

#endif
