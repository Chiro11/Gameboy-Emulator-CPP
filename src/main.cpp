#include <time.h>
#include "CPU.h"
#include "Memory.h"
#include "GPU.h"
#include "Key.h"
#include "Timer.h"
using namespace std;

CPU cpu;
Memory memory;
GPU gpu;
Key key;
Timer timer;

int main(int argc, char* argv[]) {
    memory.load_rom();
    while(key.keypress()) {
        int time = cpu.step();
        gpu.step(time);
        timer.step(time);
    }
    gpu.quit();
    return 0;
}
