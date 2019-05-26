using namespace std;

class CPU {
private:
    int a, b, c, d, e, h, l, f;
    int sp, pc;
    int cur_m, tot_m;
    int ime;
public:
    void backup();
    void restore();
};
