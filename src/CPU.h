#pragma once
#include <functional>
#include "Memory.h"
using namespace std;

extern Memory memory;

class CPU {
private:
    int a, b, c, d, e, h, l, f, sp, pc;
    int reg_backup[10];
    function<int()> ops[256];
    function<int()> cbops[256];
public:
    CPU();
    int step();
    void backup();
    void restore();
    void opload();
};
