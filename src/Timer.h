#pragma once
#include "Memory.h"
using namespace std;

class Memory;

extern Memory memory;

class Timer {
private:
    unsigned char divider;
    int dividerClock, counterClock;
public:
    Timer();
    void step(int time);
    unsigned char getDivider();
    void dividerReset();
};
