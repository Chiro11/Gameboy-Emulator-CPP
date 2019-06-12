#include "Memory.h"
using namespace std;

Memory::Memory() {
    ram.assign(0x7FFF, 0);
    mbc_type = ram_bank = mode = 0;
    rom_bank = 1;
    mmu[0xFF40] = 0x91;
    mmu[0xFF41] = 0x02;
    mmu[0xFF47] = 0xFC;
    mmu[0xFF48] = 0xFF;
    mmu[0xFF49] = 0xFF;
}

void Memory::load_rom() {
    ifstream f("test\\ttt.gb", ios::binary);
    rom.assign(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
    mbc_type = rom[0x0147];
}

int Memory::rb(int addr) {
    unsigned char val = 0;
    if(addr<=0x3FFF)
        val = rom[addr];
    else if(addr<=0x7FFF)
        val = rom[addr-0x4000+rom_bank*0x4000];
    else if(addr<=0x9FFF)
        val = mmu[addr];
    else if(addr<=0xBFFF)
        val = ram[addr-0xA000+ram_bank*0x2000];
    else if(addr==0xFF00) {
        val = mmu[0xFF00]&0xF0;
        switch((val>>4)&3) {
            case 1: val |= key.getkey(0); break;
            case 2: val |= key.getkey(1); break;
        }
    }
    else if(addr==0xFF04)
        val = timer.get_div();
    else
        val = mmu[addr];
    return (int)val;
}

void Memory::wb(int addr, int val) {
    if(addr>=0x2000 && addr<=0x7FFF && mbc_type>=1) {
        if(addr<=0x3FFF)
            rom_bank = (rom_bank&0x60)+max(val&0x1F, 1);
        else if(addr<=0x5FFF) {
            if(mode && mbc_type>=2)
                ram_bank = val&0x03;
            else
                rom_bank = (rom_bank&0x1F)+(val&0x03)*32;
        }
        else if(addr<=0x7FFF && mbc_type>=2)
            mode = val&0x01;
    }
    else if(addr>=0x8000) {
        if(addr<=0x9FFF)
            mmu[addr] = (unsigned char)val;
        else if(addr<=0xBFFF)
            ram[addr-0xA000+ram_bank*0x2000] = (unsigned char)val;
        else if(addr==0xFF04)
            timer.div_reset();
        else
            mmu[addr] = (unsigned char)val;
    }
}

int Memory::rw(int addr) {
    return rb(addr)+(rb(addr+1)<<8);
}

void Memory::ww(int addr, int val) {
    wb(addr, val&0xFF);
    wb(addr+1, val>>8);
}
