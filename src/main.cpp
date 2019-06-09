#include <time.h>
#include "CPU.h"
#include "Memory.h"
#include "GPU.h"
#include "Key.h"
using namespace std;

CPU cpu;
Memory memory;
GPU gpu;
Key key;

int main(int argc, char* argv[]) {
    memory.load();
    memory.init();
    while(key.keypress()) {
        int time = cpu.step();
        gpu.step(time);
    }
    gpu.quit();
    return 0;
}
