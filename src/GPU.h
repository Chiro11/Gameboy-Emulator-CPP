#pragma once
#include <cstring>
#include <SDL.h>
#include "Memory.h"
using namespace std;

extern Memory memory;

class GPU {
private:
    SDL_Window* window;
    SDL_Surface* surface;
    int clock;
    int curLine[160];
public:
    GPU();
    void init();
    void step(int time);
    void renderLine();
    void quit();
};
