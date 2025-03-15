#ifndef RBARREYRO_HEADER_H
#define RBARREYRO_HEADER_H

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <vector>
#include <algorithm>
#include <cstdio>

// Forward declarations
struct Player;
class GameMap;

//--------------------------------------------------
// physics.h
struct Player {
    float x, y;
    float speed = 5.0f;
    float width = 20.0f;
    float height = 20.0f;

    void updatePosition(float dx, float dy, const GameMap& map);
};
//--------------------------------------------------

//--------------------------------------------------
// map.h
struct Tile {
    bool solid;
    float color[3];
};

class GameMap {
public:
    int width, height;
    float tileSize;
    std::vector<std::vector<Tile>> tiles;

    GameMap(int w, int h, float size);
};
//--------------------------------------------------

//--------------------------------------------------
// game.h
class Game {
    Display* m_display;
    Window m_window;
    GLXContext m_glContext;
    GameMap m_map;
    
public:
    Player player;
    bool keys[256] = {false};

    Game(Display* display, Window window, GLXContext context);
    ~Game();

    void handleInput(XEvent& ev);
    void update();
    void render();
    void initializeMap();
};
//--------------------------------------------------

#endif
