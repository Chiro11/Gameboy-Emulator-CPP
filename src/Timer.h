#pragma once
#include "Memory.h"
using namespace std;

class Memory;

extern Memory memory;

class Timer {
private:
    unsigned char div;
    int div_clock, cnt_clock;
public:
    Timer();
    void step(int time);
    unsigned char get_div();
    void div_reset();
};
