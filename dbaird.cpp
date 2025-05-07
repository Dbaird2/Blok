// Name: Dason Baird
// Date: Spring 2025
//
//
/* If you want growing boxes in your levels
 * Then in your own file do
 * Step 1) make a struct Grid name[size],
 * Step 2) call dasonLoadStruct(name_of_grid, height_of_boxes, width_of_boxes,
 *                      xpos_of_boxes, ypos_of_boxes, size);
 * Step 3) call dasonDrawWalls(name_of_grid, size_of_struct);
 *      OR call dasonDrawGrowingBoxes(name_of_grid, size_of_struct);
 * Step 4) lastly call dasonPhysics(amount_of_walls, amount_of_growing_boxes
 *                          , 1, name_of_grid);      
 * If you ALSO want WALLS then do Step 1, Step 2, & Step 3 but for walls.
 *
 * Note: If you choose to not have growing boxes then just do
 * Step 1, 2, 3 for Walls, and call 
 * dasonPhysics(amount_of_walls, 0, 0, NULL)
 *
 * If you want a player box call drawPlayerBox
 * If you want to change it's position change player.tempx or player.tempy
 * for size do player.width, player.height
 */
using namespace std;
//#include <algorithm>
#include <iomanip>
//#include <sstream>
#include <random>
//#include <omp.h>
#include <chrono>
#include <cstdio>
#include "fonts.h"
#include "Global.h"
#include "dbairdheader.h"
#include "rbarreyroheader.h"
#include "stoledoheader.h"
#include "caroline.h"
#include "cmorenoyanesheader.h"
/*---------------------------------------------------------------------------*/
Teleportal portals[4];
Entity dason_goal = {360, 490, 50, 15, 0, 0};
int enemy_size = 17;
vector<vector<double>> color_vector(enemy_size, vector<double>(2));
Entity dason_enemies[20] = {
    /* Bottom Vertical Enemies */
    {650.0, 100.0, 20.0, 20.0, 10.0, 1},
    {550.0, 100.0, 20.0, 20.0, 10.0, 1},
    {600.0, 100.0, 20.0, 20.0, 10.0, -1},
    {500.0, 100.0, 20.0, 20.0, 10.0, -1},
    /* Top Vertical Enemies */
    {650.0, 300.0, 20.0, 20.0, 10.0, -1},
    {550.0, 300.0, 20.0, 20.0, 10.0, -1},
    {500.0, 250.0, 20.0, 20.0, 10.0, 1},
    {600.0, 250.0, 20.0, 20.0, 10.0, 1},
    /* Right Horizontal Enemies */
    {800, 420, 20, 20, 10, 1},
    {800, 370, 20, 20, 10, 1},
    {800, 320, 20, 20, 10, 1},
    {800, 270, 20, 20, 10, 1},
    {800, 220, 20, 20, 10, 1},
    {800, 170, 20, 20, 10, 1},
    {800, 120, 20, 20, 10, 1},
    {800, 70, 20, 20, 10, 1},
    /* Single Vertical Bottom Left */
    {795, 20, 20, 20, 10, 1}
};

/* --------------- MAP WALL STRUCTURES --------------------------------------*/
#define DASON_GRID_SIZE 58
Grid dason_grid[DASON_GRID_SIZE];
int dason_height[DASON_GRID_SIZE] = {
    10, 10, /*25*/25, 5,/*60*/ 65, 5,/*60*/ 65, 5, /*45*/50, 50, 250, 5, 5, 250,
    115, 5, 130, 5, 90, 5, 5, 5, 80, 5, 60/**/, 5, 5,
    60, 65, 5, 60, 70, 5, 90, 5, 60, 50, 5, 5, 50, 5, 5,
    60, /**/100, 30, 200, 5, 30, 40, 5, 200, /**/
    40, 40, 40, 40, 40, 40, 40};
int dason_width[DASON_GRID_SIZE] = {
    250, 170, 5, 200, 5, 75, 5, 50, 5, 5, 5, 250, 175, 5,
    5, 95, 5, 20, 5, 120, 30, 25, 5, 20, 5/**/, 30, 90, 5,
    5, 35, 5, 5, 90, 5, 50, 5, 5, 15, 45, 5, 45, 60, 5,
    /**/5, 100, 5, 90, 95, 5, 60, 20, /**/
    5, 5, 5, 5, 5, 5, 5};
int dason_x[DASON_GRID_SIZE] = {
    250, 725, 555, 485, 290, 220, 250, 205, 195, 150, 5, 660,
    185, 895, 105, 200, 55, 80, 335, 220, 400, 35, 375, 360,
    105/**/, 70, 100, 160, 195, 265, 335, 235, 150, 295, 350,
    375, 410, 395, 425, 465, 425, 440, 445, /**/495, 590, 720,
    625, 595, 670, 725, 805, /**/
    635, 600, 565, 530, 495, 460, 425};
int dason_y[DASON_GRID_SIZE] = {
    0, 0, 25, 45, 110, 175, 70, 135, 55, 90, 260, 495, 495,
    250, 155, 220, 170, 160, 180, 270, 95, 335, 180, 265,
    365/**/, 385, 430, 335, 370, 310, 335, 385, 460, 405, 430,
    370, 290, 335, 210, 100, 150, 370, 275, /**/ 270, 140,
    210, 245, 345, 415, 450, 255, /**/
    415, 415, 415, 415, 415, 415, 415};
// Vertical: H:250 x:5 y:260
#define NUM_COINS 3
static vector<Coin> dasonCoins;
int collected_coins;
int coin_score;
/*---------------------------------------------------------------------------*/

//Grid growing_box[10];
int growing_height[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int growing_width[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5,5};
int growing_x[10] = {40, 350, 110, 200, 320, 440, 160, 230, 410, 200};
int growing_y[10] = {40, 15, 80, 300, 220, 340, 60, 450, 250, 200};


using _clock        = std::chrono::steady_clock;
using _elapsed      = std::chrono::duration<double>;
using _time         = std::chrono::time_point<_clock, _elapsed>;

inline time_t current_time_t()
{
    return chrono::system_clock::to_time_t(chrono::system_clock::now());
}
auto t1 = _clock::now();

void getRandomColors(vector<vector<double>>& vec)
{
    srand(time(NULL));
    for (auto& row: vec) {
        // Red
        //row[0] = (double)rand()/(double)RAND_MAX*1.0f;
        row[0] = 1.0f;
        // Green
        //row[1] = (double)rand()/(double)RAND_MAX*1.0f;
        row[1] = 0.0f;
        // Blue
        //row[2] = (double)rand()/(double)RAND_MAX*1.0f;
        row[2] = 0.0f;
    }
}

void drawStar()
{
    const int num_points = 5;
    float angle = M_PI / 2.0f;
    float outer_radius = 40.0f;
    float inner_radius = 15.0f;
    float cy = 400.0f;
    float cx = 220.0f;

    int valid_stars = 0;
    for (int i = 0; i < 5; i++) {
        if (g.amt_of_stars[i] > 0) {
            valid_stars++;
        }
    }

    for (int j = 0; j < valid_stars; j++) {
        cx = 100.0f*(float)j+220;
        glPushMatrix();
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.75f, 0.0f);
        glVertex2f(cx, cy); 
        for (int i = 0; i <= num_points * 2; i++) {
            float r = (i % 2 == 0) ? outer_radius : inner_radius;
            float a = angle + i * M_PI / num_points;
            float x = cx + cos(a) * r;
            float y = cy + sin(a) * r;
            glVertex2f(x, y);
        }
        glEnd();
        glPopMatrix();
    }
}

void dasonTimerOut(void (*func)()) 
{
    player.death_count++;
    func();
}

void dasonTimer(int y, int x, float time_out, void(*func)())
{
    if (player.dead == 1) {
        t1 = _clock::now();
        player.dead = 0;
    }
    _elapsed diff = _clock::now() - t1;

    float time = time_out - diff.count();
    Rect title;
    title.bot = y;
    title.left = x;
    title.center = 0;
    ggprint8b(&title, 0, 0x00ffd700, "Timer %.1fs", time);
    if (time <= 0) {
        dasonTimerOut(func);
        t1 = _clock::now();
    }
}

void dasonPlayerDeath(int x_spawn, int y_spawn)
{
    player.death_count++;
    player.dead = 1;

}

void dasonEndCredit(void)
{
    Rect dason_credit;
    dason_credit.bot = 50;
    dason_credit.left = 10;
    dason_credit.center = 0;
    ggprint8b(&dason_credit, 16, 0x00ff0000, "Author 3: Dason Baird");
}

void quadDraw(int width, int height) 
{
    glBegin(GL_QUADS);
    glVertex2f(-width, -height);
    glVertex2f(-width,  height);
    glVertex2f( width,  height);
    glVertex2f( width, -height);
    glEnd();
    glPopMatrix();
}

int b = 0;
void init_dasonMazePlayer();
void dasonMenuButtonPress(int x, int y) 
{
    if (g.game_state == 0) {

        /* Start/Credits button Collision Detection */
    } else if (g.game_state == 1) {
        for (int j = 0; j < g.menu_box_amt[g.game_state-1]; j++) {
            MenuBox *c = &boxes[j];
            // Colision detection
            float cx = c->pos[0];
            float cy = c->pos[1];
            float ch = c->height;
            float cw = c->width;
            if (y <= (cy + ch) &&
                    (x >= (cx - cw)) &&
                    (x <= (cx + cw)) &&
                    (y >= (cy - ch))) {
                if (j == 2) {
                    g.game_state = 2;
                    b = 0;
                } 
                if (j == 1)
                    g.game_state = 10;
                if (j == 0) 
                    g.credit = !g.credit;
                
            }
        }
        /* Difficulty button Collision Detection */
    } else if (g.game_state == 2) {
        player.stop_w = 0;
        for (int j = 0; j <= g.menu_box_amt[g.game_state-1]; j++) {
            MenuBox *c = &boxes[j];
            float cx = c->pos[0];
            float cy = c->pos[1];
            float ch = c->height;
            float cw = c->width;
            // Colision detected
            if (y <= (cy + ch) &&
                    (x >= (cx - cw)) &&
                    (x <= (cx + cw)) &&
                    (y >= (cy - ch))) {
                if (j == 0) {
                    // CAROLINE
                    g.game_state = 7;
                    carolineLevel();
                    //carolineDisplayWinScreen();
                } else if (j == 1) {
                    // SEAN
                    g.game_state = 4;
                    seanLevel();;
                } else if (j == 2) {
                    // CARLOS
                    g.game_state = 3;
                    carlosMaze();
                } else if (j == 3) {
                    // DASON
                    g.game_state = 6;
                    init_dasonMazePlayer();
                } else if (j == 4) {
                    // RJ
                    g.game_state = 5;
                    russLevel();
                } else if (j == 6) {
                    g.game_state = 1;
                    b = 0;
                } else if (j == 5) {
                    g.game_state = rand() % 6 + 2;
                }
            }
        }
    }
}

void renderDeathCount()
{
    Rect title;
    title.bot = 490;
    title.left = 0;
    title.center = 0;
    ggprint8b(&title, 0, 0x00ff0000, "Death count: %i", player.death_count);

}

void renderFps() 
{
    Rect fps;
    fps.bot = 490;
    fps.left = 150;
    fps.center = 0;
    ggprint8b(&fps, 0, 0x0044ff00, "FPS: %.0f", g.fps);
}

void init_dasonMazePlayer() 
{
    /* coin_score = 0 fixes goal not being automatically ready */
    coin_score = 0;
    g.hard_mode = rand() % 2;

    dasonCoins.clear();
    t1 = _clock::now();
    getRandomColors(color_vector);
    /* GROWING BOX ENEMIES */
    dasonLoadStruct(growing_box, growing_height, growing_width, 
            growing_x, growing_y, 10);
    /* WALLS */
    dasonLoadStruct(dason_grid, dason_height, dason_width, 
            dason_x, dason_y, DASON_GRID_SIZE);
    float cx[3] = {220.0f, 265.0f, 150.0f};
    float cy[3] = {25.0f, 350.0f, 250.0f};
    for (int i = 0; i < NUM_COINS; i++) {
        float theta= (2.0f * M_PI / 8) * i;
        dasonCoins.push_back({cx[i], cy[i], false, cy[i], 
                20.0f, 2.0f + i * 0.2f, theta, 10});
    }
    // portal id, PI, cX, cY, r, segments;
    for (int i = 0; i < 4; i++) {
        portals[i].r = 5.0f;
    }
    portals[0].cX = 660;
    portals[0].cY = 295; 
    portals[1].cX = 750;
    portals[1].cY = 30; 
    portals[2].cX = 470;
    portals[2].cY = 20; 
    portals[3].portal_id = 2; 
    portals[3].cX = 300;
    portals[3].cY = 20; 
    portals[2].portal_id = 2; 



    player.tempx = 530;
    player.tempy = 10;
    player.width = 5;
    player.height = 5;
}

void dasonMazeRender()
{

    for (int i = 0; i < enemy_size; i++)
        SeanDrawRect(dason_enemies[i].x, dason_enemies[i].y, 
                dason_enemies[i].width, dason_enemies[i].height, 
                color_vector[i][0], color_vector[i][1], color_vector[i][2]);
    SeanDrawRect(dason_goal.x, dason_goal.y, 
            dason_goal.width, dason_goal.height, 0, 1, 0);

    dasonDrawGrowingBoxes(growing_box, 10);
    dasonDrawWalls(dason_grid, DASON_GRID_SIZE);
    carolineDrawCircle(portals, 4);
    renderDeathCount(); 
    RB_DrawCoins(dasonCoins);
    dasonTimer(490, 830, 240.0, init_dasonMazePlayer);
}

void defineBox() 
{
    if (g.game_state == 1) {
        if (b >= g.menu_box_amt[g.game_state-1])
            return;
    } else if (g.game_state == 2) {
        if (b >= g.menu_box_amt[g.game_state-1]) 
            return;
    } else if (g.game_state > 2) {
        return;
    }
    boxes[b].width = 55;
    boxes[b].height = 15;
    boxes[b].pos[0] = g.xres /2.5;
    boxes[b].pos[1] = 3*b * 15 +80;
    boxes[b].color[0] = 1.0;  
    boxes[b].color[1] = 0.0;
    boxes[b].color[2] = 0.0;
    ++b;
}

/*--------------------------------------------------------------------------*/

void dasonLoadStruct(Grid grid[], int height[], int width[], 
        int x[], int y[], int size) 
{
    for (int i = 0; i < size; i++) {
        grid[i].height = height[i];
        grid[i].width = width[i];
        grid[i].x = x[i];
        grid[i].y = y[i];
        grid[i].r = rand() % 256;
        grid[i].g = rand() % 256;
        grid[i].b = rand() % 256;
    }
}

void dasonDrawGrowingBoxes(Grid grid[], int size)
{
    for ( int i = 0; i < size; i++) {

        glPushMatrix();
        glColor3ub(grid[i].r, grid[i].g,grid[i].b);
        Wall *b = &growing_boxes[i];

        b->width = grid[i].width;
        b->height = grid[i].height;
        b->pos[0] = grid[i].x;
        b->pos[1] = grid[i].y;
        b->color[0] = grid[i].r;
        b->color[1] = grid[i].g;
        b->color[2] = grid[i].b;

        int width = b->width;
        int height = b->height;
        glTranslatef(b->pos[0], b->pos[1], 0.0f);
        quadDraw(width, height);
    }
}

void dasonDrawWalls(Grid grid[], int size)
{
    for ( int i = 0; i < size; i++) {

        glPushMatrix();
        if (size == 10)
            glColor3ub(grid[i].r, grid[i].g,grid[i].b);
        else
            glColor3ub(0, 0, 0);
        Wall *w = &walls[i];

        w->width = grid[i].width;
        w->height = grid[i].height;
        w->pos[0] = grid[i].x;
        w->pos[1] = grid[i].y;
        w->color[0] = grid[i].r;
        w->color[1] = grid[i].g;
        w->color[2] = grid[i].b;

        int width = w->width;
        int height = w->height;
        glTranslatef(w->pos[0], w->pos[1], 0.0f);
        quadDraw(width, height);
    }
}

void growingBoxPhysics(int size, Grid grid[]) 
{
    //cout << diff.count() << endl;
    if (size < 0) 
        return;
    for (int i = 0; i < size; i++) {
        Player *p = &player;
        int box_x = grid[i].x;
        int box_y = grid[i].y;
        int box_h = grid[i].height;
        int box_w = grid[i].width;


        float bounceOffset = sin(g.grow_animation) * g.bounceHeight;
        grid[i].width += bounceOffset+sin(g.grow_animation)*0.1f*8.0;
        grid[i].height += bounceOffset+sin(g.grow_animation)*0.1f*8.0;
        g.grow_animation += 0.005f;

        int x_offset = p->width;
        int y_offset = p->height;
        int box_top = box_y + box_h;
        int box_bot = box_y - box_h;
        int box_left = box_x - box_w;
        int box_right = box_x + box_w;
        /*
         * Bug found w/ bad fix
         * If box is inverted check for different values
         * Did not need this if statement
        if (bounceOffset < 0) {
        */
        if ((p->pos[1] >= box_top - y_offset)
                && (p->pos[1] <= box_bot + y_offset)
                && (p->pos[0] <= box_left + x_offset) 
                && (p->pos[0] >= box_right - x_offset)) {
            dasonPlayerDeath(530, 10);
            init_dasonMazePlayer();
            /* OLD CODE OPTIMIZED */
            /*
             p->tempx = 530;
             p->tempy = 10;
             */
            /* bug found
             * caused incorrect spawn placements when bottom collision happens
             p->tempy = 510;
             */
        }
        if ((p->pos[1] <= box_top + y_offset)
                && (p->pos[1] >= box_bot - y_offset)
                && (p->pos[0] >= box_left - x_offset) 
                && (p->pos[0] <= box_right + x_offset)) {
            dasonPlayerDeath(530, 10);
            init_dasonMazePlayer();
        }
    }
}

int lower[2] = {55, 250};
int upper[2] = {215, 350};
int hright = 1620;
int hleft = 720;

void dasonPhysics(int wall_size, int growing_size, 
        int growing_enemy_check, Grid grid[])
{
    if (g.game_state == 6) {
        SeanEnemiesVertical(0, 4, upper[0], lower[0], dason_enemies);
        SeanEnemiesVertical(4, 8, upper[1], lower[1], dason_enemies);
        SeanEnemiesHorizontal(8, enemy_size-1, hright, hleft, dason_enemies);
        SeanEnemiesVertical(enemy_size-1, enemy_size, 200, 5, dason_enemies);
        for (int i = 0; i < enemy_size; i++) {
            if (SeanCheckCollision(dason_enemies[i])) {
                dasonPlayerDeath(530, 10);
                init_dasonMazePlayer();
            }
        }
        coin_score = RB_CheckCoinCollection(dasonCoins, coin_score);
        collected_coins = coin_score;

        if (SeanCheckCollision(dason_goal) &&
                collected_coins >= NUM_COINS * 10 ) {
            fill(walls, walls + 100, Wall());
            player.death_count = 0;
            g.game_state = 9;
            dason_goal = {360, 490, 50, 15, 0, 0};
            dasonCoins.clear();
            if (g.amt_of_stars[0] < 1)
                g.amt_of_stars[0]++;

        }
        RB_UpdateCoins(dasonCoins);
        isCircleCollidingWithSquare(portals, 4);
    }

    //glClear(GL_COLOR_BUFFER_BIT);

    if (growing_enemy_check) 
        growingBoxPhysics(growing_size, grid);
    for (int i = 0; i < wall_size; i++) {
        Wall *w = &walls[i];
        Player *p = &player;

        int x_offset = p->width;
        int y_offset = p->height;
        int box_top = w->pos[1] + w->height;
        int box_bot = w->pos[1] - w->height;
        int box_left = w->pos[0] - w->width;
        int box_right = w->pos[0] + w->width;

        if ((p->pos[1] <= box_top + y_offset)
                && (p->pos[1] >= box_bot - y_offset)
                && (p->pos[0] >= box_left - x_offset) 
                && (p->pos[0] <= box_right + x_offset)) {
            if (p->pos[1] >= box_top - y_offset/4) {
                p->tempy += 5;
                player.stop_w = 1;
            }
            else if (p->pos[1] <= box_bot+y_offset/10) {
                p->tempy -= 5;
                player.stop_s = 1;
            }
            else if (p->pos[0] >= box_right) {
                p->tempx += 5;
                player.stop_a = 1;
            }
            else if (p->pos[0] <= box_left) {
                p->tempx -= 5;
                player.stop_d = 1;
            }
        } else {
            player.stop_w = 0;
            player.stop_d = 0;
            player.stop_s = 0;
            player.stop_a = 0;
        }
    }
}
/*--------------------------------------------------------------------------*/

/*  */
void dasonRenderBackground() 
{
    ren[0].backgroundImage = &img[0];
    glGenTextures(1, &ren[0].backgroundTexture);
    int w = ren[0].backgroundImage->width;
    int h = ren[0].backgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, ren[0].backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, ren[0].backgroundImage->data);
    
    ren[2].backgroundImage = &img[3];
    glGenTextures(1, &ren[2].backgroundTexture);
    int w2 = ren[2].backgroundImage->width;
    int h2 = ren[2].backgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, ren[2].backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w2, h2, 0,
                GL_RGB, GL_UNSIGNED_BYTE, ren[2].backgroundImage->data);

}
void dasonMazeLevelBackground() 
{
    ren[1].backgroundImage = &img[1];
    glGenTextures(1, &ren[1].backgroundTexture);
    int w = ren[1].backgroundImage->width;
    int h = ren[1].backgroundImage->height;
    glBindTexture(GL_TEXTURE_2D, ren[1].backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*unsigned char *dasonMazeData = 
     * ren.dasonLevelBackgroundImage->buildAlphaData(&img[1]);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
     GL_RGB, GL_UNSIGNED_BYTE, dasonMazeData);
     free(dasonMazeData);*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, ren[1].backgroundImage->data);
}
/*---------------------------------------------------*/

void makeStartScreen(int image_number) 
{
    glClear(GL_COLOR_BUFFER_BIT);
    float quadWidth = g.xres;
    float quadHeight = g.yres;
    float screenAspect = static_cast<float>(g.xres) / g.yres;
    float imageAspect = 
        static_cast<float>(ren[image_number].backgroundImage->width) 
        / ren[image_number].backgroundImage->height;

    // Adjust width/height based on aspect ratio
    if (screenAspect > imageAspect) {
        quadWidth = g.yres * imageAspect;
    } else {
        quadHeight = g.xres / imageAspect;
    }

    // Center the image in the viewport
    float xOffset = (g.xres - quadWidth) / 2.0;
    float yOffset = (g.yres - quadHeight) / 2.0;

    glBindTexture(GL_TEXTURE_2D, ren[image_number].backgroundTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 1.0); glVertex2f(xOffset, yOffset-30);
    glTexCoord2f(0.0, 0.0); glVertex2f(xOffset, yOffset + quadHeight+30); 
    glTexCoord2f(1.0, 0.0); glVertex2f(xOffset + quadWidth, 
            yOffset + quadHeight+30); 
    glTexCoord2f(1.0, 1.0); glVertex2f(xOffset + quadWidth, yOffset-30);

    glEnd();
    glPopMatrix();
}


/*----------------------------------------------------*/
/* START MENU BOXEES */
int j = 0;
void drawBoxes() 
{
    //draw the boxes

    defineBox(); 
    if (g.game_state == 1) {
        for (int i = 0; i < g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *box = &boxes[i];
            if (i < g.menu_box_amt[g.game_state-1] -1) {
                box->color[0] = 0.0f;
                box->color[1] = 0.0f;
                box->color[2] = 0.0f;
            } else {
                box->color[0] = 1.0f;
            }

            Rect rect;
            glPushMatrix();
            glColor3fv(box->color);
            glDisable(GL_BLEND);
            glTranslatef(box->pos[0], box->pos[1], 0.0f);
            quadDraw(box->width, box->height);

            rect.bot = box->pos[1]-7;
            rect.center = 0;

            /* Bouncing effect for start menu */
            float bounceOffset = sin(g.animationTime) * g.bounceHeight;
            if (i == 0)
                box->pos[1] += bounceOffset;
            if (i == 1)
                box->pos[1] += bounceOffset;
            if (i == 2) 
                box->pos[1] += bounceOffset+sin(g.animationTime);

            switch (i)
            {
                case 2:
                    rect.left = box->pos[0] - 17;
                    ggprint8b(&rect, 0, 0x00ffffff, "Start!");
                    break;
                case 1:
                    rect.left = box->pos[0] - 20;
                    ggprint8b(&rect, 0, 0x00D30000, "Help");
                    break;
                case 0:
                    rect.left = box->pos[0] - 20;
                    ggprint8b(&rect, 0, 0x00D30000, "Credit");
                    break;
            }
        }
        g.animationTime += 0.3f;
    } else if (g.game_state == 2) {
        for (int i = 0; i <= g.menu_box_amt[g.game_state-1]; i++) {
            MenuBox *box = &boxes[i];

            if (i < g.menu_box_amt[g.game_state-1] -1) {
                box->color[0] = 0.0f;
                box->color[1] = 0.0f;
                box->color[2] = 0.0f;
            }
            Rect rect;
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glDisable(GL_BLEND);
            glColor3fv(box->color);
            glTranslatef(box->pos[0], box->pos[1], 0.0f);
            quadDraw(box->width, box->height);
            rect.bot = box->pos[1]-7;
            rect.center = 0;
            switch (i)
            {
                case 6:
                    rect.left = box->pos[0] -17;
                    ggprint8b(&rect, 0, 0x00000000, "Back");
                    break;
                case 5:
                    rect.left = box->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00ffffff, "Random Level");
                    break;
                case 4:
                    rect.left = box->pos[0] -25;
                    ggprint8b(&rect, 0, 0x00ffffff, "Rj's Level");
                    break;
                case 3:
                    rect.left = box->pos[0] -40;
                    ggprint8b(&rect, 0, 0x00ffffff, "Dason's Level");
                    break;
                case 2:
                    rect.left = box->pos[0] -37;
                    ggprint8b(&rect, 0, 0x00ffffff, "Carlos's Level");
                    break;
                case 1:
                    rect.left = box->pos[0] -33;
                    ggprint8b(&rect, 0, 0x00ffffff, "Sean's Level");
                    break;
                case 0:
                    rect.left = box->pos[0] -42;
                    ggprint8b(&rect, 0, 0x00ffffff, "Caroline's Level");
                    break;
            }
        }
        g.animationTime += 0.6f;
    } 
            drawStar();
}
/*----------------------------------------------------*/

/*----------------------------------------------------*/
void drawPlayerBox(int hard_mode) 
{
    if (hard_mode) {
        if (player.color[0] >= 1.0f && player.big == 0) {
            player.height = player.height + 5;
            player.width = player.width + 5;
            player.big = 1;
        } else if (player.big == 1 && player.color[0] < 0.7f) {
            player.height = player.height - 5;
            player.width = player.width - 5;
            player.big = 0;
        }
    }
    player.pos[0] = player.tempx;
    player.pos[1] = player.tempy;
    Player *player_box = &player;
    int width = player_box->width;
    int height = player_box->height;
    glPushMatrix();
    glColor3fv(player_box->color);
    glTranslatef(player_box->pos[0], player_box->pos[1], 0.0f);
    quadDraw(width, height);
}
/*----------------------------------------------------*/

/*----------------------------------------------------*/
/* PLAYER MOVEMENT FUNCTIONS */
void handleKeyPress(XKeyEvent *event) 
{
    KeySym keysym = XLookupKeysym(event, 0);
    if (keysym < 256)
        g.key_states[keysym] = true;
}

void handleKeyRelease(XKeyEvent *event) 
{
    KeySym keysym = XLookupKeysym(event, 0);
    if (keysym < 256)
        g.key_states[keysym] = false;
}

bool value;
void dasonKeyChecks () 
{
    // HANDLES Q PRESS IN GAME LEVEL
    if (g.key_states[XK_q] && g.game_state >= 3 && g.game_state <= 7) {
        // Part of Algorithm library
        // It will reset all of walls back to Constructor values
        g.game_state = 99;
        g.hard_mode = 0;
        fill(walls, walls + 100, Wall());
        player.death_count = 0;
        //g.game_state = 2;
    }
    if (g.key_states[XK_f] && g.game_state == 0)
        g.game_state = 1;
    value = (g.fps > 0.0f) ? true : false;
    if (g.key_states[XK_minus] && value) 
        g.fps -= 5.0f;
    if (g.key_states[XK_equal]) 
        g.fps += 5.0f;
    if (g.key_states[XK_z])
        g.show_fps = !g.show_fps;


}

void processMovement() 
{

    if (g.game_state > 2) {
        if ((g.key_states[XK_w] || g.key_states[XK_s]
                    || g.key_states[XK_a] || g.key_states[XK_d])
                && player.color[0] < 1.05f ) {
            player.color[0] += 0.02f;
        } else if (player.color[0] > -0.05f) {
            player.color[0] -= 0.05f;
        }
        if (g.key_states[XK_w] && player.tempy < g.yres)
            if (!player.stop_w) {
                player.tempy += 5;
            }
        if (g.key_states[XK_a] && player.tempx > 5)
            if (!player.stop_a) {
                player.tempx -= 5;
            }
        if (g.key_states[XK_s] && player.tempy > 5)
            if (!player.stop_s) {
                player.tempy -= 5;
            }
        if (g.key_states[XK_d] && player.tempx < g.xres)
            if (!player.stop_d) {
                player.tempx += 5;
            }
    }
}

void dasonRender() 
{
    Rect r;
    r.bot = g.yres/2+50;
    r.center = 0;
    r.left = g.xres/2;
    ggprint8b(&r, 0, 0x000000, "Press ' i ' for information");
}
