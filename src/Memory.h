#pragma once
#include <cstdio>
#include <vector>
#include <cstring>
#include <fstream>
#include "Key.h"
#include "Timer.h"
using namespace std;

class Timer;

extern Key key;
extern Timer timer;

class Memory {
private:
    int mbc_type, rom_bank, ram_bank, mode;
    unsigned char mmu[65536];
    vector<unsigned char> rom;
    vector<unsigned char> ram;
public:
    Memory();
    void load_rom();
    int rb(int addr);
    int rw(int addr);
    void wb(int addr, int val);
    void ww(int addr, int val);
};
