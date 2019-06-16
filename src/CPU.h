#pragma once
#include <functional>
#include "Memory.h"
using namespace std;

extern Memory memory;

class CPU {
private:
    int ime, halt;
    int a, b, c, d, e, h, l, f, sp, pc;
    function<int()> opcode[0x100];
    function<int()> cb_opcode[0x100];
public:
    CPU();
    int step();
    void loadOpcode();
};
