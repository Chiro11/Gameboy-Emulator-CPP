#pragma once
#include <cstdio>
#include <SDL.h>
using namespace std;

class Key {
private:
    int keys[2];
    SDL_Event e;
public:
    Key();
    bool press();
    int getKey(int id);
};
