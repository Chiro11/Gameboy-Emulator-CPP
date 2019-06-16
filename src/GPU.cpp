#include "GPU.h"
using namespace std;

const int colorMap[4] = {255, 192, 96, 0};

GPU::GPU() {
    clock = 0;
    memset(curLine, 0, sizeof(curLine));
}

//Initialize SDL
void GPU::init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
}

//Simulate LCD screen according to timing
void GPU::step(int time) {
    clock += time;
    int timingMode = memory.readByte(0xFF41)&3;
    switch(timingMode) {
        //Horizontal blank
        case 0:
            if(clock>=51) {
                clock -= 51;
                int row = memory.readByte(0xFF44);
                row++;
                memory.writeByte(0xFF44, row);
                if(row==144) {
                    timingMode = 1;
                    SDL_UpdateWindowSurface(window);
                    int IF = memory.readByte(0xFF0F);
                    IF |= 0x01;
                    memory.writeByte(0xFF0F, IF);
                }
                else
                    timingMode = 2;
            }
            break;
        //Vertical blank
        case 1:
            if(clock>=114) {
                int row = memory.readByte(0xFF44);
                row++;
                clock -= 114;
                if(row==154) {
                    timingMode = 2;
                    row = 0;
                }
                memory.writeByte(0xFF44, row);
            }
            break;
        //Render line(accessing OAM)
        case 2:
            if(clock>=20) {
                clock -= 20;
                timingMode = 3;
            }
            break;
        //Render line(accessing VRAM)
        case 3:
            if(clock>=43) {
                clock -= 43;
                timingMode = 0;
                renderLine();
            }
            break;
    }
    int val = memory.readByte(0xFF41);
    memory.writeByte(0xFF41, (val&0xFC)+timingMode);
}

//Render one line to buffer
void GPU::renderLine() {
    int row = memory.readByte(0xFF44);
    int lcdc = memory.readByte(0xFF40);
    //Render background
    if(lcdc&0x01) {
        int bgY = memory.readByte(0xFF42)+row;
        int bgX = memory.readByte(0xFF43);
        int bgPalette = memory.readByte(0xFF47);
        int bgTileMap = (lcdc>>3)&1;
        int bgTileSet = (lcdc>>4)&1;
        int addr = bgY/8*32+bgX/8;
        if(bgTileMap)
            addr += 0x9C00;
        else
            addr += 0x9800;
        for(int col=0; col<160; col++) {
            int tileID = memory.readByte(addr);
            if(!bgTileSet)
                tileID += 256;
            int tileY = bgY%8;
            int tileX = bgX%8;
            int pos = 0x8000+tileID*16+tileY*2;
            int u = memory.readByte(pos);
            int v = memory.readByte(pos+1);
            int val = ((u>>(7-tileX))&1)|(((v>>(7-tileX))&1)<<1);
            curLine[col] = val;
            int color = colorMap[(bgPalette>>(val*2))&3];
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[row*160+col] = SDL_MapRGB(surface->format, color, color, color);
            bgX++;
            if(bgX%8==0)
                addr++;
        }
    }
    //Render Sprites
    if(lcdc&0x02) {
        for(int i=0; i<40; i++) {
            int addr = 0xFE00+i*4;
            int objY = memory.readByte(addr)-16;
            if(row<objY || row>=objY+8)
                continue;
            int objX = memory.readByte(addr+1)-8;
            int tileID = memory.readByte(addr+2);
            int objInfo = memory.readByte(addr+3);
            int objPalette;
            if(objInfo&0x10)
                objPalette = memory.readByte(0xFF49);
            else
                objPalette = memory.readByte(0xFF48);
            for(int col=objX; col<objX+8; col++) {
                if(col<0 || col>=160)
                    continue;
                if(curLine[col] && (objInfo&0x80))
                    continue;
                int tileY;
                if(objInfo&0x40)
                    tileY = 7-(row-objY);
                else
                    tileY = row-objY;
                int tileX;
                if(objInfo&0x20)
                    tileX = 7-(col-objX);
                else
                    tileX = col-objX;
                int pos = 0x8000+tileID*16+tileY*2;
                int u = memory.readByte(pos);
                int v = memory.readByte(pos+1);
                int val = ((u>>(7-tileX))&1)|(((v>>(7-tileX))&1)<<1);
                if(!val)
                    continue;
                int color = colorMap[(objPalette>>(val*2))&3];
                Uint32* pixels = (Uint32*)surface->pixels;
                pixels[row*160+col] = SDL_MapRGB(surface->format, color, color, color);
            }
        }
    }
}

//SDL close
void GPU::quit() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
