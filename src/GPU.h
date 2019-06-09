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
    int curline[160];
public:
    GPU();
    void quit();
    void step(int time);
    void randerLine();
};
