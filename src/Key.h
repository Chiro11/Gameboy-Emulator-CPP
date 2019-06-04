#pragma once
#include <cstdio>
#include <SDL.h>
using namespace std;

class Key {
private:
    int key[2];
    SDL_Event e;
public:
    Key();
    bool keypress();
    int getkey(int id);
};
