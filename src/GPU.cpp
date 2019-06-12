#include "GPU.h"
using namespace std;

const int palette[4] = {255, 192, 96, 0};

GPU::GPU() {
    clock = 0;
    memset(curline, 0, sizeof(curline));
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
                    int IF = memory.rb(0xFF0F);
                    IF |= 0x01;
                    memory.wb(0xFF0F, IF);
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
    int lcdc = memory.rb(0xFF40);
    if(lcdc&0x01) {
        int bgy = memory.rb(0xFF42)+row;
        int bgx = memory.rb(0xFF43);
        int bgp = memory.rb(0xFF47);
        int bgtilemap = (lcdc>>3)&1;
        int bgtileset = (lcdc>>4)&1;
        int addr = bgy/8*32+bgx/8;
        if(bgtilemap)
            addr += 0x9C00;
        else
            addr += 0x9800;
        for(int col=0; col<160; col++) {
            int tile_id = memory.rb(addr);
            if(!bgtileset)
                tile_id += 256;
            int tile_y = bgy%8;
            int tile_x = bgx%8;
            int pos = 0x8000+tile_id*16+tile_y*2;
            int u = memory.rb(pos);
            int v = memory.rb(pos+1);
            int val = ((u>>(7-tile_x))&1)|(((v>>(7-tile_x))&1)<<1);
            curline[col] = val;
            int tmp = (bgp>>(val*2))&3;
            int color = palette[tmp];
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[row*160+col] = SDL_MapRGB(surface->format, color, color, color);
            bgx++;
            if(bgx%8==0)
                addr++;
        }
    }
    if(lcdc&0x02) {
        for(int i=0; i<40; i++) {
            int addr = 0xFE00+i*4;
            int objy = memory.rb(addr)-16;
            if(row<objy || row>=objy+8)
                continue;
            int objx = memory.rb(addr+1)-8;
            int tile_id = memory.rb(addr+2);
            int objinfo = memory.rb(addr+3);
            int objp;
            if(objinfo&0x10)
                objp = memory.rb(0xFF49);
            else
                objp = memory.rb(0xFF48);
            for(int col=objx; col<objx+8; col++) {
                if(col<0 || col>=160)
                    continue;
                if(curline[col] && (objinfo&0x80))
                    continue;
                int tile_y;
                if(objinfo&0x40)
                    tile_y = 7-(row-objy);
                else
                    tile_y = row-objy;
                int tile_x;
                if(objinfo&0x20)
                    tile_x = 7-(col-objx);
                else
                    tile_x = col-objx;
                int pos = 0x8000+tile_id*16+tile_y*2;
                int u = memory.rb(pos);
                int v = memory.rb(pos+1);
                int val = ((u>>(7-tile_x))&1)|(((v>>(7-tile_x))&1)<<1);
                if(!val)
                    continue;
                int tmp = (objp>>(val*2))&3;
                int color = palette[tmp];
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[row*160+col] = SDL_MapRGB(surface->format, color, color, color);
            }
        }
    }
}
