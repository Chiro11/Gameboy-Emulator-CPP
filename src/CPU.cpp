#include "CPU.h"
using namespace std;

CPU::CPU() {
    a=0x01, b=0x00, c=0x13, d=0x00, e=0xD8, h=0x01, l=0x4D, f=0xB0;
    sp=0xFFFE, pc=0x0100;
    opload();
}

void CPU::backup() {
    reg_backup[0]=a;
    reg_backup[1]=b;
    reg_backup[2]=c;
    reg_backup[3]=d;
    reg_backup[4]=e;
    reg_backup[5]=h;
    reg_backup[6]=l;
    reg_backup[7]=f;
    reg_backup[8]=sp;
    reg_backup[9]=pc;
}

void CPU::restore() {
    a=reg_backup[0];
    b=reg_backup[1];
    c=reg_backup[2];
    d=reg_backup[3];
    e=reg_backup[4];
    f=reg_backup[5];
    h=reg_backup[6];
    l=reg_backup[7];
    sp=reg_backup[8];
    pc=reg_backup[9];
}

int CPU::step() {
    int id = memory.rb(pc++);
    return ops[id]();
}

//问题1: 关于f的0x80位
//问题2: 忽略的指令 总数是否符合
//问题3: 指令中的其它寄存器
//问题4: 关于正负号的问题
//问题5: 关于&0x01
void CPU::opload() {
ops[0x40] = [this]()->int{b=b; return 1;}; //LDrr_bb
ops[0x41] = [this]()->int{b=c; return 1;}; //LDrr_bc
ops[0x42] = [this]()->int{b=d; return 1;}; //LDrr_bd
ops[0x43] = [this]()->int{b=e; return 1;}; //LDrr_be
ops[0x44] = [this]()->int{b=h; return 1;}; //LDrr_bh
ops[0x45] = [this]()->int{b=l; return 1;}; //LDrr_bl
ops[0x47] = [this]()->int{b=a; return 1;}; //LDrr_ba
ops[0x48] = [this]()->int{c=b; return 1;}; //LDrr_cb
ops[0x49] = [this]()->int{c=c; return 1;}; //LDrr_cc
ops[0x4A] = [this]()->int{c=d; return 1;}; //LDrr_cd
ops[0x4B] = [this]()->int{c=e; return 1;}; //LDrr_ce
ops[0x4C] = [this]()->int{c=h; return 1;}; //LDrr_ch
ops[0x4D] = [this]()->int{c=l; return 1;}; //LDrr_cl
ops[0x4F] = [this]()->int{c=a; return 1;}; //LDrr_ca
ops[0x50] = [this]()->int{d=b; return 1;}; //LDrr_db
ops[0x51] = [this]()->int{d=c; return 1;}; //LDrr_dc
ops[0x52] = [this]()->int{d=d; return 1;}; //LDrr_dd
ops[0x53] = [this]()->int{d=e; return 1;}; //LDrr_de
ops[0x54] = [this]()->int{d=h; return 1;}; //LDrr_dh
ops[0x55] = [this]()->int{d=l; return 1;}; //LDrr_dl
ops[0x57] = [this]()->int{d=a; return 1;}; //LDrr_da
ops[0x58] = [this]()->int{e=b; return 1;}; //LDrr_eb
ops[0x59] = [this]()->int{e=c; return 1;}; //LDrr_ec
ops[0x5A] = [this]()->int{e=d; return 1;}; //LDrr_ed
ops[0x5B] = [this]()->int{e=e; return 1;}; //LDrr_ee
ops[0x5C] = [this]()->int{e=h; return 1;}; //LDrr_eh
ops[0x5D] = [this]()->int{e=l; return 1;}; //LDrr_el
ops[0x5F] = [this]()->int{e=a; return 1;}; //LDrr_ea
ops[0x60] = [this]()->int{h=b; return 1;}; //LDrr_hb
ops[0x61] = [this]()->int{h=c; return 1;}; //LDrr_hc
ops[0x62] = [this]()->int{h=d; return 1;}; //LDrr_hd
ops[0x63] = [this]()->int{h=e; return 1;}; //LDrr_he
ops[0x64] = [this]()->int{h=h; return 1;}; //LDrr_hh
ops[0x65] = [this]()->int{h=l; return 1;}; //LDrr_hl
ops[0x67] = [this]()->int{h=a; return 1;}; //LDrr_ha
ops[0x68] = [this]()->int{l=b; return 1;}; //LDrr_lb
ops[0x69] = [this]()->int{l=c; return 1;}; //LDrr_lc
ops[0x6A] = [this]()->int{l=d; return 1;}; //LDrr_ld
ops[0x6B] = [this]()->int{l=e; return 1;}; //LDrr_le
ops[0x6C] = [this]()->int{l=h; return 1;}; //LDrr_lh
ops[0x6D] = [this]()->int{l=l; return 1;}; //LDrr_ll
ops[0x6F] = [this]()->int{l=a; return 1;}; //LDrr_la
ops[0x78] = [this]()->int{a=b; return 1;}; //LDrr_ab
ops[0x79] = [this]()->int{a=c; return 1;}; //LDrr_ac
ops[0x7A] = [this]()->int{a=d; return 1;}; //LDrr_ad
ops[0x7B] = [this]()->int{a=e; return 1;}; //LDrr_ae
ops[0x7C] = [this]()->int{a=h; return 1;}; //LDrr_ah
ops[0x7D] = [this]()->int{a=l; return 1;}; //LDrr_al
ops[0x7F] = [this]()->int{a=a; return 1;}; //LDrr_aa
ops[0x46] = [this]()->int{b=memory.rb((h<<8)+l); return 2;}; //LDrHLm_b
ops[0x4E] = [this]()->int{c=memory.rb((h<<8)+l); return 2;}; //LDrHLm_c
ops[0x56] = [this]()->int{d=memory.rb((h<<8)+l); return 2;}; //LDrHLm_d
ops[0x5E] = [this]()->int{e=memory.rb((h<<8)+l); return 2;}; //LDrHLm_e
ops[0x66] = [this]()->int{h=memory.rb((h<<8)+l); return 2;}; //LDrHLm_h
ops[0x6E] = [this]()->int{l=memory.rb((h<<8)+l); return 2;}; //LDrHLm_l
ops[0x7E] = [this]()->int{b=memory.rb((h<<8)+l); return 2;}; //LDrHLm_a
ops[0x70] = [this]()->int{memory.wb((h<<8)+l, b); return 2;}; //LDHLmr_b
ops[0x71] = [this]()->int{memory.wb((h<<8)+l, c); return 2;}; //LDHLmr_c
ops[0x72] = [this]()->int{memory.wb((h<<8)+l, d); return 2;}; //LDHLmr_d
ops[0x73] = [this]()->int{memory.wb((h<<8)+l, e); return 2;}; //LDHLmr_e
ops[0x74] = [this]()->int{memory.wb((h<<8)+l, h); return 2;}; //LDHLmr_h
ops[0x75] = [this]()->int{memory.wb((h<<8)+l, l); return 2;}; //LDHLmr_l
ops[0x77] = [this]()->int{memory.wb((h<<8)+l, a); return 2;}; //LDHLmr_a
ops[0x06] = [this]()->int{b=memory.rb(pc++); return 2;}; //LDrn_b
ops[0x0E] = [this]()->int{c=memory.rb(pc++); return 2;}; //LDrn_c
ops[0x16] = [this]()->int{d=memory.rb(pc++); return 2;}; //LDrn_d
ops[0x1E] = [this]()->int{e=memory.rb(pc++); return 2;}; //LDrn_e
ops[0x26] = [this]()->int{h=memory.rb(pc++); return 2;}; //LDrn_h
ops[0x2E] = [this]()->int{l=memory.rb(pc++); return 2;}; //LDrn_l
ops[0x3E] = [this]()->int{a=memory.rb(pc++); return 2;}; //LDrn_a
ops[0x36] = [this]()->int{memory.wb((h<<8)+l, memory.rb(pc++)); return 3;}; //LDHLmn
ops[0x02] = [this]()->int{memory.wb((b<<8)+c, a); return 2;}; //LDBCmA
ops[0x12] = [this]()->int{memory.wb((d<<8)+e, a); return 2;}; //LDDEmA
ops[0xEA] = [this]()->int{memory.wb(memory.rw(pc), a); pc+=2; return 4;}; //LDmmA
ops[0x0A] = [this]()->int{a=memory.rb((b<<8)+c); return 2;}; //LDABCm
ops[0x1A] = [this]()->int{a=memory.rb((d<<8)+e); return 2;}; //LDADEm
ops[0xFA] = [this]()->int{a=memory.rb(memory.rw(pc)); pc+=2; return 4;}; //LDAmm
ops[0x01] = [this]()->int{c=memory.rb(pc); b=memory.rb(pc+1); pc+=2; return 3;}; //LDBCnn
ops[0x11] = [this]()->int{e=memory.rb(pc); d=memory.rb(pc+1); pc+=2; return 3;}; //LDDEnn
ops[0x21] = [this]()->int{l=memory.rb(pc); h=memory.rb(pc+1); pc+=2; return 3;}; //LDHLnn
ops[0x31] = [this]()->int{sp=memory.rw(pc); pc+=2; return 3;}; //LDSPnn
ops[0x36] = [this]()->int{int k=memory.rw(pc); l=memory.rb(k); h=memory.rb(k+1); pc+=2; return 5;}; //LDHLmm
//ops[0x36] = [this]()->int{int k=memory.rw(pc); memory.ww(k, (h<<8)+l); pc+=2; return 5;} //LDmmHL
ops[0x22] = [this]()->int{memory.wb((h<<8)+l, a); l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; return 2;}; //LDHLIA
ops[0x2A] = [this]()->int{a=memory.rb((h<<8)+l); l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; return 2;}; //LDAHLI
ops[0x32] = [this]()->int{memory.wb((h<<8)+l, a); l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; return 2;}; //LDHLDA
ops[0x3A] = [this]()->int{a=memory.rb((h<<8)+l); l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; return 2;}; //LDAHLD
ops[0xF0] = [this]()->int{a=memory.rb(0xFF00+memory.rb(pc++)); return 3;}; //LDAIOn
ops[0xE0] = [this]()->int{memory.wb(0xFF00+memory.rb(pc++), a); return 3;}; //LDIOnA
ops[0xF2] = [this]()->int{a=memory.rb(0xFF00+c); return 2;}; //LDAIOC
ops[0xE2] = [this]()->int{memory.wb(0xFF00+c, a); return 2;}; //LDIOCA
ops[0xF8] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; k+=sp; h=(k>>8)&0xFF; l=k&0xFF; return 3;}; //LDHLSPn
cbops[0x30] = [this]()->int{int k=b; b=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!b) f=0x80; return 1;}; //SWAPr_b
cbops[0x31] = [this]()->int{int k=c; c=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!c) f=0x80; return 1;}; //SWAPr_c
cbops[0x32] = [this]()->int{int k=d; d=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!d) f=0x80; return 1;}; //SWAPr_d
cbops[0x33] = [this]()->int{int k=e; e=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!e) f=0x80; return 1;}; //SWAPr_e
cbops[0x34] = [this]()->int{int k=h; h=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!h) f=0x80; return 1;}; //SWAPr_h
cbops[0x35] = [this]()->int{int k=l; l=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!l) f=0x80; return 1;}; //SWAPr_l
cbops[0x37] = [this]()->int{int k=a; a=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!a) f=0x80; return 1;}; //SWAPr_a
ops[0x80] = [this]()->int{int k=a; a+=b; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //ADDr_b
ops[0x81] = [this]()->int{int k=a; a+=c; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //ADDr_c
ops[0x82] = [this]()->int{int k=a; a+=d; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //ADDr_d
ops[0x83] = [this]()->int{int k=a; a+=e; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //ADDr_e
ops[0x84] = [this]()->int{int k=a; a+=h; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //ADDr_h
ops[0x85] = [this]()->int{int k=a; a+=l; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //ADDr_l
ops[0x87] = [this]()->int{int k=a; a+=a; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //ADDr_a
ops[0x86] = [this]()->int{int k=a; int t=memory.rb((h<<8)+l); a+=t; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADDHL
ops[0xC6] = [this]()->int{int k=a; int t=memory.rb(pc++); a+=t; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADDn
ops[0x09] = [this]()->int{int hl=(h<<8)+l; hl+=(b<<8)+c; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLBC
ops[0x19] = [this]()->int{int hl=(h<<8)+l; hl+=(d<<8)+e; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLDE
ops[0x29] = [this]()->int{int hl=(h<<8)+l; hl+=(h<<8)+l; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLHL
ops[0x39] = [this]()->int{int hl=(h<<8)+l; hl+=sp; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLSP
ops[0xE8] = [this]()->int{int k=memory.rb(pc++); if(k>0x7F) k=-((~k+1)&0xFF); sp+=k; return 4;}; //ADDSPn
ops[0x88] = [this]()->int{int k=a; a+=b; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //ADCr_b
ops[0x89] = [this]()->int{int k=a; a+=c; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //ADCr_c
ops[0x8A] = [this]()->int{int k=a; a+=d; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //ADCr_d
ops[0x8B] = [this]()->int{int k=a; a+=e; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //ADCr_e
ops[0x8C] = [this]()->int{int k=a; a+=h; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //ADCr_h
ops[0x8D] = [this]()->int{int k=a; a+=l; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //ADCr_l
ops[0x8F] = [this]()->int{int k=a; a+=a; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //ADCr_a
ops[0x8E] = [this]()->int{int k=a; int t=memory.rb((h<<8)+l); a+=t; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADCHL
ops[0xCE] = [this]()->int{int k=a; int t=memory.rb(pc++); a+=t; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADCn
ops[0x90] = [this]()->int{int k=a; a-=b; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //SUBr_b
ops[0x91] = [this]()->int{int k=a; a-=c; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //SUBr_c
ops[0x92] = [this]()->int{int k=a; a-=d; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //SUBr_d
ops[0x93] = [this]()->int{int k=a; a-=e; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //SUBr_e
ops[0x94] = [this]()->int{int k=a; a-=h; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //SUBr_h
ops[0x95] = [this]()->int{int k=a; a-=l; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //SUBr_l
ops[0x97] = [this]()->int{int k=a; a-=a; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //SUBr_a
ops[0x96] = [this]()->int{int k=a; int t=memory.rb((h<<8)+l); a-=t; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SUBHL
ops[0xD6] = [this]()->int{int k=a; int t=memory.rb(pc++); a-=t; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SUBn
ops[0x98] = [this]()->int{int k=a; a-=b; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //SBCr_b
ops[0x99] = [this]()->int{int k=a; a-=c; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //SBCr_c
ops[0x9A] = [this]()->int{int k=a; a-=d; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //SBCr_d
ops[0x9B] = [this]()->int{int k=a; a-=e; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //SBCr_e
ops[0x9C] = [this]()->int{int k=a; a-=h; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //SBCr_h
ops[0x9D] = [this]()->int{int k=a; a-=l; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //SBCr_l
ops[0x9F] = [this]()->int{int k=a; a-=a; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //SBCr_a
ops[0x9E] = [this]()->int{int k=a; int t=memory.rb((h<<8)+l); a-=t; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SBCHL
ops[0xDE] = [this]()->int{int k=a; int t=memory.rb(pc++); a-=t; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SBCn
ops[0xB8] = [this]()->int{int k=a-b; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //CPr_b
ops[0xB9] = [this]()->int{int k=a-c; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //CPr_c
ops[0xBA] = [this]()->int{int k=a-d; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //CPr_d
ops[0xBB] = [this]()->int{int k=a-e; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //CPr_e
ops[0xBC] = [this]()->int{int k=a-h; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //CPr_h
ops[0xBD] = [this]()->int{int k=a-l; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //CPr_l
ops[0xBF] = [this]()->int{int k=a-a; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //CPr_a
ops[0xBE] = [this]()->int{int k=a; int t=memory.rb((h<<8)+l); k-=t; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //CPHL
ops[0xFE] = [this]()->int{int k=a; int t=memory.rb(pc++); k-=t; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //CPn
ops[0x27] = [this]()->int{int k=a; if((f&0x20)||((a&0xF)>9)) a+=6; f&=0xEF; if((f&0x20)||(k>0x99)) {a+=0x60; f|=0x10;} return 1;}; //DAA
ops[0xA0] = [this]()->int{a&=b; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_b
ops[0xA1] = [this]()->int{a&=c; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_c
ops[0xA2] = [this]()->int{a&=d; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_d
ops[0xA3] = [this]()->int{a&=e; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_e
ops[0xA4] = [this]()->int{a&=h; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_h
ops[0xA5] = [this]()->int{a&=l; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_l
ops[0xA7] = [this]()->int{a&=a; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_a
ops[0xA6] = [this]()->int{a&=memory.rb((h<<8)+l); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ANDHL
ops[0xE6] = [this]()->int{a&=memory.rb(pc++); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ANDn
ops[0xB0] = [this]()->int{a|=b; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_b
ops[0xB1] = [this]()->int{a|=c; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_c
ops[0xB2] = [this]()->int{a|=d; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_d
ops[0xB3] = [this]()->int{a|=e; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_e
ops[0xB4] = [this]()->int{a|=h; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_h
ops[0xB5] = [this]()->int{a|=l; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_l
ops[0xB7] = [this]()->int{a|=a; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_a
ops[0xB6] = [this]()->int{a|=memory.rb((h<<8)+l); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ORHL
ops[0xF6] = [this]()->int{a|=memory.rb(pc++); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ORn
ops[0xA8] = [this]()->int{a^=b; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_b
ops[0xA9] = [this]()->int{a^=c; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_c
ops[0xAA] = [this]()->int{a^=d; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_d
ops[0xAB] = [this]()->int{a^=e; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_e
ops[0xAC] = [this]()->int{a^=h; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_h
ops[0xAD] = [this]()->int{a^=l; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_l
ops[0xAF] = [this]()->int{a^=a; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_a
ops[0xAE] = [this]()->int{a^=memory.rb((h<<8)+l); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //XORHL
ops[0xEE] = [this]()->int{a^=memory.rb(pc++); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //XORn
ops[0x04] = [this]()->int{b++; b&=0xFF; f=0; if(!b) f=0x80; return 1;}; //INCr_b
ops[0x0C] = [this]()->int{c++; c&=0xFF; f=0; if(!c) f=0x80; return 1;}; //INCr_c
ops[0x14] = [this]()->int{d++; d&=0xFF; f=0; if(!d) f=0x80; return 1;}; //INCr_d
ops[0x1C] = [this]()->int{e++; e&=0xFF; f=0; if(!e) f=0x80; return 1;}; //INCr_e
ops[0x24] = [this]()->int{h++; h&=0xFF; f=0; if(!h) f=0x80; return 1;}; //INCr_h
ops[0x2C] = [this]()->int{l++; l&=0xFF; f=0; if(!l) f=0x80; return 1;}; //INCr_l
ops[0x3C] = [this]()->int{a++; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //INCr_a
ops[0x34] = [this]()->int{int k=memory.rb((h<<8)+l)+1; k&=0xFF; memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; return 3;}; //INCHLm
ops[0x05] = [this]()->int{b--; b&=0xFF; f=0; if(!b) f=0x80; return 1;}; //DECr_b
ops[0x0D] = [this]()->int{c--; c&=0xFF; f=0; if(!c) f=0x80; return 1;}; //DECr_c
ops[0x15] = [this]()->int{d--; d&=0xFF; f=0; if(!d) f=0x80; return 1;}; //DECr_d
ops[0x1D] = [this]()->int{e--; e&=0xFF; f=0; if(!e) f=0x80; return 1;}; //DECr_e
ops[0x25] = [this]()->int{h--; h&=0xFF; f=0; if(!h) f=0x80; return 1;}; //DECr_h
ops[0x2D] = [this]()->int{l--; l&=0xFF; f=0; if(!l) f=0x80; return 1;}; //DECr_l
ops[0x3D] = [this]()->int{a--; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //DECr_a
ops[0x35] = [this]()->int{int k=memory.rb((h<<8)+l)-1; k&=0xFF; memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; return 3;}; //DECHLm
ops[0x03] = [this]()->int{c=(c+1)&0xFF; if(!c) b=(b+1)&0xFF; return 1;}; //INCBC
ops[0x13] = [this]()->int{e=(e+1)&0xFF; if(!e) d=(d+1)&0xFF; return 1;}; //INCDE
ops[0x23] = [this]()->int{l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; return 1;}; //INCHL
ops[0x33] = [this]()->int{sp=(sp+1)&0xFF; return 1;}; //INCSP
ops[0x0B] = [this]()->int{c=(c-1)&0xFF; if(c==0xFF) b=(b-1)&0xFF; return 1;}; //DECBC
ops[0x1B] = [this]()->int{e=(e-1)&0xFF; if(e==0xFF) d=(d-1)&0xFF; return 1;}; //DECDE
ops[0x2B] = [this]()->int{l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; return 1;}; //DECHL
ops[0x3B] = [this]()->int{sp=(sp-1)&0xFF; return 1;}; //DECSP
cbops[0x40] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x01)) f|=0x80; return 2;}; //BIT0b
cbops[0x41] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x01)) f|=0x80; return 2;}; //BIT0c
cbops[0x42] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x01)) f|=0x80; return 2;}; //BIT0d
cbops[0x43] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x01)) f|=0x80; return 2;}; //BIT0e
cbops[0x44] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x01)) f|=0x80; return 2;}; //BIT0h
cbops[0x45] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x01)) f|=0x80; return 2;}; //BIT0l
cbops[0x47] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x01)) f|=0x80; return 2;}; //BIT0a
cbops[0x46] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x01)) f|=0x80; return 3;}; //BIT0m
cbops[0x80] = [this]()->int{b&=0xFE; return 2;}; //RES0b
cbops[0x81] = [this]()->int{c&=0xFE; return 2;}; //RES0c
cbops[0x82] = [this]()->int{d&=0xFE; return 2;}; //RES0d
cbops[0x83] = [this]()->int{e&=0xFE; return 2;}; //RES0e
cbops[0x84] = [this]()->int{h&=0xFE; return 2;}; //RES0h
cbops[0x85] = [this]()->int{l&=0xFE; return 2;}; //RES0l
cbops[0x87] = [this]()->int{a&=0xFE; return 2;}; //RES0a
cbops[0x86] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xFE; memory.wb((h<<8)+l, k); return 4;}; //RES0m
cbops[0xC0] = [this]()->int{b|=0x01; return 2;}; //SET0b
cbops[0xC1] = [this]()->int{c|=0x01; return 2;}; //SET0c
cbops[0xC2] = [this]()->int{d|=0x01; return 2;}; //SET0d
cbops[0xC3] = [this]()->int{e|=0x01; return 2;}; //SET0e
cbops[0xC4] = [this]()->int{h|=0x01; return 2;}; //SET0h
cbops[0xC5] = [this]()->int{l|=0x01; return 2;}; //SET0l
cbops[0xC7] = [this]()->int{a|=0x01; return 2;}; //SET0a
cbops[0xC6] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x01; memory.wb((h<<8)+l, k); return 4;}; //SET0m
cbops[0x48] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x02)) f|=0x80; return 2;}; //BIT1b
cbops[0x49] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x02)) f|=0x80; return 2;}; //BIT1c
cbops[0x4A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x02)) f|=0x80; return 2;}; //BIT1d
cbops[0x4B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x02)) f|=0x80; return 2;}; //BIT1e
cbops[0x4C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x02)) f|=0x80; return 2;}; //BIT1h
cbops[0x4D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x02)) f|=0x80; return 2;}; //BIT1l
cbops[0x4F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x02)) f|=0x80; return 2;}; //BIT1a
cbops[0x4E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x02)) f|=0x80; return 3;}; //BIT1m
cbops[0x88] = [this]()->int{b&=0xFD; return 2;}; //RES1b
cbops[0x89] = [this]()->int{c&=0xFD; return 2;}; //RES1c
cbops[0x8A] = [this]()->int{d&=0xFD; return 2;}; //RES1d
cbops[0x8B] = [this]()->int{e&=0xFD; return 2;}; //RES1e
cbops[0x8C] = [this]()->int{h&=0xFD; return 2;}; //RES1h
cbops[0x8D] = [this]()->int{l&=0xFD; return 2;}; //RES1l
cbops[0x8F] = [this]()->int{a&=0xFD; return 2;}; //RES1a
cbops[0x8E] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xFD; memory.wb((h<<8)+l, k); return 4;}; //RES1m
cbops[0xC8] = [this]()->int{b|=0x02; return 2;}; //SET1b
cbops[0xC9] = [this]()->int{c|=0x02; return 2;}; //SET1c
cbops[0xCA] = [this]()->int{d|=0x02; return 2;}; //SET1d
cbops[0xCB] = [this]()->int{e|=0x02; return 2;}; //SET1e
cbops[0xCC] = [this]()->int{h|=0x02; return 2;}; //SET1h
cbops[0xCD] = [this]()->int{l|=0x02; return 2;}; //SET1l
cbops[0xCF] = [this]()->int{a|=0x02; return 2;}; //SET1a
cbops[0xCE] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x02; memory.wb((h<<8)+l, k); return 4;}; //SET1m
cbops[0x50] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x04)) f|=0x80; return 2;}; //BIT2b
cbops[0x51] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x04)) f|=0x80; return 2;}; //BIT2c
cbops[0x52] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x04)) f|=0x80; return 2;}; //BIT2d
cbops[0x53] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x04)) f|=0x80; return 2;}; //BIT2e
cbops[0x54] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x04)) f|=0x80; return 2;}; //BIT2h
cbops[0x55] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x04)) f|=0x80; return 2;}; //BIT2l
cbops[0x57] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x04)) f|=0x80; return 2;}; //BIT2a
cbops[0x56] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x04)) f|=0x80; return 3;}; //BIT2m
cbops[0x90] = [this]()->int{b&=0xFB; return 2;}; //RES2b
cbops[0x91] = [this]()->int{c&=0xFB; return 2;}; //RES2c
cbops[0x92] = [this]()->int{d&=0xFB; return 2;}; //RES2d
cbops[0x93] = [this]()->int{e&=0xFB; return 2;}; //RES2e
cbops[0x94] = [this]()->int{h&=0xFB; return 2;}; //RES2h
cbops[0x95] = [this]()->int{l&=0xFB; return 2;}; //RES2l
cbops[0x97] = [this]()->int{a&=0xFB; return 2;}; //RES2a
cbops[0x96] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xFB; memory.wb((h<<8)+l, k); return 4;}; //RES2m
cbops[0xD0] = [this]()->int{b|=0x04; return 2;}; //SET2b
cbops[0xD1] = [this]()->int{c|=0x04; return 2;}; //SET2c
cbops[0xD2] = [this]()->int{d|=0x04; return 2;}; //SET2d
cbops[0xD3] = [this]()->int{e|=0x04; return 2;}; //SET2e
cbops[0xD4] = [this]()->int{h|=0x04; return 2;}; //SET2h
cbops[0xD5] = [this]()->int{l|=0x04; return 2;}; //SET2l
cbops[0xD7] = [this]()->int{a|=0x04; return 2;}; //SET2a
cbops[0xD6] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x04; memory.wb((h<<8)+l, k); return 4;}; //SET2m
cbops[0x58] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x08)) f|=0x80; return 2;}; //BIT3b
cbops[0x59] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x08)) f|=0x80; return 2;}; //BIT3c
cbops[0x5A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x08)) f|=0x80; return 2;}; //BIT3d
cbops[0x5B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x08)) f|=0x80; return 2;}; //BIT3e
cbops[0x5C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x08)) f|=0x80; return 2;}; //BIT3h
cbops[0x5D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x08)) f|=0x80; return 2;}; //BIT3l
cbops[0x5F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x08)) f|=0x80; return 2;}; //BIT3a
cbops[0x5E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x08)) f|=0x80; return 3;}; //BIT3m
cbops[0x98] = [this]()->int{b&=0xF7; return 2;}; //RES3b
cbops[0x99] = [this]()->int{c&=0xF7; return 2;}; //RES3c
cbops[0x9A] = [this]()->int{d&=0xF7; return 2;}; //RES3d
cbops[0x9B] = [this]()->int{e&=0xF7; return 2;}; //RES3e
cbops[0x9C] = [this]()->int{h&=0xF7; return 2;}; //RES3h
cbops[0x9D] = [this]()->int{l&=0xF7; return 2;}; //RES3l
cbops[0x9F] = [this]()->int{a&=0xF7; return 2;}; //RES3a
cbops[0x9E] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xF7; memory.wb((h<<8)+l, k); return 4;}; //RES3m
cbops[0xD8] = [this]()->int{b|=0x08; return 2;}; //SET3b
cbops[0xD9] = [this]()->int{c|=0x08; return 2;}; //SET3c
cbops[0xDA] = [this]()->int{d|=0x08; return 2;}; //SET3d
cbops[0xDB] = [this]()->int{e|=0x08; return 2;}; //SET3e
cbops[0xDC] = [this]()->int{h|=0x08; return 2;}; //SET3h
cbops[0xDD] = [this]()->int{l|=0x08; return 2;}; //SET3l
cbops[0xDF] = [this]()->int{a|=0x08; return 2;}; //SET3a
cbops[0xDE] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x08; memory.wb((h<<8)+l, k); return 4;}; //SET3m
cbops[0x60] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x10)) f|=0x80; return 2;}; //BIT4b
cbops[0x61] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x10)) f|=0x80; return 2;}; //BIT4c
cbops[0x62] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x10)) f|=0x80; return 2;}; //BIT4d
cbops[0x63] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x10)) f|=0x80; return 2;}; //BIT4e
cbops[0x64] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x10)) f|=0x80; return 2;}; //BIT4h
cbops[0x65] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x10)) f|=0x80; return 2;}; //BIT4l
cbops[0x67] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x10)) f|=0x80; return 2;}; //BIT4a
cbops[0x66] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x10)) f|=0x80; return 3;}; //BIT4m
cbops[0xA0] = [this]()->int{b&=0xEF; return 2;}; //RES4b
cbops[0xA1] = [this]()->int{c&=0xEF; return 2;}; //RES4c
cbops[0xA2] = [this]()->int{d&=0xEF; return 2;}; //RES4d
cbops[0xA3] = [this]()->int{e&=0xEF; return 2;}; //RES4e
cbops[0xA4] = [this]()->int{h&=0xEF; return 2;}; //RES4h
cbops[0xA5] = [this]()->int{l&=0xEF; return 2;}; //RES4l
cbops[0xA7] = [this]()->int{a&=0xEF; return 2;}; //RES4a
cbops[0xA6] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xEF; memory.wb((h<<8)+l, k); return 4;}; //RES4m
cbops[0xE0] = [this]()->int{b|=0x10; return 2;}; //SET4b
cbops[0xE1] = [this]()->int{c|=0x10; return 2;}; //SET4c
cbops[0xE2] = [this]()->int{d|=0x10; return 2;}; //SET4d
cbops[0xE3] = [this]()->int{e|=0x10; return 2;}; //SET4e
cbops[0xE4] = [this]()->int{h|=0x10; return 2;}; //SET4h
cbops[0xE5] = [this]()->int{l|=0x10; return 2;}; //SET4l
cbops[0xE7] = [this]()->int{a|=0x10; return 2;}; //SET4a
cbops[0xE6] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x10; memory.wb((h<<8)+l, k); return 4;}; //SET4m
cbops[0x68] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x20)) f|=0x80; return 2;}; //BIT5b
cbops[0x69] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x20)) f|=0x80; return 2;}; //BIT5c
cbops[0x6A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x20)) f|=0x80; return 2;}; //BIT5d
cbops[0x6B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x20)) f|=0x80; return 2;}; //BIT5e
cbops[0x6C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x20)) f|=0x80; return 2;}; //BIT5h
cbops[0x6D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x20)) f|=0x80; return 2;}; //BIT5l
cbops[0x6F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x20)) f|=0x80; return 2;}; //BIT5a
cbops[0x6E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x20)) f|=0x80; return 3;}; //BIT5m
cbops[0xA8] = [this]()->int{b&=0xDF; return 2;}; //RES5b
cbops[0xA9] = [this]()->int{c&=0xDF; return 2;}; //RES5c
cbops[0xAA] = [this]()->int{d&=0xDF; return 2;}; //RES5d
cbops[0xAB] = [this]()->int{e&=0xDF; return 2;}; //RES5e
cbops[0xAC] = [this]()->int{h&=0xDF; return 2;}; //RES5h
cbops[0xAD] = [this]()->int{l&=0xDF; return 2;}; //RES5l
cbops[0xAF] = [this]()->int{a&=0xDF; return 2;}; //RES5a
cbops[0xAE] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xDF; memory.wb((h<<8)+l, k); return 4;}; //RES5m
cbops[0xE8] = [this]()->int{b|=0x20; return 2;}; //SET5b
cbops[0xE9] = [this]()->int{c|=0x20; return 2;}; //SET5c
cbops[0xEA] = [this]()->int{d|=0x20; return 2;}; //SET5d
cbops[0xEB] = [this]()->int{e|=0x20; return 2;}; //SET5e
cbops[0xEC] = [this]()->int{h|=0x20; return 2;}; //SET5h
cbops[0xED] = [this]()->int{l|=0x20; return 2;}; //SET5l
cbops[0xEF] = [this]()->int{a|=0x20; return 2;}; //SET5a
cbops[0xEE] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x20; memory.wb((h<<8)+l, k); return 4;}; //SET5m
cbops[0x70] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x40)) f|=0x80; return 2;}; //BIT6b
cbops[0x71] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x40)) f|=0x80; return 2;}; //BIT6c
cbops[0x72] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x40)) f|=0x80; return 2;}; //BIT6d
cbops[0x73] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x40)) f|=0x80; return 2;}; //BIT6e
cbops[0x74] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x40)) f|=0x80; return 2;}; //BIT6h
cbops[0x75] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x40)) f|=0x80; return 2;}; //BIT6l
cbops[0x77] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x40)) f|=0x80; return 2;}; //BIT6a
cbops[0x76] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x40)) f|=0x80; return 3;}; //BIT6m
cbops[0xB0] = [this]()->int{b&=0xBF; return 2;}; //RES6b
cbops[0xB1] = [this]()->int{c&=0xBF; return 2;}; //RES6c
cbops[0xB2] = [this]()->int{d&=0xBF; return 2;}; //RES6d
cbops[0xB3] = [this]()->int{e&=0xBF; return 2;}; //RES6e
cbops[0xB4] = [this]()->int{h&=0xBF; return 2;}; //RES6h
cbops[0xB5] = [this]()->int{l&=0xBF; return 2;}; //RES6l
cbops[0xB7] = [this]()->int{a&=0xBF; return 2;}; //RES6a
cbops[0xB6] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0xBF; memory.wb((h<<8)+l, k); return 4;}; //RES6m
cbops[0xF0] = [this]()->int{b|=0x40; return 2;}; //SET6b
cbops[0xF1] = [this]()->int{c|=0x40; return 2;}; //SET6c
cbops[0xF2] = [this]()->int{d|=0x40; return 2;}; //SET6d
cbops[0xF3] = [this]()->int{e|=0x40; return 2;}; //SET6e
cbops[0xF4] = [this]()->int{h|=0x40; return 2;}; //SET6h
cbops[0xF5] = [this]()->int{l|=0x40; return 2;}; //SET6l
cbops[0xF7] = [this]()->int{a|=0x40; return 2;}; //SET6a
cbops[0xF6] = [this]()->int{int k=memory.rb((h<<8)+l); k|=0x40; memory.wb((h<<8)+l, k); return 4;}; //SET6m
cbops[0x78] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x80)) f|=0x80; return 2;}; //BIT7b
cbops[0x79] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x80)) f|=0x80; return 2;}; //BIT7c
cbops[0x7A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x80)) f|=0x80; return 2;}; //BIT7d
cbops[0x7B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x80)) f|=0x80; return 2;}; //BIT7e
cbops[0x7C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x80)) f|=0x80; return 2;}; //BIT7h
cbops[0x7D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x80)) f|=0x80; return 2;}; //BIT7l
cbops[0x7F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x80)) f|=0x80; return 2;}; //BIT7a
cbops[0x7E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.rb((h<<8)+l)&0x80)) f|=0x80; return 3;}; //BIT7m
cbops[0xB8] = [this]()->int{b&=0x7F; return 2;}; //RES7b
cbops[0xB9] = [this]()->int{c&=0x7F; return 2;}; //RES7c
cbops[0xBA] = [this]()->int{d&=0x7F; return 2;}; //RES7d
cbops[0xBB] = [this]()->int{e&=0x7F; return 2;}; //RES7e
cbops[0xBC] = [this]()->int{h&=0x7F; return 2;}; //RES7h
cbops[0xBD] = [this]()->int{l&=0x7F; return 2;}; //RES7l
cbops[0xBF] = [this]()->int{a&=0x7F; return 2;}; //RES7a
cbops[0xBE] = [this]()->int{int k=memory.rb((h<<8)+l); k&=0x7F; memory.wb((h<<8)+l, k); return 4;}; //RES7m
cbops[0xF8] = [this]()->int{b|=0x80; return 2;}; //SET7b
cbops[0xF9] = [this]()->int{c|=0x80; return 2;}; //SET7c
cbops[0xFA] = [this]()->int{d|=0x80; return 2;}; //SET7d
cbops[0xFB] = [this]()->int{e|=0x80; return 2;}; //SET7e
cbops[0xFC] = [this]()->int{h|=0x80; return 2;}; //SET7h
cbops[0xFD] = [this]()->int{l|=0x80; return 2;}; //SET7l
cbops[0xFF] = [this]()->int{a|=0x80; return 2;}; //SET7a
cbops[0xFE] = [this]{int k=memory.rb((h<<8)+l); k|=0x80; memory.wb((h<<8)+l, k); return 4;}; //SET7m
ops[0x17] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RLA
ops[0x07] = [this]()->int{int k=0; if(a&0x80) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RLCA
ops[0x1F] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RRA
ops[0x0F] = [this]()->int{int k=0; if(a&0x01) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RRCA
cbops[0x10] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(b&0x80) t=0x10; b=(b<<1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_b
cbops[0x11] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(c&0x80) t=0x10; c=(c<<1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_c
cbops[0x12] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(d&0x80) t=0x10; d=(d<<1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_d
cbops[0x13] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(e&0x80) t=0x10; e=(e<<1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_e
cbops[0x14] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(h&0x80) t=0x10; h=(h<<1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_h
cbops[0x15] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(l&0x80) t=0x10; l=(l<<1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_l
cbops[0x17] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_a
cbops[0x16] = [this]()->int{int k=memory.rb((h<<8)+l); int t=0; if(f&0x10) t=1; int u=0; if(k&0x80) u=0x10; k=(k<<1)+t; k&=0xFF; f=0; if(!k) f=0x80; memory.wb((h<<8)+l, k); f=(f&0xEF)+u; return 4;}; //RLHL
cbops[0x00] = [this]()->int{int k=0; if(b&0x80) k=1; int t=0; if(b&0x80) t=0x10; b=(b<<1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_b
cbops[0x01] = [this]()->int{int k=0; if(c&0x10) k=1; int t=0; if(c&0x80) t=0x10; c=(c<<1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_c
cbops[0x02] = [this]()->int{int k=0; if(d&0x10) k=1; int t=0; if(d&0x80) t=0x10; d=(d<<1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_d
cbops[0x03] = [this]()->int{int k=0; if(e&0x10) k=1; int t=0; if(e&0x80) t=0x10; e=(e<<1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_e
cbops[0x04] = [this]()->int{int k=0; if(h&0x10) k=1; int t=0; if(h&0x80) t=0x10; h=(h<<1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_h
cbops[0x05] = [this]()->int{int k=0; if(l&0x10) k=1; int t=0; if(l&0x80) t=0x10; l=(l<<1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_l
cbops[0x07] = [this]()->int{int k=0; if(a&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_a
cbops[0x06] = [this]()->int{int k=memory.rb((h<<8)+l); int t=0; if(k&0x80) t=1; int u=0; if(k&0x80) u=0x10; k=(k<<1)+t; k&=0xFF; f=0; if(!k) f=0x80; memory.wb((h<<8)+l, k); f=(f&0xEF)+u; return 4;}; //RLCHL
cbops[0x18] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(b&0x01) t=0x10; b=(b>>1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_b
cbops[0x19] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(c&0x01) t=0x10; c=(c>>1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_c
cbops[0x1A] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(d&0x01) t=0x10; d=(d>>1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_d
cbops[0x1B] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(e&0x01) t=0x10; e=(e>>1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_e
cbops[0x1C] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(h&0x01) t=0x10; h=(h>>1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_h
cbops[0x1D] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(l&0x01) t=0x10; l=(l>>1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_l
cbops[0x1F] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_a
cbops[0x1E] = [this]()->int{int k=memory.rb((h<<8)+l); int t=0; if(f&0x10) t=0x80; int u=0; if(k&0x01) u=0x10; k=(k>>1)+t; k&=0xFF; memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; f=(f&0xEF)+u; return 4;}; //RRHL
cbops[0x08] = [this]()->int{int k=0; if(b&0x01) k=0x80; int t=0; if(b&0x01) t=0x10; b=(b>>1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_b
cbops[0x09] = [this]()->int{int k=0; if(c&0x01) k=0x80; int t=0; if(c&0x01) t=0x10; c=(c>>1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_c
cbops[0x0A] = [this]()->int{int k=0; if(d&0x01) k=0x80; int t=0; if(d&0x01) t=0x10; d=(d>>1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_d
cbops[0x0B] = [this]()->int{int k=0; if(e&0x01) k=0x80; int t=0; if(e&0x01) t=0x10; e=(e>>1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_e
cbops[0x0C] = [this]()->int{int k=0; if(h&0x01) k=0x80; int t=0; if(h&0x01) t=0x10; h=(h>>1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_h
cbops[0x0D] = [this]()->int{int k=0; if(l&0x01) k=0x80; int t=0; if(l&0x01) t=0x10; l=(l>>1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_l
cbops[0x0F] = [this]()->int{int k=0; if(a&0x01) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_a
cbops[0x0E] = [this]()->int{int k=memory.rb((h<<8)+l); int t=0; if(k&0x01) t=0x80; int u=0; if(k&0x01) u=0x10; k=(k>>1)+t; k&=0xFF; memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; f=(f&0xEF)+u; return 4;}; //RRCHL
cbops[0x20] = [this]()->int{int k=0; if(b&0x80) k=0x10; b=(b<<1)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_b
cbops[0x21] = [this]()->int{int k=0; if(c&0x80) k=0x10; c=(c<<1)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_c
cbops[0x22] = [this]()->int{int k=0; if(d&0x80) k=0x10; d=(d<<1)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_d
cbops[0x23] = [this]()->int{int k=0; if(e&0x80) k=0x10; e=(e<<1)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_e
cbops[0x24] = [this]()->int{int k=0; if(h&0x80) k=0x10; h=(h<<1)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_h
cbops[0x25] = [this]()->int{int k=0; if(l&0x80) k=0x10; l=(l<<1)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_l
cbops[0x27] = [this]()->int{int k=0; if(a&0x80) k=0x10; a=(a<<1)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_a
//cbops[0x0E] = [this]()->int{int k=0; if(b&0x80) k=0x10; b=(b<<1)&0xFF+1; f=0; if(!b) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_b
//cbops[0x0E] = [this]()->int{int k=0; if(c&0x80) k=0x10; c=(c<<1)&0xFF+1; f=0; if(!c) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_c
//cbops[0x0E] = [this]()->int{int k=0; if(d&0x80) k=0x10; d=(d<<1)&0xFF+1; f=0; if(!d) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_d
//cbops[0x0E] = [this]()->int{int k=0; if(e&0x80) k=0x10; e=(e<<1)&0xFF+1; f=0; if(!e) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_e
//cbops[0x0E] = [this]()->int{int k=0; if(h&0x80) k=0x10; h=(h<<1)&0xFF+1; f=0; if(!h) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_h
//cbops[0x0E] = [this]()->int{int k=0; if(l&0x80) k=0x10; l=(l<<1)&0xFF+1; f=0; if(!l) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_l
//cbops[0x0E] = [this]()->int{int k=0; if(a&0x80) k=0x10; a=(a<<1)&0xFF+1; f=0; if(!a) f=0x80; f=(f&0xEF)+k; return 2;} //SLLr_a
cbops[0x28] = [this]()->int{int k=b&0x80; int t=0; if(b&0x01) t=0x10; b=((b>>1)+k)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_b
cbops[0x29] = [this]()->int{int k=c&0x80; int t=0; if(c&0x01) t=0x10; c=((c>>1)+k)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_c
cbops[0x2A] = [this]()->int{int k=d&0x80; int t=0; if(d&0x01) t=0x10; d=((d>>1)+k)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_d
cbops[0x2B] = [this]()->int{int k=e&0x80; int t=0; if(e&0x01) t=0x10; e=((e>>1)+k)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_e
cbops[0x2C] = [this]()->int{int k=h&0x80; int t=0; if(h&0x01) t=0x10; h=((h>>1)+k)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_h
cbops[0x2D] = [this]()->int{int k=l&0x80; int t=0; if(l&0x01) t=0x10; l=((l>>1)+k)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_l
cbops[0x2F] = [this]()->int{int k=a&0x80; int t=0; if(a&0x01) t=0x10; a=((a>>1)+k)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_a
cbops[0x38] = [this]()->int{int k=0; if(b&0x01) k=0x10; b=(b>>1)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_b
cbops[0x39] = [this]()->int{int k=0; if(c&0x01) k=0x10; c=(c>>1)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_c
cbops[0x3A] = [this]()->int{int k=0; if(d&0x01) k=0x10; d=(d>>1)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_d
cbops[0x3B] = [this]()->int{int k=0; if(e&0x01) k=0x10; e=(e>>1)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_e
cbops[0x3C] = [this]()->int{int k=0; if(h&0x01) k=0x10; h=(h>>1)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_h
cbops[0x3D] = [this]()->int{int k=0; if(l&0x01) k=0x10; l=(l>>1)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_l
cbops[0x3F] = [this]()->int{int k=0; if(a&0x01) k=0x10; a=(a>>1)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_a
ops[0x2F] = [this]()->int{a^=0xFF; f=0; if(!a) f=0x80; return 1;}; //CPL
//ops[0x3F] = [this]{a=-a; f=0; if(a<0) f=0x10; a&=0xFF; if(!a) f|=0x80; return 2;} //NEG
ops[0x3F] = [this]()->int{int k=0; if(!(f&0x10)) k=0x10; f=(f&0xEF)+k; return 1;}; //CCF
ops[0x37] = [this]()->int{f|=0x10; return 1;}; //SCF
ops[0xC5] = [this]()->int{sp--; memory.wb(sp, b); sp--; memory.wb(sp, c); return 3;}; //PUSHBC
ops[0xD5] = [this]()->int{sp--; memory.wb(sp, d); sp--; memory.wb(sp, e); return 3;}; //PUSHDE
ops[0xE5] = [this]()->int{sp--; memory.wb(sp, h); sp--; memory.wb(sp, l); return 3;}; //PUSHHL
ops[0xF5] = [this]()->int{sp--; memory.wb(sp, a); sp--; memory.wb(sp, f); return 3;}; //PUSHAF
ops[0xC1] = [this]()->int{c=memory.rb(sp++); b=memory.rb(sp++); return 3;}; //POPBC
ops[0xD1] = [this]()->int{e=memory.rb(sp++); d=memory.rb(sp++); return 3;}; //POPDE
ops[0xE1] = [this]()->int{l=memory.rb(sp++); h=memory.rb(sp++); return 3;}; //POPHL
ops[0xF1] = [this]()->int{f=memory.rb(sp++); a=memory.rb(sp++); return 3;}; //POPAF
ops[0xC3] = [this]()->int{pc=memory.rw(pc); return 3;}; //JPnn
ops[0xE9] = [this]()->int{pc=(h<<8)+l; return 1;}; //JPHL
ops[0xC2] = [this]()->int{if((f&0x80)==0x00) {pc=memory.rw(pc); return 4;} else {pc+=2; return 3;}}; //JPNZnn
ops[0xCA] = [this]()->int{if((f&0x80)==0x80) {pc=memory.rw(pc); return 4;} else {pc+=2; return 3;}}; //JPZnn
ops[0xD2] = [this]()->int{if((f&0x10)==0x00) {pc=memory.rw(pc); return 4;} else {pc+=2; return 3;}}; //JPNCnn
ops[0xDA] = [this]()->int{if((f&0x10)==0x10) {pc=memory.rw(pc); return 4;} else {pc+=2; return 3;}}; //JPCnn
ops[0x18] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc+=k+1; return 3;}; //JRn
ops[0x20] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; int ret=2; if((f&0x80)==0x00) {pc+=k; ret++;} return ret;}; //JRNZn
ops[0x28] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; int ret=2; if((f&0x80)==0x80) {pc+=k; ret++;} return ret;}; //JRZn
ops[0x30] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; int ret=2; if((f&0x10)==0x00) {pc+=k; ret++;} return ret;}; //JRNCn
ops[0x38] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; int ret=2; if((f&0x10)==0x10) {pc+=k; ret++;} return ret;}; //JRCn
ops[0x10] = [this]()->int{int k=memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; int ret=2; b--; if(b) {pc+=k; ret++;} return ret;}; //DJNZn
ops[0xCD] = [this]()->int{sp-=2; memory.ww(sp, pc+2); pc=memory.rw(pc); return 5;}; //CALLnn
ops[0xC4] = [this]()->int{int ret=3; if((f&0x80)==0x00) {sp-=2; memory.ww(sp, pc+2); pc=memory.rw(pc); ret+=2;} else pc+=2; return ret;}; //CALLNZnn
ops[0xCC] = [this]()->int{int ret=3; if((f&0x80)==0x80) {sp-=2; memory.ww(sp, pc+2); pc=memory.rw(pc); ret+=2;} else pc+=2; return ret;}; //CALLZnn
ops[0xD4] = [this]()->int{int ret=3; if((f&0x10)==0x00) {sp-=2; memory.ww(sp, pc+2); pc=memory.rw(pc); ret+=2;} else pc+=2; return ret;}; //CALLNCnn
ops[0xDB] = [this]()->int{int ret=3; if((f&0x10)==0x10) {sp-=2; memory.ww(sp, pc+2); pc=memory.rw(pc); ret+=2;} else pc+=2; return ret;}; //CALLCnn
ops[0xC9] = [this]()->int{pc=memory.rw(sp); sp+=2; return 3;}; //RET
ops[0xD9] = [this]()->int{restore(); pc=memory.rw(sp); sp+=2; return 3;}; //RETI
ops[0xC0] = [this]()->int{int ret=1; if((f&0x80)==0x00) {pc=memory.rw(sp); sp+=2; ret+=2;} return ret;}; //RETNZ
ops[0xC8] = [this]()->int{int ret=1; if((f&0x80)==0x80) {pc=memory.rw(sp); sp+=2; ret+=2;} return ret;}; //RETZ
ops[0xD0] = [this]()->int{int ret=1; if((f&0x10)==0x00) {pc=memory.rw(sp); sp+=2; ret+=2;} return ret;}; //RETNC
ops[0xD8] = [this]()->int{int ret=1; if((f&0x10)==0x10) {pc=memory.rw(sp); sp+=2; ret+=2;} return ret;}; //RETC
ops[0xC7] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x00; return 3;}; //RST00
ops[0xCF] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x08; return 3;}; //RST08
ops[0xD7] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x10; return 3;}; //RST10
ops[0xDF] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x18; return 3;}; //RST18
ops[0xE7] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x20; return 3;}; //RST20
ops[0xEF] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x28; return 3;}; //RST28
ops[0xF7] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x30; return 3;}; //RST30
ops[0xFF] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x38; return 3;}; //RST38
//ops[0xD8] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x40; return 3;} //RST40
//ops[0xD8] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x48; return 3;} //RST48
//ops[0xD8] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x50; return 3;} //RST50
//ops[0xD8] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x58; return 3;} //RST58
//ops[0xD8] = [this]()->int{backup(); sp-=2; memory.ww(sp, pc); pc=0x60; return 3;} //RST60
ops[0x00] = [this]()->int{return 1;}; //NOP
ops[0x76] = [this]()->int{return 1;}; //HALT
ops[0xF3] = [this]()->int{return 1;}; //DI
ops[0xFB] = [this]()->int{return 1;}; //EI
ops[0xCB] = [this]()->int{return cbops[memory.rb(pc++)]();}; //MAPcb
}
