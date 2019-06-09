#pragma once
#include <functional>
#include "Memory.h"
using namespace std;

extern Memory memory;

class CPU {
private:
    FILE* fp;
    int a, b, c, d, e, h, l, f, sp, pc;
    int ime, halt;
    function<int()> ops[256];
    function<int()> cbops[256];
public:
    CPU();
    int step();
    void opload();
};
