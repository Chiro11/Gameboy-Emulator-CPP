using namespace std;

class MMU {
private:
public:
    int rb(int addr);
    int rw(int addr);
    void wb(int addr, int val);
    void ww(int addr, int val);
};
