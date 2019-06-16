#pragma once
#include <cstring>
#include <fstream>
#include <vector>
#include "Timer.h"
#include "Key.h"
using namespace std;

class Timer;

extern Timer timer;
extern Key key;

class Memory {
private:
    int mbcType, mbcMode, romBank, ramBank;
    unsigned char mmu[0x8000];
    vector<unsigned char> rom;
    vector<unsigned char> ram;
public:
    Memory();
    bool loadRom(string& romName);
    int readByte(int addr);
    int readWord(int addr);
    void writeByte(int addr, int val);
    void writeWord(int addr, int val);
};
