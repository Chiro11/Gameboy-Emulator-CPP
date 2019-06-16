#include "Timer.h"
using namespace std;

Timer::Timer() {
    divider = 0;
    dividerClock = counterClock = 0;
}

//Two timer registers
void Timer::step(int time) {
    dividerClock += time;
    divider = (divider+dividerClock/64)&0xFF;
    dividerClock %= 64;
    counterClock += time;
    int counter = memory.readByte(0xFF05);
    int control = memory.readByte(0xFF07);
    int cycle;
    switch(control&3) {
        case 0: cycle = 256; break;
        case 1: cycle = 4; break;
        case 2: cycle = 16; break;
        case 3: cycle = 64; break;
    }
    counter += counterClock/cycle;
    counterClock %= cycle;
    if(counter>0xFF) {
        counter = memory.readByte(0xFF06);
        int IF = memory.readByte(0xFF0F);
        IF |= 0x04;
        memory.writeByte(0xFF0F, IF);
    }
    memory.writeByte(0xFF05, counter);
}

void Timer::dividerReset() {
    divider = 0;
}

unsigned char Timer::getDivider() {
    return divider;
}
