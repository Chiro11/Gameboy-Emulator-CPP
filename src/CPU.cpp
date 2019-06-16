#include "CPU.h"
using namespace std;

//Initialize registers
CPU::CPU() {
    ime=1, halt=0;
    a=0x01, b=0x00, c=0x13, d=0x00, e=0xD8, h=0x01, l=0x4D, f=0xB0, sp=0xFFFE, pc=0x0100;
    loadOpcode();
}

//Execute one instruction at a time
int CPU::step() {
    int time = 0;
    //Interrupt Service Routine
    int IE = memory.readByte(0xFFFF);
    int IF = memory.readByte(0xFF0F);
    if(ime && (IE&IF&0x01)) {
        halt = 0; ime = 0;
        IF &= 0xFE;
        memory.writeByte(0xFF0F, IF);
        sp -= 2;
        memory.writeWord(sp, pc);
        pc = 0x40;
        time = 3;
    }
    else if(halt)
        time = 1;
    else {
        time = opcode[memory.readByte(pc++)]();
        pc &= 0xFFFF;
    }
    return time;
}

//Load opcode and CB prefix opcode
void CPU::loadOpcode() {
    for(int i=0; i<0x100; i++) {
        opcode[i] = [this]()->int{return 1;};
        cb_opcode[i] = [this]()->int{return 1;};
    }
    opcode[0x00] = [this]()->int{return 1;}; //NOP
    opcode[0x01] = [this]()->int{c=memory.readByte(pc); b=memory.readByte(pc+1); pc+=2; return 3;}; //LDBCnn
    opcode[0x02] = [this]()->int{memory.writeByte((b<<8)+c, a); return 2;}; //LDBCmA
    opcode[0x03] = [this]()->int{c=(c+1)&0xFF; if(!c) b=(b+1)&0xFF; return 1;}; //INCBC
    opcode[0x04] = [this]()->int{b=(b+1)&0xFF; f=0; if(!b) f=0x80; return 1;}; //INCr_b
    opcode[0x05] = [this]()->int{b=(b-1)&0xFF; f=0; if(!b) f=0x80; return 1;}; //DECr_b
    opcode[0x06] = [this]()->int{b=memory.readByte(pc++); return 2;}; //LDrn_b
    opcode[0x07] = [this]()->int{int k=0; if(a&0x80) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RLCA
    opcode[0x08] = [this]()->int{memory.writeWord(memory.readWord(pc), sp); pc+=2; return 5;}; //LDmmSP
    opcode[0x09] = [this]()->int{int hl=(h<<8)+l; hl+=((b<<8)+c); if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLBC
    opcode[0x0A] = [this]()->int{a=memory.readByte((b<<8)+c); return 2;}; //LDABCm
    opcode[0x0B] = [this]()->int{c=(c-1)&0xFF; if(c==0xFF) b=(b-1)&0xFF; return 1;}; //DECBC
    opcode[0x0C] = [this]()->int{c=(c+1)&0xFF; f=0; if(!c) f=0x80; return 1;}; //INCr_c
    opcode[0x0D] = [this]()->int{c=(c-1)&0xFF; f=0; if(!c) f=0x80; return 1;}; //DECr_c
    opcode[0x0E] = [this]()->int{c=memory.readByte(pc++); return 2;}; //LDrn_c
    opcode[0x0F] = [this]()->int{int k=0; if(a&0x01) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RRCA
    opcode[0x10] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); int ret=2; b--; if(b) {pc+=k; ret++;} return ret;}; //DJNZn
    opcode[0x11] = [this]()->int{e=memory.readByte(pc); d=memory.readByte(pc+1); pc+=2; return 3;}; //LDDEnn
    opcode[0x12] = [this]()->int{memory.writeByte((d<<8)+e, a); return 2;}; //LDDEmA
    opcode[0x13] = [this]()->int{e=(e+1)&0xFF; if(!e) d=(d+1)&0xFF; return 1;}; //INCDE
    opcode[0x14] = [this]()->int{d=(d+1)&0xFF; f=0; if(!d) f=0x80; return 1;}; //INCr_d
    opcode[0x15] = [this]()->int{d=(d-1)&0xFF; f=0; if(!d) f=0x80; return 1;}; //DECr_d
    opcode[0x16] = [this]()->int{d=memory.readByte(pc++); return 2;}; //LDrn_d
    opcode[0x17] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RLA
    opcode[0x18] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); pc+=k; return 3;}; //JRn
    opcode[0x19] = [this]()->int{int hl=(h<<8)+l; hl+=((d<<8)+e); if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLDE
    opcode[0x1A] = [this]()->int{a=memory.readByte((d<<8)+e); return 2;}; //LDADEm
    opcode[0x1B] = [this]()->int{e=(e-1)&0xFF; if(e==0xFF) d=(d-1)&0xFF; return 1;}; //DECDE
    opcode[0x1C] = [this]()->int{e=(e+1)&0xFF; f=0; if(!e) f=0x80; return 1;}; //INCr_e
    opcode[0x1D] = [this]()->int{e=(e-1)&0xFF; f=0; if(!e) f=0x80; return 1;}; //DECr_e
    opcode[0x1E] = [this]()->int{e=memory.readByte(pc++); return 2;}; //LDrn_e
    opcode[0x1F] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=(f&0xEF)+t; return 1;}; //RRA
    opcode[0x20] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); int ret=2; if((f&0x80)==0x00) {pc+=k; ret++;} return ret;}; //JRNZn
    opcode[0x21] = [this]()->int{l=memory.readByte(pc); h=memory.readByte(pc+1); pc+=2; return 3;}; //LDHLnn
    opcode[0x22] = [this]()->int{memory.writeByte((h<<8)+l, a); l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; return 2;}; //LDHLIA
    opcode[0x23] = [this]()->int{l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; return 1;}; //INCHL
    opcode[0x24] = [this]()->int{h=(h+1)&0xFF; f=0; if(!h) f=0x80; return 1;}; //INCr_h
    opcode[0x25] = [this]()->int{h=(h-1)&0xFF; f=0; if(!h) f=0x80; return 1;}; //DECr_h
    opcode[0x26] = [this]()->int{h=memory.readByte(pc++); return 2;}; //LDrn_h
    opcode[0x27] = [this]()->int{int k=a; if((f&0x20)||((a&0xF)>0x9)) a+=0x06; f&=0xEF; if((f&0x20)||(k>0x99)) {a+=0x60; f|=0x10;} return 1;}; //DAA
    opcode[0x28] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); int ret=2; if((f&0x80)==0x80) {pc+=k; ret++;} return ret;}; //JRZn
    opcode[0x29] = [this]()->int{int hl=(h<<8)+l; hl+=((h<<8)+l); if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLHL
    opcode[0x2A] = [this]()->int{a=memory.readByte((h<<8)+l); l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; return 2;}; //LDAHLI
    opcode[0x2B] = [this]()->int{l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; return 1;}; //DECHL
    opcode[0x2C] = [this]()->int{l=(l+1)&0xFF; f=0; if(!l) f=0x80; return 1;}; //INCr_l
    opcode[0x2D] = [this]()->int{l=(l-1)&0xFF; f=0; if(!l) f=0x80; return 1;}; //DECr_l
    opcode[0x2E] = [this]()->int{l=memory.readByte(pc++); return 2;}; //LDrn_l
    opcode[0x2F] = [this]()->int{a^=0xFF; f=0; if(!a) f=0x80; return 1;}; //CPL
    opcode[0x30] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); int ret=2; if((f&0x10)==0x00) {pc+=k; ret++;} return ret;}; //JRNCn
    opcode[0x31] = [this]()->int{sp=memory.readWord(pc); pc+=2; return 3;}; //LDSPnn
    opcode[0x32] = [this]()->int{memory.writeByte((h<<8)+l, a); l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; return 2;}; //LDHLDA
    opcode[0x33] = [this]()->int{sp=(sp+1)&0xFFFF; return 1;}; //INCSP
    opcode[0x34] = [this]()->int{int k=memory.readByte((h<<8)+l); k=(k+1)&0xFF; memory.writeByte((h<<8)+l, k); f=0; if(!k) f=0x80; return 3;}; //INCHLm
    opcode[0x35] = [this]()->int{int k=memory.readByte((h<<8)+l); k=(k-1)&0xFF; memory.writeByte((h<<8)+l, k); f=0; if(!k) f=0x80; return 3;}; //DECHLm
    opcode[0x36] = [this]()->int{memory.writeByte((h<<8)+l, memory.readByte(pc++)); return 3;}; //LDHLmn
    opcode[0x37] = [this]()->int{f|=0x10; return 1;}; //SCF
    opcode[0x38] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); int ret=2; if((f&0x10)==0x10) {pc+=k; ret++;} return ret;}; //JRCn
    opcode[0x39] = [this]()->int{int hl=(h<<8)+l; hl+=sp; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; return 3;}; //ADDHLSP
    opcode[0x3A] = [this]()->int{a=memory.readByte((h<<8)+l); l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; return 2;}; //LDAHLD
    opcode[0x3B] = [this]()->int{sp=(sp-1)&0xFFFF; return 1;}; //DECSP
    opcode[0x3C] = [this]()->int{a=(a+1)&0xFF; f=0; if(!a) f=0x80; return 1;}; //INCr_a
    opcode[0x3D] = [this]()->int{a=(a-1)&0xFF; f=0; if(!a) f=0x80; return 1;}; //DECr_a
    opcode[0x3E] = [this]()->int{a=memory.readByte(pc++); return 2;}; //LDrn_a
    opcode[0x3F] = [this]()->int{int k=0; if(!(f&0x10)) k=0x10; f=(f&0xEF)+k; return 1;}; //CCF
    opcode[0x40] = [this]()->int{b=b; return 1;}; //LDrr_bb
    opcode[0x41] = [this]()->int{b=c; return 1;}; //LDrr_bc
    opcode[0x42] = [this]()->int{b=d; return 1;}; //LDrr_bd
    opcode[0x43] = [this]()->int{b=e; return 1;}; //LDrr_be
    opcode[0x44] = [this]()->int{b=h; return 1;}; //LDrr_bh
    opcode[0x45] = [this]()->int{b=l; return 1;}; //LDrr_bl
    opcode[0x46] = [this]()->int{b=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_b
    opcode[0x47] = [this]()->int{b=a; return 1;}; //LDrr_ba
    opcode[0x48] = [this]()->int{c=b; return 1;}; //LDrr_cb
    opcode[0x49] = [this]()->int{c=c; return 1;}; //LDrr_cc
    opcode[0x4A] = [this]()->int{c=d; return 1;}; //LDrr_cd
    opcode[0x4B] = [this]()->int{c=e; return 1;}; //LDrr_ce
    opcode[0x4C] = [this]()->int{c=h; return 1;}; //LDrr_ch
    opcode[0x4D] = [this]()->int{c=l; return 1;}; //LDrr_cl
    opcode[0x4E] = [this]()->int{c=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_c
    opcode[0x4F] = [this]()->int{c=a; return 1;}; //LDrr_ca
    opcode[0x50] = [this]()->int{d=b; return 1;}; //LDrr_db
    opcode[0x51] = [this]()->int{d=c; return 1;}; //LDrr_dc
    opcode[0x52] = [this]()->int{d=d; return 1;}; //LDrr_dd
    opcode[0x53] = [this]()->int{d=e; return 1;}; //LDrr_de
    opcode[0x54] = [this]()->int{d=h; return 1;}; //LDrr_dh
    opcode[0x55] = [this]()->int{d=l; return 1;}; //LDrr_dl
    opcode[0x56] = [this]()->int{d=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_d
    opcode[0x57] = [this]()->int{d=a; return 1;}; //LDrr_da
    opcode[0x58] = [this]()->int{e=b; return 1;}; //LDrr_eb
    opcode[0x59] = [this]()->int{e=c; return 1;}; //LDrr_ec
    opcode[0x5A] = [this]()->int{e=d; return 1;}; //LDrr_ed
    opcode[0x5B] = [this]()->int{e=e; return 1;}; //LDrr_ee
    opcode[0x5C] = [this]()->int{e=h; return 1;}; //LDrr_eh
    opcode[0x5D] = [this]()->int{e=l; return 1;}; //LDrr_el
    opcode[0x5E] = [this]()->int{e=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_e
    opcode[0x5F] = [this]()->int{e=a; return 1;}; //LDrr_ea
    opcode[0x60] = [this]()->int{h=b; return 1;}; //LDrr_hb
    opcode[0x61] = [this]()->int{h=c; return 1;}; //LDrr_hc
    opcode[0x62] = [this]()->int{h=d; return 1;}; //LDrr_hd
    opcode[0x63] = [this]()->int{h=e; return 1;}; //LDrr_he
    opcode[0x64] = [this]()->int{h=h; return 1;}; //LDrr_hh
    opcode[0x65] = [this]()->int{h=l; return 1;}; //LDrr_hl
    opcode[0x66] = [this]()->int{h=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_h
    opcode[0x67] = [this]()->int{h=a; return 1;}; //LDrr_ha
    opcode[0x68] = [this]()->int{l=b; return 1;}; //LDrr_lb
    opcode[0x69] = [this]()->int{l=c; return 1;}; //LDrr_lc
    opcode[0x6A] = [this]()->int{l=d; return 1;}; //LDrr_ld
    opcode[0x6B] = [this]()->int{l=e; return 1;}; //LDrr_le
    opcode[0x6C] = [this]()->int{l=h; return 1;}; //LDrr_lh
    opcode[0x6D] = [this]()->int{l=l; return 1;}; //LDrr_ll
    opcode[0x6E] = [this]()->int{l=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_l
    opcode[0x6F] = [this]()->int{l=a; return 1;}; //LDrr_la
    opcode[0x70] = [this]()->int{memory.writeByte((h<<8)+l, b); return 2;}; //LDHLmr_b
    opcode[0x71] = [this]()->int{memory.writeByte((h<<8)+l, c); return 2;}; //LDHLmr_c
    opcode[0x72] = [this]()->int{memory.writeByte((h<<8)+l, d); return 2;}; //LDHLmr_d
    opcode[0x73] = [this]()->int{memory.writeByte((h<<8)+l, e); return 2;}; //LDHLmr_e
    opcode[0x74] = [this]()->int{memory.writeByte((h<<8)+l, h); return 2;}; //LDHLmr_h
    opcode[0x75] = [this]()->int{memory.writeByte((h<<8)+l, l); return 2;}; //LDHLmr_l
    opcode[0x76] = [this]()->int{halt=1; return 1;}; //HALT
    opcode[0x77] = [this]()->int{memory.writeByte((h<<8)+l, a); return 2;}; //LDHLmr_a
    opcode[0x78] = [this]()->int{a=b; return 1;}; //LDrr_ab
    opcode[0x79] = [this]()->int{a=c; return 1;}; //LDrr_ac
    opcode[0x7A] = [this]()->int{a=d; return 1;}; //LDrr_ad
    opcode[0x7B] = [this]()->int{a=e; return 1;}; //LDrr_ae
    opcode[0x7C] = [this]()->int{a=h; return 1;}; //LDrr_ah
    opcode[0x7D] = [this]()->int{a=l; return 1;}; //LDrr_al
    opcode[0x7E] = [this]()->int{a=memory.readByte((h<<8)+l); return 2;}; //LDrHLm_a
    opcode[0x7F] = [this]()->int{a=a; return 1;}; //LDrr_aa
    opcode[0x80] = [this]()->int{int k=a; a+=b; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //ADDr_b
    opcode[0x81] = [this]()->int{int k=a; a+=c; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //ADDr_c
    opcode[0x82] = [this]()->int{int k=a; a+=d; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //ADDr_d
    opcode[0x83] = [this]()->int{int k=a; a+=e; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //ADDr_e
    opcode[0x84] = [this]()->int{int k=a; a+=h; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //ADDr_h
    opcode[0x85] = [this]()->int{int k=a; a+=l; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //ADDr_l
    opcode[0x86] = [this]()->int{int k=a; int t=memory.readByte((h<<8)+l); a+=t; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADDHL
    opcode[0x87] = [this]()->int{int k=a; a+=a; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //ADDr_a
    opcode[0x88] = [this]()->int{int k=a; a+=b; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //ADCr_b
    opcode[0x89] = [this]()->int{int k=a; a+=c; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //ADCr_c
    opcode[0x8A] = [this]()->int{int k=a; a+=d; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //ADCr_d
    opcode[0x8B] = [this]()->int{int k=a; a+=e; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //ADCr_e
    opcode[0x8C] = [this]()->int{int k=a; a+=h; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //ADCr_h
    opcode[0x8D] = [this]()->int{int k=a; a+=l; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //ADCr_l
    opcode[0x8E] = [this]()->int{int k=a; int t=memory.readByte((h<<8)+l); a+=t; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADCHL
    opcode[0x8F] = [this]()->int{int k=a; a+=a; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //ADCr_a
    opcode[0x90] = [this]()->int{int k=a; a-=b; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //SUBr_b
    opcode[0x91] = [this]()->int{int k=a; a-=c; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //SUBr_c
    opcode[0x92] = [this]()->int{int k=a; a-=d; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //SUBr_d
    opcode[0x93] = [this]()->int{int k=a; a-=e; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //SUBr_e
    opcode[0x94] = [this]()->int{int k=a; a-=h; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //SUBr_h
    opcode[0x95] = [this]()->int{int k=a; a-=l; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //SUBr_l
    opcode[0x96] = [this]()->int{int k=a; int t=memory.readByte((h<<8)+l); a-=t; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SUBHL
    opcode[0x97] = [this]()->int{int k=a; a-=a; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //SUBr_a
    opcode[0x98] = [this]()->int{int k=a; a-=b; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //SBCr_b
    opcode[0x99] = [this]()->int{int k=a; a-=c; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //SBCr_c
    opcode[0x9A] = [this]()->int{int k=a; a-=d; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //SBCr_d
    opcode[0x9B] = [this]()->int{int k=a; a-=e; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //SBCr_e
    opcode[0x9C] = [this]()->int{int k=a; a-=h; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //SBCr_h
    opcode[0x9D] = [this]()->int{int k=a; a-=l; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //SBCr_l
    opcode[0x9E] = [this]()->int{int k=a; int t=memory.readByte((h<<8)+l); a-=t; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SBCHL
    opcode[0x9F] = [this]()->int{int k=a; a-=a; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //SBCr_a
    opcode[0xA0] = [this]()->int{a&=b; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_b
    opcode[0xA1] = [this]()->int{a&=c; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_c
    opcode[0xA2] = [this]()->int{a&=d; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_d
    opcode[0xA3] = [this]()->int{a&=e; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_e
    opcode[0xA4] = [this]()->int{a&=h; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_h
    opcode[0xA5] = [this]()->int{a&=l; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_l
    opcode[0xA6] = [this]()->int{a&=(memory.readByte((h<<8)+l)); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ANDHL
    opcode[0xA7] = [this]()->int{a&=a; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ANDr_a
    opcode[0xA8] = [this]()->int{a^=b; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_b
    opcode[0xA9] = [this]()->int{a^=c; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_c
    opcode[0xAA] = [this]()->int{a^=d; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_d
    opcode[0xAB] = [this]()->int{a^=e; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_e
    opcode[0xAC] = [this]()->int{a^=h; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_h
    opcode[0xAD] = [this]()->int{a^=l; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_l
    opcode[0xAE] = [this]()->int{a^=(memory.readByte((h<<8)+l)); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //XORHL
    opcode[0xAF] = [this]()->int{a^=a; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //XORr_a
    opcode[0xB0] = [this]()->int{a|=b; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_b
    opcode[0xB1] = [this]()->int{a|=c; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_c
    opcode[0xB2] = [this]()->int{a|=d; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_d
    opcode[0xB3] = [this]()->int{a|=e; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_e
    opcode[0xB4] = [this]()->int{a|=h; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_h
    opcode[0xB5] = [this]()->int{a|=l; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_l
    opcode[0xB6] = [this]()->int{a|=(memory.readByte((h<<8)+l)); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ORHL
    opcode[0xB7] = [this]()->int{a|=a; a&=0xFF; f=0; if(!a) f=0x80; return 1;}; //ORr_a
    opcode[0xB8] = [this]()->int{int k=a-b; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^b^k)&0x10) f|=0x20; return 1;}; //CPr_b
    opcode[0xB9] = [this]()->int{int k=a-c; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^c^k)&0x10) f|=0x20; return 1;}; //CPr_c
    opcode[0xBA] = [this]()->int{int k=a-d; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^d^k)&0x10) f|=0x20; return 1;}; //CPr_d
    opcode[0xBB] = [this]()->int{int k=a-e; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^e^k)&0x10) f|=0x20; return 1;}; //CPr_e
    opcode[0xBC] = [this]()->int{int k=a-h; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^h^k)&0x10) f|=0x20; return 1;}; //CPr_h
    opcode[0xBD] = [this]()->int{int k=a-l; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^l^k)&0x10) f|=0x20; return 1;}; //CPr_l
    opcode[0xBE] = [this]()->int{int k=a; int t=memory.readByte((h<<8)+l); k-=t; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //CPHL
    opcode[0xBF] = [this]()->int{int k=a-a; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^a^k)&0x10) f|=0x20; return 1;}; //CPr_a
    opcode[0xC0] = [this]()->int{int ret=1; if((f&0x80)==0x00) {pc=memory.readWord(sp); sp+=2; ret+=2;} return ret;}; //RETNZ
    opcode[0xC1] = [this]()->int{c=memory.readByte(sp++); b=memory.readByte(sp++); return 3;}; //POPBC
    opcode[0xC2] = [this]()->int{if((f&0x80)==0x00) {pc=memory.readWord(pc); return 4;} else {pc+=2; return 3;}}; //JPNZnn
    opcode[0xC3] = [this]()->int{pc=memory.readWord(pc); return 3;}; //JPnn
    opcode[0xC4] = [this]()->int{int ret=3; if((f&0x80)==0x00) {sp-=2; memory.writeWord(sp, pc+2); pc=memory.readWord(pc); ret+=2;} else pc+=2; return ret;}; //CALLNZnn
    opcode[0xC5] = [this]()->int{memory.writeByte(--sp, b); memory.writeByte(--sp, c); return 3;}; //PUSHBC
    opcode[0xC6] = [this]()->int{int k=a; int t=memory.readByte(pc++); a+=t; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADDn
    opcode[0xC7] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x00; return 3;}; //RST00
    opcode[0xC8] = [this]()->int{int ret=1; if((f&0x80)==0x80) {pc=memory.readWord(sp); sp+=2; ret+=2;} return ret;}; //RETZ
    opcode[0xC9] = [this]()->int{pc=memory.readWord(sp); sp+=2; return 3;}; //RET
    opcode[0xCA] = [this]()->int{if((f&0x80)==0x80) {pc=memory.readWord(pc); return 4;} else {pc+=2; return 3;}}; //JPZnn
    opcode[0xCB] = [this]()->int{int k=memory.readByte(pc++); pc&=0xFFFF; return cb_opcode[k]();}; //MAPcb
    opcode[0xCC] = [this]()->int{int ret=3; if((f&0x80)==0x80) {sp-=2; memory.writeWord(sp, pc+2); pc=memory.readWord(pc); ret+=2;} else pc+=2; return ret;}; //CALLZnn
    opcode[0xCD] = [this]()->int{sp-=2; memory.writeWord(sp, pc+2); pc=memory.readWord(pc); return 5;}; //CALLnn
    opcode[0xCE] = [this]()->int{int k=a; int t=memory.readByte(pc++); a+=t; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //ADCn
    opcode[0xCF] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x08; return 3;}; //RST08
    opcode[0xD0] = [this]()->int{int ret=1; if((f&0x10)==0x00) {pc=memory.readWord(sp); sp+=2; ret+=2;} return ret;}; //RETNC
    opcode[0xD1] = [this]()->int{e=memory.readByte(sp++); d=memory.readByte(sp++); return 3;}; //POPDE
    opcode[0xD2] = [this]()->int{if((f&0x10)==0x00) {pc=memory.readWord(pc); return 4;} else {pc+=2; return 3;}}; //JPNCnn
    opcode[0xD4] = [this]()->int{int ret=3; if((f&0x10)==0x00) {sp-=2; memory.writeWord(sp, pc+2); pc=memory.readWord(pc); ret+=2;} else pc+=2; return ret;}; //CALLNCnn
    opcode[0xD5] = [this]()->int{memory.writeByte(--sp, d); memory.writeByte(--sp, e); return 3;}; //PUSHDE
    opcode[0xD6] = [this]()->int{int k=a; int t=memory.readByte(pc++); a-=t; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SUBn
    opcode[0xD7] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x10; return 3;}; //RST10
    opcode[0xD8] = [this]()->int{int ret=1; if((f&0x10)==0x10) {pc=memory.readWord(sp); sp+=2; ret+=2;} return ret;}; //RETC
    opcode[0xD9] = [this]()->int{ime=1; pc=memory.readWord(sp); sp+=2; return 3;}; //RETI
    opcode[0xDA] = [this]()->int{if((f&0x10)==0x10) {pc=memory.readWord(pc); return 4;} else {pc+=2; return 3;}}; //JPCnn
    opcode[0xDC] = [this]()->int{int ret=3; if((f&0x10)==0x10) {sp-=2; memory.writeWord(sp, pc+2); pc=memory.readWord(pc); ret+=2;} else pc+=2; return ret;}; //CALLCnn
    opcode[0xDE] = [this]()->int{int k=a; int t=memory.readByte(pc++); a-=t; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //SBCn
    opcode[0xDF] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x18; return 3;}; //RST18
    opcode[0xE0] = [this]()->int{memory.writeByte(0xFF00+memory.readByte(pc++), a); return 3;}; //LDIOnA
    opcode[0xE1] = [this]()->int{l=memory.readByte(sp++); h=memory.readByte(sp++); return 3;}; //POPHL
    opcode[0xE2] = [this]()->int{memory.writeByte(0xFF00+c, a); return 2;}; //LDIOCA
    opcode[0xE5] = [this]()->int{memory.writeByte(--sp, h); memory.writeByte(--sp, l); return 3;}; //PUSHHL
    opcode[0xE6] = [this]()->int{a&=(memory.readByte(pc++)); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ANDn
    opcode[0xE7] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x20; return 3;}; //RST20
    opcode[0xE8] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); sp+=k; return 4;}; //ADDSPn
    opcode[0xE9] = [this]()->int{pc=(h<<8)+l; return 1;}; //JPHL
    opcode[0xEA] = [this]()->int{memory.writeByte(memory.readWord(pc), a); pc+=2; return 4;}; //LDmmA
    opcode[0xEE] = [this]()->int{a^=(memory.readByte(pc++)); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //XORn
    opcode[0xEF] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x28; return 3;}; //RST28
    opcode[0xF0] = [this]()->int{a=memory.readByte(0xFF00+memory.readByte(pc++)); return 3;}; //LDAIOn
    opcode[0xF1] = [this]()->int{f=memory.readByte(sp++); a=memory.readByte(sp++); return 3;}; //POPAF
    opcode[0xF2] = [this]()->int{a=memory.readByte(0xFF00+c); return 2;}; //LDAIOC
    opcode[0xF3] = [this]()->int{ime=0; return 1;}; //DI
    opcode[0xF5] = [this]()->int{memory.writeByte(--sp, a); memory.writeByte(--sp, f); return 3;}; //PUSHAF
    opcode[0xF6] = [this]()->int{a|=(memory.readByte(pc++)); a&=0xFF; f=0; if(!a) f=0x80; return 2;}; //ORn
    opcode[0xF7] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x30; return 3;}; //RST30
    opcode[0xF8] = [this]()->int{int k=memory.readByte(pc++); if(k>0x7F) k=-((~k+1)&0xFF); k+=sp; h=(k>>8)&0xFF; l=k&0xFF; return 3;}; //LDHLSPn
    opcode[0xFA] = [this]()->int{a=memory.readByte(memory.readWord(pc)); pc+=2; return 4;}; //LDAmm
    opcode[0xFB] = [this]()->int{ime=1; return 1;}; //EI
    opcode[0xFE] = [this]()->int{int k=a; int t=memory.readByte(pc++); k-=t; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^k^t)&0x10) f|=0x20; return 2;}; //CPn
    opcode[0xFF] = [this]()->int{sp-=2; memory.writeWord(sp, pc); pc=0x38; return 3;}; //RST38
    cb_opcode[0x00] = [this]()->int{int k=0; if(b&0x80) k=1; int t=0; if(b&0x80) t=0x10; b=(b<<1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_b
    cb_opcode[0x01] = [this]()->int{int k=0; if(c&0x80) k=1; int t=0; if(c&0x80) t=0x10; c=(c<<1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_c
    cb_opcode[0x02] = [this]()->int{int k=0; if(d&0x80) k=1; int t=0; if(d&0x80) t=0x10; d=(d<<1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_d
    cb_opcode[0x03] = [this]()->int{int k=0; if(e&0x80) k=1; int t=0; if(e&0x80) t=0x10; e=(e<<1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_e
    cb_opcode[0x04] = [this]()->int{int k=0; if(h&0x80) k=1; int t=0; if(h&0x80) t=0x10; h=(h<<1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_h
    cb_opcode[0x05] = [this]()->int{int k=0; if(l&0x80) k=1; int t=0; if(l&0x80) t=0x10; l=(l<<1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_l
    cb_opcode[0x06] = [this]()->int{int k=memory.readByte((h<<8)+l); int t=0; if(k&0x80) t=1; int u=0; if(k&0x80) u=0x10; k=(k<<1)+t; k&=0xFF; f=0; if(!k) f=0x80; memory.writeByte((h<<8)+l, k); f=(f&0xEF)+u; return 4;}; //RLCHL
    cb_opcode[0x07] = [this]()->int{int k=0; if(a&0x80) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RLCr_a
    cb_opcode[0x08] = [this]()->int{int k=0; if(b&0x01) k=0x80; int t=0; if(b&0x01) t=0x10; b=(b>>1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_b
    cb_opcode[0x09] = [this]()->int{int k=0; if(c&0x01) k=0x80; int t=0; if(c&0x01) t=0x10; c=(c>>1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_c
    cb_opcode[0x0A] = [this]()->int{int k=0; if(d&0x01) k=0x80; int t=0; if(d&0x01) t=0x10; d=(d>>1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_d
    cb_opcode[0x0B] = [this]()->int{int k=0; if(e&0x01) k=0x80; int t=0; if(e&0x01) t=0x10; e=(e>>1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_e
    cb_opcode[0x0C] = [this]()->int{int k=0; if(h&0x01) k=0x80; int t=0; if(h&0x01) t=0x10; h=(h>>1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_h
    cb_opcode[0x0D] = [this]()->int{int k=0; if(l&0x01) k=0x80; int t=0; if(l&0x01) t=0x10; l=(l>>1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_l
    cb_opcode[0x0E] = [this]()->int{int k=memory.readByte((h<<8)+l); int t=0; if(k&0x01) t=0x80; int u=0; if(k&0x01) u=0x10; k=(k>>1)+t; k&=0xFF; memory.writeByte((h<<8)+l, k); f=0; if(!k) f=0x80; f=(f&0xEF)+u; return 4;}; //RRCHL
    cb_opcode[0x0F] = [this]()->int{int k=0; if(a&0x01) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RRCr_a
    cb_opcode[0x10] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(b&0x80) t=0x10; b=(b<<1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_b
    cb_opcode[0x11] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(c&0x80) t=0x10; c=(c<<1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_c
    cb_opcode[0x12] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(d&0x80) t=0x10; d=(d<<1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_d
    cb_opcode[0x13] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(e&0x80) t=0x10; e=(e<<1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_e
    cb_opcode[0x14] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(h&0x80) t=0x10; h=(h<<1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_h
    cb_opcode[0x15] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(l&0x80) t=0x10; l=(l<<1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_l
    cb_opcode[0x16] = [this]()->int{int k=memory.readByte((h<<8)+l); int t=0; if(f&0x10) t=1; int u=0; if(k&0x80) u=0x10; k=(k<<1)+t; k&=0xFF; f=0; if(!k) f=0x80; memory.writeByte((h<<8)+l, k); f=(f&0xEF)+u; return 4;}; //RLHL
    cb_opcode[0x17] = [this]()->int{int k=0; if(f&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RLr_a
    cb_opcode[0x18] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(b&0x01) t=0x10; b=(b>>1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_b
    cb_opcode[0x19] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(c&0x01) t=0x10; c=(c>>1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_c
    cb_opcode[0x1A] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(d&0x01) t=0x10; d=(d>>1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_d
    cb_opcode[0x1B] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(e&0x01) t=0x10; e=(e>>1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_e
    cb_opcode[0x1C] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(h&0x01) t=0x10; h=(h>>1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_h
    cb_opcode[0x1D] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(l&0x01) t=0x10; l=(l>>1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_l
    cb_opcode[0x1E] = [this]()->int{int k=memory.readByte((h<<8)+l); int t=0; if(f&0x10) t=0x80; int u=0; if(k&0x01) u=0x10; k=(k>>1)+t; k&=0xFF; memory.writeByte((h<<8)+l, k); f=0; if(!k) f=0x80; f=(f&0xEF)+u; return 4;}; //RRHL
    cb_opcode[0x1F] = [this]()->int{int k=0; if(f&0x10) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //RRr_a
    cb_opcode[0x20] = [this]()->int{int k=0; if(b&0x80) k=0x10; b=(b<<1)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_b
    cb_opcode[0x21] = [this]()->int{int k=0; if(c&0x80) k=0x10; c=(c<<1)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_c
    cb_opcode[0x22] = [this]()->int{int k=0; if(d&0x80) k=0x10; d=(d<<1)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_d
    cb_opcode[0x23] = [this]()->int{int k=0; if(e&0x80) k=0x10; e=(e<<1)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_e
    cb_opcode[0x24] = [this]()->int{int k=0; if(h&0x80) k=0x10; h=(h<<1)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_h
    cb_opcode[0x25] = [this]()->int{int k=0; if(l&0x80) k=0x10; l=(l<<1)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_l
    cb_opcode[0x27] = [this]()->int{int k=0; if(a&0x80) k=0x10; a=(a<<1)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+k; return 2;}; //SLAr_a
    cb_opcode[0x28] = [this]()->int{int k=b&0x80; int t=0; if(b&0x01) t=0x10; b=((b>>1)+k)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_b
    cb_opcode[0x29] = [this]()->int{int k=c&0x80; int t=0; if(c&0x01) t=0x10; c=((c>>1)+k)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_c
    cb_opcode[0x2A] = [this]()->int{int k=d&0x80; int t=0; if(d&0x01) t=0x10; d=((d>>1)+k)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_d
    cb_opcode[0x2B] = [this]()->int{int k=e&0x80; int t=0; if(e&0x01) t=0x10; e=((e>>1)+k)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_e
    cb_opcode[0x2C] = [this]()->int{int k=h&0x80; int t=0; if(h&0x01) t=0x10; h=((h>>1)+k)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_h
    cb_opcode[0x2D] = [this]()->int{int k=l&0x80; int t=0; if(l&0x01) t=0x10; l=((l>>1)+k)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_l
    cb_opcode[0x2F] = [this]()->int{int k=a&0x80; int t=0; if(a&0x01) t=0x10; a=((a>>1)+k)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; return 2;}; //SRAr_a
    cb_opcode[0x30] = [this]()->int{int k=b; b=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!b) f|=0x80; return 1;}; //SWAPr_b
    cb_opcode[0x31] = [this]()->int{int k=c; c=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!c) f|=0x80; return 1;}; //SWAPr_c
    cb_opcode[0x32] = [this]()->int{int k=d; d=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!d) f|=0x80; return 1;}; //SWAPr_d
    cb_opcode[0x33] = [this]()->int{int k=e; e=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!e) f|=0x80; return 1;}; //SWAPr_e
    cb_opcode[0x34] = [this]()->int{int k=h; h=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!h) f|=0x80; return 1;}; //SWAPr_h
    cb_opcode[0x35] = [this]()->int{int k=l; l=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!l) f|=0x80; return 1;}; //SWAPr_l
    cb_opcode[0x37] = [this]()->int{int k=a; a=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!a) f|=0x80; return 1;}; //SWAPr_a
    cb_opcode[0x38] = [this]()->int{int k=0; if(b&0x01) k=0x10; b=(b>>1)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_b
    cb_opcode[0x39] = [this]()->int{int k=0; if(c&0x01) k=0x10; c=(c>>1)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_c
    cb_opcode[0x3A] = [this]()->int{int k=0; if(d&0x01) k=0x10; d=(d>>1)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_d
    cb_opcode[0x3B] = [this]()->int{int k=0; if(e&0x01) k=0x10; e=(e>>1)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_e
    cb_opcode[0x3C] = [this]()->int{int k=0; if(h&0x01) k=0x10; h=(h>>1)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_h
    cb_opcode[0x3D] = [this]()->int{int k=0; if(l&0x01) k=0x10; l=(l>>1)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_l
    cb_opcode[0x3F] = [this]()->int{int k=0; if(a&0x01) k=0x10; a=(a>>1)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+k; return 2;}; //SRLr_a
    cb_opcode[0x40] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x01)) f|=0x80; return 2;}; //BIT0b
    cb_opcode[0x41] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x01)) f|=0x80; return 2;}; //BIT0c
    cb_opcode[0x42] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x01)) f|=0x80; return 2;}; //BIT0d
    cb_opcode[0x43] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x01)) f|=0x80; return 2;}; //BIT0e
    cb_opcode[0x44] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x01)) f|=0x80; return 2;}; //BIT0h
    cb_opcode[0x45] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x01)) f|=0x80; return 2;}; //BIT0l
    cb_opcode[0x46] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x01)) f|=0x80; return 3;}; //BIT0m
    cb_opcode[0x47] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x01)) f|=0x80; return 2;}; //BIT0a
    cb_opcode[0x48] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x02)) f|=0x80; return 2;}; //BIT1b
    cb_opcode[0x49] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x02)) f|=0x80; return 2;}; //BIT1c
    cb_opcode[0x4A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x02)) f|=0x80; return 2;}; //BIT1d
    cb_opcode[0x4B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x02)) f|=0x80; return 2;}; //BIT1e
    cb_opcode[0x4C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x02)) f|=0x80; return 2;}; //BIT1h
    cb_opcode[0x4D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x02)) f|=0x80; return 2;}; //BIT1l
    cb_opcode[0x4E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x02)) f|=0x80; return 3;}; //BIT1m
    cb_opcode[0x4F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x02)) f|=0x80; return 2;}; //BIT1a
    cb_opcode[0x50] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x04)) f|=0x80; return 2;}; //BIT2b
    cb_opcode[0x51] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x04)) f|=0x80; return 2;}; //BIT2c
    cb_opcode[0x52] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x04)) f|=0x80; return 2;}; //BIT2d
    cb_opcode[0x53] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x04)) f|=0x80; return 2;}; //BIT2e
    cb_opcode[0x54] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x04)) f|=0x80; return 2;}; //BIT2h
    cb_opcode[0x55] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x04)) f|=0x80; return 2;}; //BIT2l
    cb_opcode[0x56] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x04)) f|=0x80; return 3;}; //BIT2m
    cb_opcode[0x57] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x04)) f|=0x80; return 2;}; //BIT2a
    cb_opcode[0x58] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x08)) f|=0x80; return 2;}; //BIT3b
    cb_opcode[0x59] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x08)) f|=0x80; return 2;}; //BIT3c
    cb_opcode[0x5A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x08)) f|=0x80; return 2;}; //BIT3d
    cb_opcode[0x5B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x08)) f|=0x80; return 2;}; //BIT3e
    cb_opcode[0x5C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x08)) f|=0x80; return 2;}; //BIT3h
    cb_opcode[0x5D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x08)) f|=0x80; return 2;}; //BIT3l
    cb_opcode[0x5E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x08)) f|=0x80; return 3;}; //BIT3m
    cb_opcode[0x5F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x08)) f|=0x80; return 2;}; //BIT3a
    cb_opcode[0x60] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x10)) f|=0x80; return 2;}; //BIT4b
    cb_opcode[0x61] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x10)) f|=0x80; return 2;}; //BIT4c
    cb_opcode[0x62] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x10)) f|=0x80; return 2;}; //BIT4d
    cb_opcode[0x63] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x10)) f|=0x80; return 2;}; //BIT4e
    cb_opcode[0x64] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x10)) f|=0x80; return 2;}; //BIT4h
    cb_opcode[0x65] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x10)) f|=0x80; return 2;}; //BIT4l
    cb_opcode[0x66] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x10)) f|=0x80; return 3;}; //BIT4m
    cb_opcode[0x67] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x10)) f|=0x80; return 2;}; //BIT4a
    cb_opcode[0x68] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x20)) f|=0x80; return 2;}; //BIT5b
    cb_opcode[0x69] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x20)) f|=0x80; return 2;}; //BIT5c
    cb_opcode[0x6A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x20)) f|=0x80; return 2;}; //BIT5d
    cb_opcode[0x6B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x20)) f|=0x80; return 2;}; //BIT5e
    cb_opcode[0x6C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x20)) f|=0x80; return 2;}; //BIT5h
    cb_opcode[0x6D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x20)) f|=0x80; return 2;}; //BIT5l
    cb_opcode[0x6E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x20)) f|=0x80; return 3;}; //BIT5m
    cb_opcode[0x6F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x20)) f|=0x80; return 2;}; //BIT5a
    cb_opcode[0x70] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x40)) f|=0x80; return 2;}; //BIT6b
    cb_opcode[0x71] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x40)) f|=0x80; return 2;}; //BIT6c
    cb_opcode[0x72] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x40)) f|=0x80; return 2;}; //BIT6d
    cb_opcode[0x73] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x40)) f|=0x80; return 2;}; //BIT6e
    cb_opcode[0x74] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x40)) f|=0x80; return 2;}; //BIT6h
    cb_opcode[0x75] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x40)) f|=0x80; return 2;}; //BIT6l
    cb_opcode[0x76] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x40)) f|=0x80; return 3;}; //BIT6m
    cb_opcode[0x77] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x40)) f|=0x80; return 2;}; //BIT6a
    cb_opcode[0x78] = [this]()->int{f&=0x1F; f|=0x20; if(!(b&0x80)) f|=0x80; return 2;}; //BIT7b
    cb_opcode[0x79] = [this]()->int{f&=0x1F; f|=0x20; if(!(c&0x80)) f|=0x80; return 2;}; //BIT7c
    cb_opcode[0x7A] = [this]()->int{f&=0x1F; f|=0x20; if(!(d&0x80)) f|=0x80; return 2;}; //BIT7d
    cb_opcode[0x7B] = [this]()->int{f&=0x1F; f|=0x20; if(!(e&0x80)) f|=0x80; return 2;}; //BIT7e
    cb_opcode[0x7C] = [this]()->int{f&=0x1F; f|=0x20; if(!(h&0x80)) f|=0x80; return 2;}; //BIT7h
    cb_opcode[0x7D] = [this]()->int{f&=0x1F; f|=0x20; if(!(l&0x80)) f|=0x80; return 2;}; //BIT7l
    cb_opcode[0x7E] = [this]()->int{f&=0x1F; f|=0x20; if(!(memory.readByte((h<<8)+l)&0x80)) f|=0x80; return 3;}; //BIT7m
    cb_opcode[0x7F] = [this]()->int{f&=0x1F; f|=0x20; if(!(a&0x80)) f|=0x80; return 2;}; //BIT7a
    cb_opcode[0x80] = [this]()->int{b&=0xFE; return 2;}; //RES0b
    cb_opcode[0x81] = [this]()->int{c&=0xFE; return 2;}; //RES0c
    cb_opcode[0x82] = [this]()->int{d&=0xFE; return 2;}; //RES0d
    cb_opcode[0x83] = [this]()->int{e&=0xFE; return 2;}; //RES0e
    cb_opcode[0x84] = [this]()->int{h&=0xFE; return 2;}; //RES0h
    cb_opcode[0x85] = [this]()->int{l&=0xFE; return 2;}; //RES0l
    cb_opcode[0x86] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xFE; memory.writeByte((h<<8)+l, k); return 4;}; //RES0m
    cb_opcode[0x87] = [this]()->int{a&=0xFE; return 2;}; //RES0a
    cb_opcode[0x88] = [this]()->int{b&=0xFD; return 2;}; //RES1b
    cb_opcode[0x89] = [this]()->int{c&=0xFD; return 2;}; //RES1c
    cb_opcode[0x8A] = [this]()->int{d&=0xFD; return 2;}; //RES1d
    cb_opcode[0x8B] = [this]()->int{e&=0xFD; return 2;}; //RES1e
    cb_opcode[0x8C] = [this]()->int{h&=0xFD; return 2;}; //RES1h
    cb_opcode[0x8D] = [this]()->int{l&=0xFD; return 2;}; //RES1l
    cb_opcode[0x8E] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xFD; memory.writeByte((h<<8)+l, k); return 4;}; //RES1m
    cb_opcode[0x8F] = [this]()->int{a&=0xFD; return 2;}; //RES1a
    cb_opcode[0x90] = [this]()->int{b&=0xFB; return 2;}; //RES2b
    cb_opcode[0x91] = [this]()->int{c&=0xFB; return 2;}; //RES2c
    cb_opcode[0x92] = [this]()->int{d&=0xFB; return 2;}; //RES2d
    cb_opcode[0x93] = [this]()->int{e&=0xFB; return 2;}; //RES2e
    cb_opcode[0x94] = [this]()->int{h&=0xFB; return 2;}; //RES2h
    cb_opcode[0x95] = [this]()->int{l&=0xFB; return 2;}; //RES2l
    cb_opcode[0x96] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xFB; memory.writeByte((h<<8)+l, k); return 4;}; //RES2m
    cb_opcode[0x97] = [this]()->int{a&=0xFB; return 2;}; //RES2a
    cb_opcode[0x98] = [this]()->int{b&=0xF7; return 2;}; //RES3b
    cb_opcode[0x99] = [this]()->int{c&=0xF7; return 2;}; //RES3c
    cb_opcode[0x9A] = [this]()->int{d&=0xF7; return 2;}; //RES3d
    cb_opcode[0x9B] = [this]()->int{e&=0xF7; return 2;}; //RES3e
    cb_opcode[0x9C] = [this]()->int{h&=0xF7; return 2;}; //RES3h
    cb_opcode[0x9D] = [this]()->int{l&=0xF7; return 2;}; //RES3l
    cb_opcode[0x9E] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xF7; memory.writeByte((h<<8)+l, k); return 4;}; //RES3m
    cb_opcode[0x9F] = [this]()->int{a&=0xF7; return 2;}; //RES3a
    cb_opcode[0xA0] = [this]()->int{b&=0xEF; return 2;}; //RES4b
    cb_opcode[0xA1] = [this]()->int{c&=0xEF; return 2;}; //RES4c
    cb_opcode[0xA2] = [this]()->int{d&=0xEF; return 2;}; //RES4d
    cb_opcode[0xA3] = [this]()->int{e&=0xEF; return 2;}; //RES4e
    cb_opcode[0xA4] = [this]()->int{h&=0xEF; return 2;}; //RES4h
    cb_opcode[0xA5] = [this]()->int{l&=0xEF; return 2;}; //RES4l
    cb_opcode[0xA6] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xEF; memory.writeByte((h<<8)+l, k); return 4;}; //RES4m
    cb_opcode[0xA7] = [this]()->int{a&=0xEF; return 2;}; //RES4a
    cb_opcode[0xA8] = [this]()->int{b&=0xDF; return 2;}; //RES5b
    cb_opcode[0xA9] = [this]()->int{c&=0xDF; return 2;}; //RES5c
    cb_opcode[0xAA] = [this]()->int{d&=0xDF; return 2;}; //RES5d
    cb_opcode[0xAB] = [this]()->int{e&=0xDF; return 2;}; //RES5e
    cb_opcode[0xAC] = [this]()->int{h&=0xDF; return 2;}; //RES5h
    cb_opcode[0xAD] = [this]()->int{l&=0xDF; return 2;}; //RES5l
    cb_opcode[0xAE] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xDF; memory.writeByte((h<<8)+l, k); return 4;}; //RES5m
    cb_opcode[0xAF] = [this]()->int{a&=0xDF; return 2;}; //RES5a
    cb_opcode[0xB0] = [this]()->int{b&=0xBF; return 2;}; //RES6b
    cb_opcode[0xB1] = [this]()->int{c&=0xBF; return 2;}; //RES6c
    cb_opcode[0xB2] = [this]()->int{d&=0xBF; return 2;}; //RES6d
    cb_opcode[0xB3] = [this]()->int{e&=0xBF; return 2;}; //RES6e
    cb_opcode[0xB4] = [this]()->int{h&=0xBF; return 2;}; //RES6h
    cb_opcode[0xB5] = [this]()->int{l&=0xBF; return 2;}; //RES6l
    cb_opcode[0xB6] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0xBF; memory.writeByte((h<<8)+l, k); return 4;}; //RES6m
    cb_opcode[0xB7] = [this]()->int{a&=0xBF; return 2;}; //RES6a
    cb_opcode[0xB8] = [this]()->int{b&=0x7F; return 2;}; //RES7b
    cb_opcode[0xB9] = [this]()->int{c&=0x7F; return 2;}; //RES7c
    cb_opcode[0xBA] = [this]()->int{d&=0x7F; return 2;}; //RES7d
    cb_opcode[0xBB] = [this]()->int{e&=0x7F; return 2;}; //RES7e
    cb_opcode[0xBC] = [this]()->int{h&=0x7F; return 2;}; //RES7h
    cb_opcode[0xBD] = [this]()->int{l&=0x7F; return 2;}; //RES7l
    cb_opcode[0xBE] = [this]()->int{int k=memory.readByte((h<<8)+l); k&=0x7F; memory.writeByte((h<<8)+l, k); return 4;}; //RES7m
    cb_opcode[0xBF] = [this]()->int{a&=0x7F; return 2;}; //RES7a
    cb_opcode[0xC0] = [this]()->int{b|=0x01; return 2;}; //SET0b
    cb_opcode[0xC1] = [this]()->int{c|=0x01; return 2;}; //SET0c
    cb_opcode[0xC2] = [this]()->int{d|=0x01; return 2;}; //SET0d
    cb_opcode[0xC3] = [this]()->int{e|=0x01; return 2;}; //SET0e
    cb_opcode[0xC4] = [this]()->int{h|=0x01; return 2;}; //SET0h
    cb_opcode[0xC5] = [this]()->int{l|=0x01; return 2;}; //SET0l
    cb_opcode[0xC6] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x01; memory.writeByte((h<<8)+l, k); return 4;}; //SET0m
    cb_opcode[0xC7] = [this]()->int{a|=0x01; return 2;}; //SET0a
    cb_opcode[0xC8] = [this]()->int{b|=0x02; return 2;}; //SET1b
    cb_opcode[0xC9] = [this]()->int{c|=0x02; return 2;}; //SET1c
    cb_opcode[0xCA] = [this]()->int{d|=0x02; return 2;}; //SET1d
    cb_opcode[0xCB] = [this]()->int{e|=0x02; return 2;}; //SET1e
    cb_opcode[0xCC] = [this]()->int{h|=0x02; return 2;}; //SET1h
    cb_opcode[0xCD] = [this]()->int{l|=0x02; return 2;}; //SET1l
    cb_opcode[0xCE] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x02; memory.writeByte((h<<8)+l, k); return 4;}; //SET1m
    cb_opcode[0xCF] = [this]()->int{a|=0x02; return 2;}; //SET1a
    cb_opcode[0xD0] = [this]()->int{b|=0x04; return 2;}; //SET2b
    cb_opcode[0xD1] = [this]()->int{c|=0x04; return 2;}; //SET2c
    cb_opcode[0xD2] = [this]()->int{d|=0x04; return 2;}; //SET2d
    cb_opcode[0xD3] = [this]()->int{e|=0x04; return 2;}; //SET2e
    cb_opcode[0xD4] = [this]()->int{h|=0x04; return 2;}; //SET2h
    cb_opcode[0xD5] = [this]()->int{l|=0x04; return 2;}; //SET2l
    cb_opcode[0xD6] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x04; memory.writeByte((h<<8)+l, k); return 4;}; //SET2m
    cb_opcode[0xD7] = [this]()->int{a|=0x04; return 2;}; //SET2a
    cb_opcode[0xD8] = [this]()->int{b|=0x08; return 2;}; //SET3b
    cb_opcode[0xD9] = [this]()->int{c|=0x08; return 2;}; //SET3c
    cb_opcode[0xDA] = [this]()->int{d|=0x08; return 2;}; //SET3d
    cb_opcode[0xDB] = [this]()->int{e|=0x08; return 2;}; //SET3e
    cb_opcode[0xDC] = [this]()->int{h|=0x08; return 2;}; //SET3h
    cb_opcode[0xDD] = [this]()->int{l|=0x08; return 2;}; //SET3l
    cb_opcode[0xDE] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x08; memory.writeByte((h<<8)+l, k); return 4;}; //SET3m
    cb_opcode[0xDF] = [this]()->int{a|=0x08; return 2;}; //SET3a
    cb_opcode[0xE0] = [this]()->int{b|=0x10; return 2;}; //SET4b
    cb_opcode[0xE1] = [this]()->int{c|=0x10; return 2;}; //SET4c
    cb_opcode[0xE2] = [this]()->int{d|=0x10; return 2;}; //SET4d
    cb_opcode[0xE3] = [this]()->int{e|=0x10; return 2;}; //SET4e
    cb_opcode[0xE4] = [this]()->int{h|=0x10; return 2;}; //SET4h
    cb_opcode[0xE5] = [this]()->int{l|=0x10; return 2;}; //SET4l
    cb_opcode[0xE6] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x10; memory.writeByte((h<<8)+l, k); return 4;}; //SET4m
    cb_opcode[0xE7] = [this]()->int{a|=0x10; return 2;}; //SET4a
    cb_opcode[0xE8] = [this]()->int{b|=0x20; return 2;}; //SET5b
    cb_opcode[0xE9] = [this]()->int{c|=0x20; return 2;}; //SET5c
    cb_opcode[0xEA] = [this]()->int{d|=0x20; return 2;}; //SET5d
    cb_opcode[0xEB] = [this]()->int{e|=0x20; return 2;}; //SET5e
    cb_opcode[0xEC] = [this]()->int{h|=0x20; return 2;}; //SET5h
    cb_opcode[0xED] = [this]()->int{l|=0x20; return 2;}; //SET5l
    cb_opcode[0xEE] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x20; memory.writeByte((h<<8)+l, k); return 4;}; //SET5m
    cb_opcode[0xEF] = [this]()->int{a|=0x20; return 2;}; //SET5a
    cb_opcode[0xF0] = [this]()->int{b|=0x40; return 2;}; //SET6b
    cb_opcode[0xF1] = [this]()->int{c|=0x40; return 2;}; //SET6c
    cb_opcode[0xF2] = [this]()->int{d|=0x40; return 2;}; //SET6d
    cb_opcode[0xF3] = [this]()->int{e|=0x40; return 2;}; //SET6e
    cb_opcode[0xF4] = [this]()->int{h|=0x40; return 2;}; //SET6h
    cb_opcode[0xF5] = [this]()->int{l|=0x40; return 2;}; //SET6l
    cb_opcode[0xF6] = [this]()->int{int k=memory.readByte((h<<8)+l); k|=0x40; memory.writeByte((h<<8)+l, k); return 4;}; //SET6m
    cb_opcode[0xF7] = [this]()->int{a|=0x40; return 2;}; //SET6a
    cb_opcode[0xF8] = [this]()->int{b|=0x80; return 2;}; //SET7b
    cb_opcode[0xF9] = [this]()->int{c|=0x80; return 2;}; //SET7c
    cb_opcode[0xFA] = [this]()->int{d|=0x80; return 2;}; //SET7d
    cb_opcode[0xFB] = [this]()->int{e|=0x80; return 2;}; //SET7e
    cb_opcode[0xFC] = [this]()->int{h|=0x80; return 2;}; //SET7h
    cb_opcode[0xFD] = [this]()->int{l|=0x80; return 2;}; //SET7l
    cb_opcode[0xFE] = [this]{int k=memory.readByte((h<<8)+l); k|=0x80; memory.writeByte((h<<8)+l, k); return 4;}; //SET7m
    cb_opcode[0xFF] = [this]()->int{a|=0x80; return 2;}; //SET7a
}
