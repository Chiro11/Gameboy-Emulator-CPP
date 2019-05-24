#include <cstdio>
#include <SDL.h>
using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    SDL_Event e;
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT: quit = true; break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                case SDLK_UP: printf("up\n"); break;
                case SDLK_DOWN: printf("down\n"); break;
                }
                break;
            case SDL_KEYUP:
                switch(e.key.keysym.sym) {
                case SDLK_UP: printf("up\n"); break;
                case SDLK_DOWN: printf("down\n"); break;
                }
                break;
            }
        }
        SDL_UpdateWindowSurface(window);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
