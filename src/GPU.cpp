#include "GPU.h"
using namespace std;

const int palette[4] = {0, 96, 192, 255};

GPU::GPU() {
    clock = 0;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
}

void GPU::quit() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GPU::step(int time) {
    clock += time;
    int mode = memory.rb(0xFF41)&3;
    switch(mode) {
        case 0:
            if(clock>=51) {
                clock -= 51;
                int row = memory.rb(0xFF44);
                row++;
                memory.wb(0xFF44, row);
                if(row==144) {
                    mode = 1;
                    SDL_UpdateWindowSurface(window);
                }
                else
                    mode = 2;
            }
            break;
        case 1:
            if(clock>=114) {
                int row = memory.rb(0xFF44);
                row++;
                clock -= 114;
                if(row==154) {
                    mode = 2;
                    row = 0;
                }
                memory.wb(0xFF44, row);
            }
            break;
        case 2:
            if(clock>=20) {
                clock -= 20;
                mode = 3;
            }
            break;
        case 3:
            if(clock>=43) {
                clock -= 43;
                mode = 0;
                randerLine();
            }
            break;
    }
    int val = memory.rb(0xFF41);
    memory.wb(0xFF41, (val&0xFC)+mode);
}

void GPU::randerLine() {
    int row = memory.rb(0xFF44);
    int y = memory.rb(0xFF42)+row;
    int x = memory.rb(0xFF43);
    int lcd_reg = memory.rb(0xFF40);
    int bgtilemap = (lcd_reg>>3)&1;
    int bgtileset = (lcd_reg>>4)&1;
    int addr = y/8*32+x/8;
    if(bgtilemap)
        addr += 0x9C00;
    else
        addr += 0x9800;
    if(lcd_reg&0x01) {
        for(int col=0; col<160; col++) {
            int tile_id = memory.rb(addr);
            if(!bgtileset)
                tile_id += 256;
            int tile_y = y%8;
            int tile_x = x%8;
            int pos = 0x8000+tile_id*16+tile_y*2;
            int u = memory.rb(pos);
            int v = memory.rb(pos+1);
            int color = palette[((u>>(7-tile_x))&1)|(((v>>(7-tile_x))&1)<<1)];
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[row*160+col] = SDL_MapRGB(surface->format, color, color, color);
            x++;
            if(x%8==0)
                addr++;
        }
    }
}
