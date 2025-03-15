//Name: Russell Barreyro
//Date: Spring 2025
//
//
#include "rbarreyroheader.h"
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include <chrono>
#include "functions.h"
#include "fonts.h"
using namespace std;


//Start Credits
//--------------------------------------------------------------------

void rjEndCredit (void)
{
    Rect title;
    title.bot = 82;
    title.left = 10;
    title.center = 0;
    ggprint8b(&title, 16, 0x00ff0000, "Author 1: Russell Barreyro");
}
//--------------------------------------------------------------------

//--------------------------------------------------
// map.cpp implementations
GameMap::GameMap(int w, int h, float size) : 
    width(w), height(h), tileSize(size),
    tiles(h, std::vector<Tile>(w)) {}
//--------------------------------------------------

//--------------------------------------------------
// physics.cpp implementations
void Player::updatePosition(float dx, float dy, const GameMap& map) {
    x = std::clamp(x + dx, 0.0f, 
        map.width * map.tileSize - width);
    y = std::clamp(y + dy, 0.0f,
        map.height * map.tileSize - height);
}
//--------------------------------------------------

//--------------------------------------------------
// game.cpp implementations
Game::Game(Display* display, Window window, GLXContext context) :
    m_display(display), m_window(window), m_glContext(context),
    m_map(25, 19, 32.0f) {
    // ... rest of constructor
}

Game::~Game() {
    glXDestroyContext(m_display, m_glContext);
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);
}

void Game::initializeMap() {
    for(int y = 0; y < m_map.height; y++) {
        for(int x = 0; x < m_map.width; x++) {
            bool border = x == 0 || x == m_map.width-1 || y == 0 || y == m_map.height-1;
            float r, g, b;
            
            if(border) {
                r = 0.8f; g = 0.2f; b = 0.2f;
            } else {
                bool checker = (x + y) % 2 == 0;
                r = checker ? 0.3f : 0.25f;
                g = checker ? 0.3f : 0.25f;
                b = checker ? 0.4f : 0.35f;
            }
            
            m_map.tiles[y][x] = { border, {r, g, b} };
        }
    }
}

void Game::handleInput(XEvent& ev) {
    if(ev.type == KeyPress || ev.type == KeyRelease) {
        KeySym key = XLookupKeysym(&ev.xkey, 0);
        if(key < 256) keys[key] = (ev.type == KeyPress);
    }
}

void Game::update() {
    float dx = 0, dy = 0;
    if(keys[XK_w]) dy -= player.speed;
    if(keys[XK_s]) dy += player.speed;
    if(keys[XK_a]) dx -= player.speed;
    if(keys[XK_d]) dx += player.speed;
    
    player.updatePosition(dx, dy, m_map);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw map
    for(int y = 0; y < m_map.height; y++) {
        for(int x = 0; x < m_map.width; x++) {
            const Tile& tile = m_map.tiles[y][x];
            if(tile.solid) {
                glColor3fv(tile.color);
                glBegin(GL_QUADS);
                float xpos = x * m_map.tileSize;
                float ypos = y * m_map.tileSize;
                glVertex2f(xpos, ypos);
                glVertex2f(xpos + m_map.tileSize, ypos);
                glVertex2f(xpos + m_map.tileSize, ypos + m_map.tileSize);
                glVertex2f(xpos, ypos + m_map.tileSize);
                glEnd();
            }
        }
    }
    
    // Draw player
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(player.x, player.y);
    glVertex2f(player.x + player.width, player.y);
    glVertex2f(player.x + player.width, player.y + player.height);
    glVertex2f(player.x, player.y + player.height);
    glEnd();
    
    glXSwapBuffers(m_display, m_window);
}
//--------------------------------------------------------------------

//main.cpp
//--------------------------------------------------------------------
Display* initX() {
    Display* dpy = XOpenDisplay(nullptr);
    if(!dpy) throw std::runtime_error("Failed to open X display");
    return dpy;
}

GLXContext initGL(Display* dpy, Window win) {
    static int visualAttribs[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER,
        None
    };
    
    XVisualInfo* vi = glXChooseVisual(dpy, 0, visualAttribs);
    if(!vi) throw std::runtime_error("No suitable visual found");
    
    GLXContext context = glXCreateContext(dpy, vi, nullptr, GL_TRUE);
    if(!context) throw std::runtime_error("Failed to create GL context");
    
    glXMakeCurrent(dpy, win, context);
    glOrtho(0, 800, 600, 0, -1, 1);
    return context;
}

int main() {
    try {
        Display* dpy = initX();
        Window win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
            0, 0, 800, 600, 0, 0, 0);
        XMapWindow(dpy, win);
        XSelectInput(dpy, win, KeyPressMask | KeyReleaseMask);
        
        GLXContext glContext = initGL(dpy, win);
        Game game(dpy, win, glContext);
        
        
        while(true) {
            // Handle input
            while(XPending(dpy)) {
                XEvent ev;
                XNextEvent(dpy, &ev);
                if(ev.type == KeyPress && 
                   XLookupKeysym(&ev.xkey, 0) == XK_Escape) goto cleanup;
                game.handleInput(ev);
            }
            
            // Update and render
            game.update();
            game.render();
            
            // Frame timing
            usleep(16000);  // ~60 FPS
        }
        
        cleanup:
        return 0;
    } catch(const std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }
}

//--------------------------------------------------------------------
