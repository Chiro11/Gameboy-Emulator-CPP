#pragma once
#include <cstdio>
#include <vector>
#include <cstring>
#include <fstream>
using namespace std;

class Memory {
private:
    vector<unsigned char> rom;
public:
    Memory();
    void load();
    void init();
    int rb(int addr);
    int rw(int addr);
    void wb(int addr, int val);
    void ww(int addr, int val);
};
