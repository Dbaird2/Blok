// Name: Dason Baird
// Date: Spring 2025
//
//
#include <iostream>
#include "functions.h"
#include "dbairdheader.h"
#include "fonts.h"
#include "defs.h"
using namespace std;

/*Image::~Image() {
    delete[] data;
}

Image::Image(const char *fname) {
    if (fname[0] == '\0')
        return;

    int ppmFlag = 0;
    char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];

    if (strncmp(name + (slen - 4), ".ppm", 4) == 0)
        ppmFlag = 1;

    if (ppmFlag) {
        strcpy(ppmname, name);
    } else {
        name[slen - 4] = '\0';
        sprintf(ppmname, "%s.ppm", name);
        char ts[100];
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
    }

    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);

        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);

        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);

        int n = width * height * 3;
        data = new unsigned char[n];
        for (int i = 0; i < n; i++)
            data[i] = fgetc(fpi);

        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n", ppmname);
        exit(0);
    }

    if (!ppmFlag)
        unlink(ppmname);
}

void Image::render() {
    printf("Rendering Image (%d x %d)\n", width, height);
    int index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char r = data[index++];
            unsigned char g = data[index++];
            unsigned char b = data[index++];

            // Simple ASCII grayscale render
            unsigned char grayscale = (r + g + b) / 3;
            if (grayscale > 200)
                printf(".");
            else if (grayscale > 150)
                printf("*");
            else if (grayscale > 100)
                printf("+");
            else if (grayscale > 50)
                printf("#");
            else
                printf("@");
        }
        printf("\n");
    }
}

namespace dasonImageClass {
    public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname);
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
//Image img[3] = {


namespace dasonSpriteClass {
    public:
        int onoff;
        int frame;
        double delay;
        Vec pos;
        Image *image;
        GLuint tex;
        struct timespec time;
        Sprite() {
            onoff = 0;
            frame = 0;
            image = NULL;
            delay = 0.1;
        }
};
*/

void dasonEndCredit (void)
{
    Rect title;
    title.bot = 50;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 3: Dason Baird");
}
