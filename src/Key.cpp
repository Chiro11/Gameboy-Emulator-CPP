#include "Key.h"
using namespace std;

Key::Key() {
    keys[0] = keys[1] = 0xF;
}

bool Key::press() {
    bool quit = false;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT: quit = true; break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_UP: keys[1] &= 0xB; break;
                    case SDLK_DOWN: keys[1] &= 0x7; break;
                    case SDLK_LEFT: keys[1] &= 0xD; break;
                    case SDLK_RIGHT: keys[1] &= 0xE; break;
                    case SDLK_z: keys[0] &= 0xE; break;
                    case SDLK_x: keys[0] &= 0xD; break;
                    case SDLK_RETURN: keys[0] &= 0x7; break;
                    case SDLK_SPACE: keys[0] &= 0xB; break;
                }
                break;
            case SDL_KEYUP:
                switch(e.key.keysym.sym) {
                    case SDLK_UP: keys[1] |= 0x4; break;
                    case SDLK_DOWN: keys[1] |= 0x8; break;
                    case SDLK_LEFT: keys[1] |= 0x2; break;
                    case SDLK_RIGHT: keys[1] |= 0x1; break;
                    case SDLK_z: keys[0] |= 0x1; break;
                    case SDLK_x: keys[0] |= 0x2; break;
                    case SDLK_RETURN: keys[0] |= 0x8; break;
                    case SDLK_SPACE: keys[0] |= 0x4; break;
                }
                break;
        }
    }
    return !quit;
}

int Key::getKey(int id) {
    return keys[id];
}
