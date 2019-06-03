#include <cstdio>
#include "CPU.h"
#include "Memory.h"
#include "GPU.h"
using namespace std;

CPU cpu;
Memory memory;
GPU gpu;

int main(int argc, char* argv[]) {
    memory.load();
    memory.init();
    while(true) {
        int time = cpu.step();
        gpu.step(time);
    }
    gpu.quit();
    return 0;
}
