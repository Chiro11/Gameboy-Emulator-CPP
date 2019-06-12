#include "Timer.h"
using namespace std;

Timer::Timer() {
    div = 0;
    div_clock = cnt_clock = 0;
}

void Timer::step(int time) {
    div_clock += time;
    div = (div+div_clock/64)&0xFF;
    div_clock %= 64;
    cnt_clock += time;
    int cnt = memory.rb(0xFF05);
    int ctrl = memory.rb(0xFF07);
    int cycle = 0;
    switch(ctrl&0x03) {
        case 0: cycle = 256; break;
        case 1: cycle = 4; break;
        case 2: cycle = 16; break;
        case 3: cycle = 64; break;
    }
    cnt += cnt_clock/cycle;
    cnt_clock %= cycle;
    if(cnt>0xFF) {
        cnt = memory.rb(0xFF06);
        int IF = memory.rb(0xFF0F);
        IF |= 0x04;
        memory.wb(0xFF0F, IF);
    }
}

void Timer::div_reset() {
    div = 0;
}

unsigned char Timer::get_div() {
    return div;
}
