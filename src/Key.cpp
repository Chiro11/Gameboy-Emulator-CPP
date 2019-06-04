#include "Key.h"
using namespace std;

Key::Key() {
    key[0] = key[1] = 0xF;
}

bool Key::keypress() {
    bool ret = true;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT: ret = false; break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_UP: key[1] &= 0xB; break;
                    case SDLK_DOWN: key[1] &= 0x7; break;
                    case SDLK_LEFT: key[1] &= 0xD; break;
                    case SDLK_RIGHT: key[1] &= 0xE; break;
                    case SDLK_z: key[0] &= 0xE; break;
                    case SDLK_x: key[0] &= 0xD; break;
                    case SDLK_RETURN: key[0] &= 0x7; break;
                    case SDLK_SPACE: key[0] &= 0xB; break;
                }
                break;
            case SDL_KEYUP:
                switch(e.key.keysym.sym) {
                    case SDLK_UP: key[1] |= 0x4; break;
                    case SDLK_DOWN: key[1] |= 0x8; break;
                    case SDLK_LEFT: key[1] |= 0x2; break;
                    case SDLK_RIGHT: key[1] |= 0x1; break;
                    case SDLK_z: key[0] |= 0x1; break;
                    case SDLK_x: key[0] |= 0x2; break;
                    case SDLK_RETURN: key[0] |= 0x8; break;
                    case SDLK_SPACE: key[0] |= 0x4; break;
                }
                break;
        }
    }
    return ret;
}

int Key::getkey(int id) {
    return key[id];
}
