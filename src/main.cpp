#include <iostream>
#include "CPU.h"
#include "Memory.h"
#include "GPU.h"
#include "Timer.h"
#include "Key.h"
using namespace std;

//Create and initialize hardware
CPU cpu;
Memory memory;
GPU gpu;
Timer timer;
Key key;

int main(int argc, char* argv[]) {
    //get ROM name and load to memory
    string romName;
    cout << "Beta version of Gameboy Simulator" << endl;
    cout << "Keypad Manual: up/down/left/right Z--A X--B Enter--Start Space--Select" << endl;
    cout << "ROM Name: ";
    cin >> romName;
    while(!memory.loadRom(romName)) {
        cout << "Fail to load the ROM." << endl;
        cout << "Please put ROM and exe in the same folder and remember the suffix '.gb'" << endl;
        cout << "ROM Name: ";
        cin >> romName;
    }
    gpu.init();
    int time;
    //Main game loop
    while(key.press()) {
        time = cpu.step();
        gpu.step(time);
        timer.step(time);
    }
    gpu.quit();
    return 0;
}
