#include "Memory.h"
using namespace std;

Memory::Memory() {
    rom.assign(0xFFFF, 0);
}

void Memory::load() {
    ifstream f("opus5.gb", ios::binary);
    rom.assign(istreambuf_iterator<char>(f), istreambuf_iterator<char>());
}

void Memory::init() {
    wb(0xFF10, 0x80);
    wb(0xFF11, 0xBF);
    wb(0xFF12, 0xF3);
    wb(0xFF14, 0xBF);
    wb(0xFF16, 0x3F);
    wb(0xFF19, 0xBF);
    wb(0xFF1A, 0x7F);
    wb(0xFF1B, 0xFF);
    wb(0xFF1C, 0x9F);
    wb(0xFF1E, 0xBF);
    wb(0xFF20, 0xFF);
    wb(0xFF23, 0xBF);
    wb(0xFF24, 0x77);
    wb(0xFF25, 0xF3);
    wb(0xFF26, 0xF1);
    wb(0xFF40, 0x91);
    wb(0xFF41, 0x02);
    wb(0xFF47, 0xFC);
    wb(0xFF48, 0xFF);
    wb(0xFF49, 0xFF);
}

int Memory::rb(int addr) {
    return rom[addr];
}

void Memory::wb(int addr, int val) {
    rom[addr] = val;
}

int Memory::rw(int addr) {
    return rb(addr)+(rb(addr+1)<<8);
}

void Memory::ww(int addr, int val) {
    wb(addr, val&255);
    wb(addr+1, val>>8);
}
