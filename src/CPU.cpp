using namespace std;

void init() {
    a=0, b=0, c=0, d=0, e=0, h=0, l=0, f=0;
    sp=0, pc=0;
    cur_m=0, tot_m=0;
    ime=1;
}

//问题1: 关于f的0x80位
//问题2: 忽略的指令 总数是否符合
//问题3: 全局替换Memory
{
ops[0x40] = [this]{b=b; cur_m=1;} //LDrr_bb
ops[0x41] = [this]{b=c; cur_m=1;} //LDrr_bc
ops[0x42] = [this]{b=d; cur_m=1;} //LDrr_bd
ops[0x43] = [this]{b=e; cur_m=1;} //LDrr_be
ops[0x44] = [this]{b=h; cur_m=1;} //LDrr_bh
ops[0x45] = [this]{b=l; cur_m=1;} //LDrr_bl
ops[0x47] = [this]{b=a; cur_m=1;} //LDrr_ba
ops[0x48] = [this]{c=b; cur_m=1;} //LDrr_cb
ops[0x49] = [this]{c=c; cur_m=1;} //LDrr_cc
ops[0x4A] = [this]{c=d; cur_m=1;} //LDrr_cd
ops[0x4B] = [this]{c=e; cur_m=1;} //LDrr_ce
ops[0x4C] = [this]{c=h; cur_m=1;} //LDrr_ch
ops[0x4D] = [this]{c=l; cur_m=1;} //LDrr_cl
ops[0x4F] = [this]{c=a; cur_m=1;} //LDrr_ca
ops[0x50] = [this]{d=b; cur_m=1;} //LDrr_db
ops[0x51] = [this]{d=c; cur_m=1;} //LDrr_dc
ops[0x52] = [this]{d=d; cur_m=1;} //LDrr_dd
ops[0x53] = [this]{d=e; cur_m=1;} //LDrr_de
ops[0x54] = [this]{d=h; cur_m=1;} //LDrr_dh
ops[0x55] = [this]{d=l; cur_m=1;} //LDrr_dl
ops[0x57] = [this]{d=a; cur_m=1;} //LDrr_da
ops[0x58] = [this]{e=b; cur_m=1;} //LDrr_eb
ops[0x59] = [this]{e=c; cur_m=1;} //LDrr_ec
ops[0x5A] = [this]{e=d; cur_m=1;} //LDrr_ed
ops[0x5B] = [this]{e=e; cur_m=1;} //LDrr_ee
ops[0x5C] = [this]{e=h; cur_m=1;} //LDrr_eh
ops[0x5D] = [this]{e=l; cur_m=1;} //LDrr_el
ops[0x5F] = [this]{e=a; cur_m=1;} //LDrr_ea
ops[0x60] = [this]{h=b; cur_m=1;} //LDrr_hb
ops[0x61] = [this]{h=c; cur_m=1;} //LDrr_hc
ops[0x62] = [this]{h=d; cur_m=1;} //LDrr_hd
ops[0x63] = [this]{h=e; cur_m=1;} //LDrr_he
ops[0x64] = [this]{h=h; cur_m=1;} //LDrr_hh
ops[0x65] = [this]{h=l; cur_m=1;} //LDrr_hl
ops[0x67] = [this]{h=a; cur_m=1;} //LDrr_ha
ops[0x68] = [this]{l=b; cur_m=1;} //LDrr_lb
ops[0x69] = [this]{l=c; cur_m=1;} //LDrr_lc
ops[0x6A] = [this]{l=d; cur_m=1;} //LDrr_ld
ops[0x6B] = [this]{l=e; cur_m=1;} //LDrr_le
ops[0x6C] = [this]{l=h; cur_m=1;} //LDrr_lh
ops[0x6D] = [this]{l=l; cur_m=1;} //LDrr_ll
ops[0x6F] = [this]{l=a; cur_m=1;} //LDrr_la
ops[0x78] = [this]{a=b; cur_m=1;} //LDrr_ab
ops[0x79] = [this]{a=c; cur_m=1;} //LDrr_ac
ops[0x7A] = [this]{a=d; cur_m=1;} //LDrr_ad
ops[0x7B] = [this]{a=e; cur_m=1;} //LDrr_ae
ops[0x7C] = [this]{a=h; cur_m=1;} //LDrr_ah
ops[0x7D] = [this]{a=l; cur_m=1;} //LDrr_al
ops[0x7F] = [this]{a=a; cur_m=1;} //LDrr_aa
ops[0x46] = [this]{b=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_b
ops[0x4E] = [this]{c=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_c
ops[0x56] = [this]{d=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_d
ops[0x5E] = [this]{e=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_e
ops[0x66] = [this]{h=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_h
ops[0x6E] = [this]{l=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_l
ops[0x7E] = [this]{b=Memory.rb((h<<8)+l)); cur_m=2;} //LDrHLm_a
ops[0x70] = [this]{Memory.wb((h<<8)+l, b); cur_m=2;} //LDHLmr_b
ops[0x71] = [this]{Memory.wb((h<<8)+l, c); cur_m=2;} //LDHLmr_c
ops[0x72] = [this]{Memory.wb((h<<8)+l, d); cur_m=2;} //LDHLmr_d
ops[0x73] = [this]{Memory.wb((h<<8)+l, e); cur_m=2;} //LDHLmr_e
ops[0x74] = [this]{Memory.wb((h<<8)+l, h); cur_m=2;} //LDHLmr_h
ops[0x75] = [this]{Memory.wb((h<<8)+l, l); cur_m=2;} //LDHLmr_l
ops[0x77] = [this]{Memory.wb((h<<8)+l, a); cur_m=2;} //LDHLmr_a
ops[0x06] = [this]{b=Memory.rb(pc++); cur_m=2;} //LDrn_b
ops[0x0E] = [this]{c=Memory.rb(pc++); cur_m=2;} //LDrn_c
ops[0x16] = [this]{d=Memory.rb(pc++); cur_m=2;} //LDrn_d
ops[0x1E] = [this]{e=Memory.rb(pc++); cur_m=2;} //LDrn_e
ops[0x26] = [this]{h=Memory.rb(pc++); cur_m=2;} //LDrn_h
ops[0x2E] = [this]{l=Memory.rb(pc++); cur_m=2;} //LDrn_l
ops[0x3E] = [this]{a=Memory.rb(pc++); cur_m=2;} //LDrn_a
ops[0x36] = [this]{Memory.wb((h<<8)+l, Memory.rb(pc++)); cur_m=3;} //LDHLmn
ops[0x02] = [this]{Memory.wb((b<<8)+c, a); cur_m=2;} //LDBCmA
ops[0x12] = [this]{Memory.wb((d<<8)+e, a); cur_m=2;} //LDDEmA
ops[0xEA] = [this]{Memory.wb(Memory.rw(pc), a); pc+=2; cur_m=4;} //LDmmA
ops[0x0A] = [this]{a=Memory.rb((b<<8)+c); cur_m=2;} //LDABCm
ops[0x1A] = [this]{a=Memory.rb((d<<8)+e); cur_m=2;} //LDADEm
ops[0xFA] = [this]{a=Memory.rb(Memory.rw(pc)); pc+=2; cur_m=4;} //LDAmm
ops[0x01] = [this]{c=Memory.rb(pc); b=Memory.rb(pc+1); pc+=2; cur_m=3;} //LDBCnn
ops[0x11] = [this]{e=Memory.rb(pc); d=Memory.rb(pc+1); pc+=2; cur_m=3;} //LDDEnn
ops[0x21] = [this]{l=Memory.rb(pc); h=Memory.rb(pc+1); pc+=2; cur_m=3;} //LDHLnn
ops[0x31] = [this]{sp=Memory.rw(pc); pc+=2; cur_m=3;} //LDSPnn
ops[0x36] = [this]{int k=Memory.rw(pc); l=Memory.rb(k); h=Memory.rb(k+1); pc+=2; cur_m=5;} //LDHLmm
//ops[0x36] = [this]{int k=Memory.rw(pc); Memory.ww(k, (h<<8)+l); pc+=2; cur_m=5;} //LDmmHL
ops[0x22] = [this]{Memory.wb((h<<8)+l, a); l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; cur_m=2;} //LDHLIA
ops[0x2A] = [this]{a=Memory.rb((h<<8)+l); l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; cur_m=2;} //LDAHLI
ops[0x32] = [this]{Memory.wb((h<<8)+l, a); l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; cur_m=2;} //LDHLDA
ops[0x3A] = [this]{a=Memory.rb((h<<8)+l); l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; cur_m=2;} //LDAHLD
ops[0xF0] = [this]{a=Memory.rb(0xFF00+Memory.rb(pc++)); cur_m=3;} //LDAIOn
ops[0xE0] = [this]{Memory.wb(0xFF00+Memory.rb(pc++), a); cur_m=3;} //LDIOnA
ops[0xF2] = [this]{a=Memory.rb(0xFF00+c); cur_m=2;} //LDAIOC
ops[0xE2] = [this]{Memory.wb(0xFF00+c, a); cur_m=2;} //LDIOCA
ops[0xF8] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; i+=sp; h=(i>>8)&0xFF; l=i&0xFF; cur_m=3;} //LDHLSPn
cbops[0x30] = [this]{int k=b; b=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!b) f=0x80; cur_m=1;} //SWAPr_b
cbops[0x31] = [this]{int k=c; c=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!c) f=0x80; cur_m=1;} //SWAPr_c
cbops[0x32] = [this]{int k=d; d=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!d) f=0x80; cur_m=1;} //SWAPr_d
cbops[0x33] = [this]{int k=e; e=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!e) f=0x80; cur_m=1;} //SWAPr_e
cbops[0x34] = [this]{int k=h; h=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!h) f=0x80; cur_m=1;} //SWAPr_h
cbops[0x35] = [this]{int k=l; l=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!l) f=0x80; cur_m=1;} //SWAPr_l
cbops[0x37] = [this]{int k=a; a=((k&0xF)<<4)|((k&0xF0)>>4); f=0; if(!a) f=0x80; cur_m=1;} //SWAPr_a
ops[0x80] = [this]{int k=a; a+=b; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; cur_m=1;} //ADDr_b
ops[0x81] = [this]{int k=a; a+=c; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; cur_m=1;} //ADDr_c
ops[0x82] = [this]{int k=a; a+=d; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; cur_m=1;} //ADDr_d
ops[0x83] = [this]{int k=a; a+=e; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; cur_m=1;} //ADDr_e
ops[0x84] = [this]{int k=a; a+=h; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; cur_m=1;} //ADDr_h
ops[0x85] = [this]{int k=a; a+=l; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; cur_m=1;} //ADDr_l
ops[0x87] = [this]{int k=a; a+=a; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; cur_m=1;} //ADDr_a
ops[0x86] = [this]{int k=a; int t=Memory.rb((h<<8)+l); a+=t; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //ADDHL
ops[0xC6] = [this]{int k=a; int t=Memory.rb(pc++); a+=t; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //ADDn
ops[0x09] = [this]{int hl=(h<<8)+l; hl+=(b<<8)+c; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; cur_m=3;} //ADDHLBC
ops[0x19] = [this]{int hl=(h<<8)+l; hl+=(d<<8)+e; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; cur_m=3;} //ADDHLDE
ops[0x29] = [this]{int hl=(h<<8)+l; hl+=(h<<8)+l; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; cur_m=3;} //ADDHLHL
ops[0x39] = [this]{int hl=(h<<8)+l; hl+=sp; if(hl>0xFFFF) f|=0x10; else f&=0xEF; h=(hl>>8)&0xFF; l=hl&0xFF; cur_m=3;} //ADDHLSP
ops[0xE8] = [this]{int k=Memory.rb(pc++); if(k>0x7F) k=-((~k+1)&0xFF); sp+=k; cur_m=4;} //ADDSPn
ops[0x88] = [this]{int k=a; a+=b; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; cur_m=1;} //ADCr_b
ops[0x89] = [this]{int k=a; a+=c; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; cur_m=1;} //ADCr_c
ops[0x8A] = [this]{int k=a; a+=d; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; cur_m=1;} //ADCr_d
ops[0x8B] = [this]{int k=a; a+=e; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; cur_m=1;} //ADCr_e
ops[0x8C] = [this]{int k=a; a+=h; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; cur_m=1;} //ADCr_h
ops[0x8D] = [this]{int k=a; a+=l; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; cur_m=1;} //ADCr_l
ops[0x8F] = [this]{int k=a; a+=a; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; cur_m=1;} //ADCr_a
ops[0x8E] = [this]{int k=a; int t=Memory.rb((h<<8)+l); a+=t; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //ADCHL
ops[0xCE] = [this]{int k=a; int t=Memory.rb(pc++); a+=t; if(f&0x10) a++; f=0; if(a>0xFF) f=0x10; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //ADCn
ops[0x98] = [this]{int k=a; a-=b; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^b^k)&0x10) f|=0x20; cur_m=1;} //SBCr_b
ops[0x99] = [this]{int k=a; a-=c; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^c^k)&0x10) f|=0x20; cur_m=1;} //SBCr_c
ops[0x9A] = [this]{int k=a; a-=d; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^d^k)&0x10) f|=0x20; cur_m=1;} //SBCr_d
ops[0x9B] = [this]{int k=a; a-=e; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^e^k)&0x10) f|=0x20; cur_m=1;} //SBCr_e
ops[0x9C] = [this]{int k=a; a-=h; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^h^k)&0x10) f|=0x20; cur_m=1;} //SBCr_h
ops[0x9D] = [this]{int k=a; a-=l; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^l^k)&0x10) f|=0x20; cur_m=1;} //SBCr_l
ops[0x9F] = [this]{int k=a; a-=a; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^a^k)&0x10) f|=0x20; cur_m=1;} //SBCr_a
ops[0x9E] = [this]{int k=a; int t=Memory.rb((h<<8)+l); a-=t; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //SBCHL
ops[0xDE] = [this]{int k=a; int t=Memory.rb(pc++); a-=t; if(f&0x10) a--; f=0x40; if(a<0) f=0x50; a&=0xFF; if(!a) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //SBCn
ops[0xB8] = [this]{int k=a-b; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^b^k)&0x10) f|=0x20; cur_m=1;} //CPr_b
ops[0xB9] = [this]{int k=a-c; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^c^k)&0x10) f|=0x20; cur_m=1;} //CPr_c
ops[0xBA] = [this]{int k=a-d; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^d^k)&0x10) f|=0x20; cur_m=1;} //CPr_d
ops[0xBB] = [this]{int k=a-e; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^e^k)&0x10) f|=0x20; cur_m=1;} //CPr_e
ops[0xBC] = [this]{int k=a-h; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^h^k)&0x10) f|=0x20; cur_m=1;} //CPr_h
ops[0xBD] = [this]{int k=a-l; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^l^k)&0x10) f|=0x20; cur_m=1;} //CPr_l
ops[0xBF] = [this]{int k=a-a; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^a^k)&0x10) f|=0x20; cur_m=1;} //CPr_a
ops[0xBE] = [this]{int k=a; int t=Memory.rb((h<<8)+l); k-=t; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //CPHL
ops[0xFE] = [this]{int k=a; int t=Memory.rb(pc++); k-=t; f=0x40; if(k<0) f=0x50; k&=0xFF; if(!k) f|=0x80; if((a^k^t)&0x10) f|=0x20; cur_m=2;} //CPn
ops[0x27] = [this]{int k=a; if((f&0x20)||((a&0xF)>9) a+=6; f&=0xEF; if((f&0x20)||(k>0x99)) {a+=0x60; f|=0x10;} cur_m=1;} //DAA
ops[0xA0] = [this]{a&=b; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_b
ops[0xA1] = [this]{a&=c; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_c
ops[0xA2] = [this]{a&=d; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_d
ops[0xA3] = [this]{a&=e; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_e
ops[0xA4] = [this]{a&=h; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_h
ops[0xA5] = [this]{a&=l; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_l
ops[0xA7] = [this]{a&=a; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ANDr_a
ops[0xA6] = [this]{a&=Memory.rb((h<<8)+l); a&=0xFF; f=0; if(!a) f=0x80; cur_m=2;} //ANDHL
ops[0xE6] = [this]{a&=Memory.rb(pc++); a&=0xFF; f=0; if(!a) f=0x80; cur_m=2;} //ANDn
ops[0xB0] = [this]{a|=b; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_b
ops[0xB1] = [this]{a|=c; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_c
ops[0xB2] = [this]{a|=d; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_d
ops[0xB3] = [this]{a|=e; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_e
ops[0xB4] = [this]{a|=h; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_h
ops[0xB5] = [this]{a|=l; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_l
ops[0xB7] = [this]{a|=a; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //ORr_a
ops[0xB6] = [this]{a|=Memory.rb((h<<8)+l); a&=0xFF; f=0; if(!a) f=0x80; cur_m=2;} //ORHL
ops[0xF6] = [this]{a|=Memory.rb(pc++); a&=0xFF; f=0; if(!a) f=0x80; cur_m=2;} //ORn
ops[0xA8] = [this]{a^=b; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_b
ops[0xA9] = [this]{a^=c; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_c
ops[0xAA] = [this]{a^=d; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_d
ops[0xAB] = [this]{a^=e; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_e
ops[0xAC] = [this]{a^=h; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_h
ops[0xAD] = [this]{a^=l; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_l
ops[0xAF] = [this]{a^=a; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //XORr_a
ops[0xAE] = [this]{a^=Memory.rb((h<<8)+l); a&=0xFF; f=0; if(!a) f=0x80; cur_m=2;} //XORHL
ops[0xEE] = [this]{a^=Memory.rb(pc++); a&=0xFF; f=0; if(!a) f=0x80; cur_m=2;} //XORn
ops[0x04] = [this]{b++; b&=0xFF; f=0; if(!b) f=0x80; cur_m=1;} //INCr_b
ops[0x0C] = [this]{c++; c&=0xFF; f=0; if(!c) f=0x80; cur_m=1;} //INCr_c
ops[0x14] = [this]{d++; d&=0xFF; f=0; if(!d) f=0x80; cur_m=1;} //INCr_d
ops[0x1C] = [this]{e++; e&=0xFF; f=0; if(!e) f=0x80; cur_m=1;} //INCr_e
ops[0x24] = [this]{h++; h&=0xFF; f=0; if(!h) f=0x80; cur_m=1;} //INCr_h
ops[0x2C] = [this]{l++; l&=0xFF; f=0; if(!l) f=0x80; cur_m=1;} //INCr_l
ops[0x3C] = [this]{a++; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //INCr_a
ops[0x34] = [this]{int k=Memory.rb((h<<8)+l)+1; k&=0xFF; Memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; cur_m=3;} //INCHLm
ops[0x05] = [this]{b--; b&=0xFF; f=0; if(!b) f=0x80; cur_m=1;} //DECr_b
ops[0x0D] = [this]{c--; c&=0xFF; f=0; if(!c) f=0x80; cur_m=1;} //DECr_c
ops[0x15] = [this]{d--; d&=0xFF; f=0; if(!d) f=0x80; cur_m=1;} //DECr_d
ops[0x1D] = [this]{e--; e&=0xFF; f=0; if(!e) f=0x80; cur_m=1;} //DECr_e
ops[0x25] = [this]{h--; h&=0xFF; f=0; if(!h) f=0x80; cur_m=1;} //DECr_h
ops[0x2D] = [this]{l--; l&=0xFF; f=0; if(!l) f=0x80; cur_m=1;} //DECr_l
ops[0x3D] = [this]{a--; a&=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //DECr_a
ops[0x35] = [this]{int k=Memory.rb((h<<8)+l)-1; k&=0xFF; Memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; cur_m=3;} //DECHLm
ops[0x03] = [this]{c=(c+1)&0xFF; if(!c) b=(b+1)&0xFF; cur_m=1;} //INCBC
ops[0x13] = [this]{e=(e+1)&0xFF; if(!e) d=(d+1)&0xFF; cur_m=1;} //INCDE
ops[0x23] = [this]{l=(l+1)&0xFF; if(!l) h=(h+1)&0xFF; cur_m=1;} //INCHL
ops[0x33] = [this]{sp=(sp+1)&0xFF; cur_m=1;} //INCSP
ops[0x0B] = [this]{c=(c-1)&0xFF; if(c==0xFF) b=(b-1)&0xFF; cur_m=1;} //DECBC
ops[0x1B] = [this]{e=(e-1)&0xFF; if(e==0xFF) d=(d-1)&0xFF; cur_m=1;} //DECDE
ops[0x2B] = [this]{l=(l-1)&0xFF; if(l==0xFF) h=(h-1)&0xFF; cur_m=1;} //DECHL
ops[0x3B] = [this]{sp=(sp-1)&0xFF; cur_m=1;} //DECSP
cbops[0x40] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x01)) f=0x80; cur_m=2;} //BIT0b
cbops[0x41] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x01)) f=0x80; cur_m=2;} //BIT0c
cbops[0x42] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x01)) f=0x80; cur_m=2;} //BIT0d
cbops[0x43] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x01)) f=0x80; cur_m=2;} //BIT0e
cbops[0x44] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x01)) f=0x80; cur_m=2;} //BIT0h
cbops[0x45] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x01)) f=0x80; cur_m=2;} //BIT0l
cbops[0x47] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x01)) f=0x80; cur_m=2;} //BIT0a
cbops[0x46] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x01) f=0x80; cur_m=3;} //BIT0m
cbops[0x80] = [this]{b&=0xFE; cur_m=2;} //RES0b
cbops[0x81] = [this]{c&=0xFE; cur_m=2;} //RES0c
cbops[0x82] = [this]{d&=0xFE; cur_m=2;} //RES0d
cbops[0x83] = [this]{e&=0xFE; cur_m=2;} //RES0e
cbops[0x84] = [this]{h&=0xFE; cur_m=2;} //RES0h
cbops[0x85] = [this]{l&=0xFE; cur_m=2;} //RES0l
cbops[0x87] = [this]{a&=0xFE; cur_m=2;} //RES0a
cbops[0x86] = [this]{int k=Memory.rb((h<<8)+l); k&=0xFE; Memory.wb((h<<8)+l, k); cur_m=4;} //RES0m
cbops[0xC0] = [this]{b|=0x01; cur_m=2;} //SET0b
cbops[0xC1] = [this]{c|=0x01; cur_m=2;} //SET0c
cbops[0xC2] = [this]{d|=0x01; cur_m=2;} //SET0d
cbops[0xC3] = [this]{e|=0x01; cur_m=2;} //SET0e
cbops[0xC4] = [this]{h|=0x01; cur_m=2;} //SET0h
cbops[0xC5] = [this]{l|=0x01; cur_m=2;} //SET0l
cbops[0xC7] = [this]{a|=0x01; cur_m=2;} //SET0a
cbops[0xC6] = [this]{int k=Memory.rb((h<<8)+l); k|=0x01; Memory.wb((h<<8)+l, k); cur_m=4;} //SET0m
cbops[0x48] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x02)) f=0x80; cur_m=2;} //BIT1b
cbops[0x49] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x02)) f=0x80; cur_m=2;} //BIT1c
cbops[0x4A] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x02)) f=0x80; cur_m=2;} //BIT1d
cbops[0x4B] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x02)) f=0x80; cur_m=2;} //BIT1e
cbops[0x4C] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x02)) f=0x80; cur_m=2;} //BIT1h
cbops[0x4D] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x02)) f=0x80; cur_m=2;} //BIT1l
cbops[0x4F] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x02)) f=0x80; cur_m=2;} //BIT1a
cbops[0x4E] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x02)) f=0x80; cur_m=3;} //BIT1m
cbops[0x88] = [this]{b&=0xFD; cur_m=2;} //RES1b
cbops[0x89] = [this]{c&=0xFD; cur_m=2;} //RES1c
cbops[0x8A] = [this]{d&=0xFD; cur_m=2;} //RES1d
cbops[0x8B] = [this]{e&=0xFD; cur_m=2;} //RES1e
cbops[0x8C] = [this]{h&=0xFD; cur_m=2;} //RES1h
cbops[0x8D] = [this]{l&=0xFD; cur_m=2;} //RES1l
cbops[0x8F] = [this]{a&=0xFD; cur_m=2;} //RES1a
cbops[0x8E] = [this]{int k=Memory.rb((h<<8)+l); k&=0xFD; Memory.wb((h<<8)+l, k); cur_m=4;} //RES1m
cbops[0xC8] = [this]{b|=0x02; cur_m=2;} //SET1b
cbops[0xC9] = [this]{c|=0x02; cur_m=2;} //SET1c
cbops[0xCA] = [this]{d|=0x02; cur_m=2;} //SET1d
cbops[0xCB] = [this]{e|=0x02; cur_m=2;} //SET1e
cbops[0xCC] = [this]{h|=0x02; cur_m=2;} //SET1h
cbops[0xCD] = [this]{l|=0x02; cur_m=2;} //SET1l
cbops[0xCF] = [this]{a|=0x02; cur_m=2;} //SET1a
cbops[0xCE] = [this]{int k=Memory.rb((h<<8)+l); k|=0x02; Memory.wb((h<<8)+l, k); cur_m=4;} //SET1m
cbops[0x50] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x04)) f=0x80; cur_m=2;} //BIT2b
cbops[0x51] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x04)) f=0x80; cur_m=2;} //BIT2c
cbops[0x52] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x04)) f=0x80; cur_m=2;} //BIT2d
cbops[0x53] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x04)) f=0x80; cur_m=2;} //BIT2e
cbops[0x54] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x04)) f=0x80; cur_m=2;} //BIT2h
cbops[0x55] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x04)) f=0x80; cur_m=2;} //BIT2l
cbops[0x57] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x04)) f=0x80; cur_m=2;} //BIT2a
cbops[0x56] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x04)) f=0x80; cur_m=3;} //BIT2m
cbops[0x90] = [this]{b&=0xFB; cur_m=2;} //RES2b
cbops[0x91] = [this]{c&=0xFB; cur_m=2;} //RES2c
cbops[0x92] = [this]{d&=0xFB; cur_m=2;} //RES2d
cbops[0x93] = [this]{e&=0xFB; cur_m=2;} //RES2e
cbops[0x94] = [this]{h&=0xFB; cur_m=2;} //RES2h
cbops[0x95] = [this]{l&=0xFB; cur_m=2;} //RES2l
cbops[0x97] = [this]{a&=0xFB; cur_m=2;} //RES2a
cbops[0x96] = [this]{int k=Memory.rb((h<<8)+l); k&=0xFB; Memory.wb((h<<8)+l, k); cur_m=4;} //RES2m
cbops[0xD0] = [this]{b|=0x04; cur_m=2;} //SET2b
cbops[0xD1] = [this]{c|=0x04; cur_m=2;} //SET2c
cbops[0xD2] = [this]{d|=0x04; cur_m=2;} //SET2d
cbops[0xD3] = [this]{e|=0x04; cur_m=2;} //SET2e
cbops[0xD4] = [this]{h|=0x04; cur_m=2;} //SET2h
cbops[0xD5] = [this]{l|=0x04; cur_m=2;} //SET2l
cbops[0xD7] = [this]{a|=0x04; cur_m=2;} //SET2a
cbops[0xD6] = [this]{int k=Memory.rb((h<<8)+l); k|=0x04; Memory.wb((h<<8)+l, k); cur_m=4;} //SET2m
cbops[0x58] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x08)) f=0x80; cur_m=2;} //BIT3b
cbops[0x59] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x08)) f=0x80; cur_m=2;} //BIT3c
cbops[0x5A] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x08)) f=0x80; cur_m=2;} //BIT3d
cbops[0x5B] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x08)) f=0x80; cur_m=2;} //BIT3e
cbops[0x5C] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x08)) f=0x80; cur_m=2;} //BIT3h
cbops[0x5D] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x08)) f=0x80; cur_m=2;} //BIT3l
cbops[0x5F] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x08)) f=0x80; cur_m=2;} //BIT3a
cbops[0x5E] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x08)) f=0x80; cur_m=3;} //BIT3m
cbops[0x98] = [this]{b&=0xF7; cur_m=2;} //RES3b
cbops[0x99] = [this]{c&=0xF7; cur_m=2;} //RES3c
cbops[0x9A] = [this]{d&=0xF7; cur_m=2;} //RES3d
cbops[0x9B] = [this]{e&=0xF7; cur_m=2;} //RES3e
cbops[0x9C] = [this]{h&=0xF7; cur_m=2;} //RES3h
cbops[0x9D] = [this]{l&=0xF7; cur_m=2;} //RES3l
cbops[0x9F] = [this]{a&=0xF7; cur_m=2;} //RES3a
cbops[0x9E] = [this]{int k=Memory.rb((h<<8)+l); k&=0xF7; Memory.wb((h<<8)+l, k); cur_m=4;} //RES3m
cbops[0xD8] = [this]{b|=0x08; cur_m=2;} //SET3b
cbops[0xD9] = [this]{c|=0x08; cur_m=2;} //SET3c
cbops[0xDA] = [this]{d|=0x08; cur_m=2;} //SET3d
cbops[0xDB] = [this]{e|=0x08; cur_m=2;} //SET3e
cbops[0xDC] = [this]{h|=0x08; cur_m=2;} //SET3h
cbops[0xDD] = [this]{l|=0x08; cur_m=2;} //SET3l
cbops[0xDF] = [this]{a|=0x08; cur_m=2;} //SET3a
cbops[0xDE] = [this]{int k=Memory.rb((h<<8)+l); k|=0x08; Memory.wb((h<<8)+l, k); cur_m=4;} //SET3m
cbops[0x60] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x10)) f=0x80; cur_m=2;} //BIT4b
cbops[0x61] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x10)) f=0x80; cur_m=2;} //BIT4c
cbops[0x62] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x10)) f=0x80; cur_m=2;} //BIT4d
cbops[0x63] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x10)) f=0x80; cur_m=2;} //BIT4e
cbops[0x64] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x10)) f=0x80; cur_m=2;} //BIT4h
cbops[0x65] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x10)) f=0x80; cur_m=2;} //BIT4l
cbops[0x67] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x10)) f=0x80; cur_m=2;} //BIT4a
cbops[0x66] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x10)) f=0x80; cur_m=3;} //BIT4m
cbops[0xA0] = [this]{b&=0xEF; cur_m=2;} //RES4b
cbops[0xA1] = [this]{c&=0xEF; cur_m=2;} //RES4c
cbops[0xA2] = [this]{d&=0xEF; cur_m=2;} //RES4d
cbops[0xA3] = [this]{e&=0xEF; cur_m=2;} //RES4e
cbops[0xA4] = [this]{h&=0xEF; cur_m=2;} //RES4h
cbops[0xA5] = [this]{l&=0xEF; cur_m=2;} //RES4l
cbops[0xA7] = [this]{a&=0xEF; cur_m=2;} //RES4a
cbops[0xA6] = [this]{int k=Memory.rb((h<<8)+l); k&=0xEF; Memory.wb((h<<8)+l, k); cur_m=4;} //RES4m
cbops[0xE0] = [this]{b|=0x10; cur_m=2;} //SET4b
cbops[0xE1] = [this]{c|=0x10; cur_m=2;} //SET4c
cbops[0xE2] = [this]{d|=0x10; cur_m=2;} //SET4d
cbops[0xE3] = [this]{e|=0x10; cur_m=2;} //SET4e
cbops[0xE4] = [this]{h|=0x10; cur_m=2;} //SET4h
cbops[0xE5] = [this]{l|=0x10; cur_m=2;} //SET4l
cbops[0xE7] = [this]{a|=0x10; cur_m=2;} //SET4a
cbops[0xE6] = [this]{int k=Memory.rb((h<<8)+l); k|=0x10; Memory.wb((h<<8)+l, k); cur_m=4;} //SET4m
cbops[0x68] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x20)) f=0x80; cur_m=2;} //BIT5b
cbops[0x69] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x20)) f=0x80; cur_m=2;} //BIT5c
cbops[0x6A] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x20)) f=0x80; cur_m=2;} //BIT5d
cbops[0x6B] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x20)) f=0x80; cur_m=2;} //BIT5e
cbops[0x6C] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x20)) f=0x80; cur_m=2;} //BIT5h
cbops[0x6D] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x20)) f=0x80; cur_m=2;} //BIT5l
cbops[0x6F] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x20)) f=0x80; cur_m=2;} //BIT5a
cbops[0x6E] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x20)) f=0x80; cur_m=3;} //BIT5m
cbops[0xA8] = [this]{b&=0xDF; cur_m=2;} //RES5b
cbops[0xA9] = [this]{c&=0xDF; cur_m=2;} //RES5c
cbops[0xAA] = [this]{d&=0xDF; cur_m=2;} //RES5d
cbops[0xAB] = [this]{e&=0xDF; cur_m=2;} //RES5e
cbops[0xAC] = [this]{h&=0xDF; cur_m=2;} //RES5h
cbops[0xAD] = [this]{l&=0xDF; cur_m=2;} //RES5l
cbops[0xAF] = [this]{a&=0xDF; cur_m=2;} //RES5a
cbops[0xAE] = [this]{int k=Memory.rb((h<<8)+l); k&=0xDF; Memory.wb((h<<8)+l, k); cur_m=4;} //RES5m
cbops[0xE8] = [this]{b|=0x20; cur_m=2;} //SET5b
cbops[0xE9] = [this]{c|=0x20; cur_m=2;} //SET5c
cbops[0xEA] = [this]{d|=0x20; cur_m=2;} //SET5d
cbops[0xEB] = [this]{e|=0x20; cur_m=2;} //SET5e
cbops[0xEC] = [this]{h|=0x20; cur_m=2;} //SET5h
cbops[0xED] = [this]{l|=0x20; cur_m=2;} //SET5l
cbops[0xEF] = [this]{a|=0x20; cur_m=2;} //SET5a
cbops[0xEE] = [this]{int k=Memory.rb((h<<8)+l); k|=0x20; Memory.wb((h<<8)+l, k); cur_m=4;} //SET5m
cbops[0x70] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x40)) f=0x80; cur_m=2;} //BIT6b
cbops[0x71] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x40)) f=0x80; cur_m=2;} //BIT6c
cbops[0x72] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x40)) f=0x80; cur_m=2;} //BIT6d
cbops[0x73] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x40)) f=0x80; cur_m=2;} //BIT6e
cbops[0x74] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x40)) f=0x80; cur_m=2;} //BIT6h
cbops[0x75] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x40)) f=0x80; cur_m=2;} //BIT6l
cbops[0x77] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x40)) f=0x80; cur_m=2;} //BIT6a
cbops[0x76] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x40)) f=0x80; cur_m=3;} //BIT6m
cbops[0xB0] = [this]{b&=0xBF; cur_m=2;} //RES6b
cbops[0xB1] = [this]{c&=0xBF; cur_m=2;} //RES6c
cbops[0xB2] = [this]{d&=0xBF; cur_m=2;} //RES6d
cbops[0xB3] = [this]{e&=0xBF; cur_m=2;} //RES6e
cbops[0xB4] = [this]{h&=0xBF; cur_m=2;} //RES6h
cbops[0xB5] = [this]{l&=0xBF; cur_m=2;} //RES6l
cbops[0xB7] = [this]{a&=0xBF; cur_m=2;} //RES6a
cbops[0xB6] = [this]{int k=Memory.rb((h<<8)+l); k&=0xBF; Memory.wb((h<<8)+l, k); cur_m=4;} //RES6m
cbops[0xF0] = [this]{b|=0x40; cur_m=2;} //SET6b
cbops[0xF1] = [this]{c|=0x40; cur_m=2;} //SET6c
cbops[0xF2] = [this]{d|=0x40; cur_m=2;} //SET6d
cbops[0xF3] = [this]{e|=0x40; cur_m=2;} //SET6e
cbops[0xF4] = [this]{h|=0x40; cur_m=2;} //SET6h
cbops[0xF5] = [this]{l|=0x40; cur_m=2;} //SET6l
cbops[0xF7] = [this]{a|=0x40; cur_m=2;} //SET6a
cbops[0xF6] = [this]{int k=Memory.rb((h<<8)+l); k|=0x40; Memory.wb((h<<8)+l, k); cur_m=4;} //SET6m
cbops[0x78] = [this]{f&=0x1F; f|=0x20; f=0; if(!(b&0x80)) f=0x80; cur_m=2;} //BIT7b
cbops[0x79] = [this]{f&=0x1F; f|=0x20; f=0; if(!(c&0x80)) f=0x80; cur_m=2;} //BIT7c
cbops[0x7A] = [this]{f&=0x1F; f|=0x20; f=0; if(!(d&0x80)) f=0x80; cur_m=2;} //BIT7d
cbops[0x7B] = [this]{f&=0x1F; f|=0x20; f=0; if(!(e&0x80)) f=0x80; cur_m=2;} //BIT7e
cbops[0x7C] = [this]{f&=0x1F; f|=0x20; f=0; if(!(h&0x80)) f=0x80; cur_m=2;} //BIT7h
cbops[0x7D] = [this]{f&=0x1F; f|=0x20; f=0; if(!(l&0x80)) f=0x80; cur_m=2;} //BIT7l
cbops[0x7F] = [this]{f&=0x1F; f|=0x20; f=0; if(!(a&0x80)) f=0x80; cur_m=2;} //BIT7a
cbops[0x7E] = [this]{f&=0x1F; f|=0x20; f=0; if(!(Memory.rb((h<<8)+l)&0x80)) f=0x80; cur_m=3;} //BIT7m
cbops[0xB8] = [this]{b&=0x7F; cur_m=2;} //RES7b
cbops[0xB9] = [this]{c&=0x7F; cur_m=2;} //RES7c
cbops[0xBA] = [this]{d&=0x7F; cur_m=2;} //RES7d
cbops[0xBB] = [this]{e&=0x7F; cur_m=2;} //RES7e
cbops[0xBC] = [this]{h&=0x7F; cur_m=2;} //RES7h
cbops[0xBD] = [this]{l&=0x7F; cur_m=2;} //RES7l
cbops[0xBF] = [this]{a&=0x7F; cur_m=2;} //RES7a
cbops[0xBE] = [this]{int k=Memory.rb((h<<8)+l); k&=0x7F; Memory.wb((h<<8)+l, k); cur_m=4;} //RES7m
cbops[0xF8] = [this]{b|=0x80; cur_m=2;} //SET7b
cbops[0xF9] = [this]{c|=0x80; cur_m=2;} //SET7c
cbops[0xFA] = [this]{d|=0x80; cur_m=2;} //SET7d
cbops[0xFB] = [this]{e|=0x80; cur_m=2;} //SET7e
cbops[0xFC] = [this]{h|=0x80; cur_m=2;} //SET7h
cbops[0xFD] = [this]{l|=0x80; cur_m=2;} //SET7l
cbops[0xFF] = [this]{a|=0x80; cur_m=2;} //SET7a
cbops[0xFE] = [this]{int k=Memory.rb((h<<8)+l); k|=0x80; Memory.wb((h<<8)+l, k); cur_m=4;} //SET7m
ops[0x17] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=(f&0xEF)+t; cur_m=1;} //RLA
ops[0x07] = [this]{int k=0; if(a&0x80) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=(f&0xEF)+t; cur_m=1;} //RLCA
ops[0x1F] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=(f&0xEF)+t; cur_m=1;} //RRA
ops[0x0F] = [this]{int k=0; if(a&0x01) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=(f&0xEF)+t; cur_m=1;} //RRCA
cbops[0x10] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(b&0x80) t=0x10; b=(b<<1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_b
cbops[0x11] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(c&0x80) t=0x10; c=(c<<1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_c
cbops[0x12] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(d&0x80) t=0x10; d=(d<<1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_d
cbops[0x13] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(e&0x80) t=0x10; e=(e<<1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_e
cbops[0x14] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(h&0x80) t=0x10; h=(h<<1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_h
cbops[0x15] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(l&0x80) t=0x10; l=(l<<1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_l
cbops[0x17] = [this]{int k=0; if(f&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLr_a
cbops[0x16] = [this]{int k=Memory.rb((h<<8)+l); int t=0; if(f&0x10) t=1; int u=0; if(k&0x80) u=0x10; k=(k<<1)+t; k&=0xFF; f=0; if(!k) f=0x80; Memory.wb((h<<8)+l, k); f=(f&0xEF)+u; cur_m=4;} //RLHL
cbops[0x00] = [this]{int k=0; if(b&0x80) k=1; int t=0; if(b&0x80) t=0x10; b=(b<<1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_b
cbops[0x01] = [this]{int k=0; if(c&0x10) k=1; int t=0; if(c&0x80) t=0x10; c=(c<<1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_c
cbops[0x02] = [this]{int k=0; if(d&0x10) k=1; int t=0; if(d&0x80) t=0x10; d=(d<<1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_d
cbops[0x03] = [this]{int k=0; if(e&0x10) k=1; int t=0; if(e&0x80) t=0x10; e=(e<<1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_e
cbops[0x04] = [this]{int k=0; if(h&0x10) k=1; int t=0; if(h&0x80) t=0x10; h=(h<<1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_h
cbops[0x05] = [this]{int k=0; if(l&0x10) k=1; int t=0; if(l&0x80) t=0x10; l=(l<<1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_l
cbops[0x07] = [this]{int k=0; if(a&0x10) k=1; int t=0; if(a&0x80) t=0x10; a=(a<<1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RLCr_a
cbops[0x06] = [this]{int k=Memory.rb((h<<8)+l); int t=0; if(k&0x80) t=1; int u=0; if(k&0x80) u=0x10; k=(k<<1)+t; k&=0xFF; f=0; if(!k) f=0x80; Memory.wb((h<<8)+l, k); f=(f&0xEF)+u; cur_m=4;} //RLCHL
cbops[0x18] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(b&0x01) t=0x10; b=(b>>1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_b
cbops[0x19] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(c&0x01) t=0x10; c=(c>>1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_c
cbops[0x1A] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(d&0x01) t=0x10; d=(d>>1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_d
cbops[0x1B] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(e&0x01) t=0x10; e=(e>>1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_e
cbops[0x1C] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(h&0x01) t=0x10; h=(h>>1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_h
cbops[0x1D] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(l&0x01) t=0x10; l=(l>>1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_l
cbops[0x1F] = [this]{int k=0; if(f&0x10) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRr_a
cbops[0x1E] = [this]{int k=Memory.rb((h<<8)+l); int t=0; if(f&0x10) t=0x80; int u=0; if(k&0x01) u=0x10; k=(k>>1)+t; k&=0xFF; Memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; f=(f&0xEF)+u; cur_m=4;} //RRHL
cbops[0x08] = [this]{int k=0; if(b&0x01) k=0x80; int t=0; if(b&0x01) t=0x10; b=(b>>1)+k; b&=0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_b
cbops[0x09] = [this]{int k=0; if(c&0x01) k=0x80; int t=0; if(c&0x01) t=0x10; c=(c>>1)+k; c&=0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_c
cbops[0x0A] = [this]{int k=0; if(d&0x01) k=0x80; int t=0; if(d&0x01) t=0x10; d=(d>>1)+k; d&=0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_d
cbops[0x0B] = [this]{int k=0; if(e&0x01) k=0x80; int t=0; if(e&0x01) t=0x10; e=(e>>1)+k; e&=0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_e
cbops[0x0C] = [this]{int k=0; if(h&0x01) k=0x80; int t=0; if(h&0x01) t=0x10; h=(h>>1)+k; h&=0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_h
cbops[0x0D] = [this]{int k=0; if(l&0x01) k=0x80; int t=0; if(l&0x01) t=0x10; l=(l>>1)+k; l&=0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_l
cbops[0x0F] = [this]{int k=0; if(a&0x01) k=0x80; int t=0; if(a&0x01) t=0x10; a=(a>>1)+k; a&=0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; cur_m=2;} //RRCr_a
cbops[0x0E] = [this]{int k=Memory.rb((h<<8)+l); int t=0; if(k&0x01) t=0x80; int u=0; if(k&0x01) u=0x10; k=(k>>1)+t; k&=0xFF; Memory.wb((h<<8)+l, k); f=0; if(!k) f=0x80; f=(f&0xEF)+u; cur_m=4;} //RRCHL
cbops[0x20] = [this]{int k=0; if(b&0x80) k=0x10; b=(b<<1)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_b
cbops[0x21] = [this]{int k=0; if(c&0x80) k=0x10; c=(c<<1)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_c
cbops[0x22] = [this]{int k=0; if(d&0x80) k=0x10; d=(d<<1)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_d
cbops[0x23] = [this]{int k=0; if(e&0x80) k=0x10; e=(e<<1)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_e
cbops[0x24] = [this]{int k=0; if(h&0x80) k=0x10; h=(h<<1)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_h
cbops[0x25] = [this]{int k=0; if(l&0x80) k=0x10; l=(l<<1)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_l
cbops[0x27] = [this]{int k=0; if(a&0x80) k=0x10; a=(a<<1)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLAr_a
//cbops[0x0E] = [this]{int k=0; if(b&0x80) k=0x10; b=(b<<1)&0xFF+1; f=0; if(!b) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_b
//cbops[0x0E] = [this]{int k=0; if(c&0x80) k=0x10; c=(c<<1)&0xFF+1; f=0; if(!c) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_c
//cbops[0x0E] = [this]{int k=0; if(d&0x80) k=0x10; d=(d<<1)&0xFF+1; f=0; if(!d) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_d
//cbops[0x0E] = [this]{int k=0; if(e&0x80) k=0x10; e=(e<<1)&0xFF+1; f=0; if(!e) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_e
//cbops[0x0E] = [this]{int k=0; if(h&0x80) k=0x10; h=(h<<1)&0xFF+1; f=0; if(!h) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_h
//cbops[0x0E] = [this]{int k=0; if(l&0x80) k=0x10; l=(l<<1)&0xFF+1; f=0; if(!l) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_l
//cbops[0x0E] = [this]{int k=0; if(a&0x80) k=0x10; a=(a<<1)&0xFF+1; f=0; if(!a) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SLLr_a
cbops[0x28] = [this]{int k=b&0x80; int t=0; if(b&0x01) t=0x10; b=((b>>1)+k)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_b
cbops[0x29] = [this]{int k=c&0x80; int t=0; if(c&0x01) t=0x10; c=((c>>1)+k)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_c
cbops[0x2A] = [this]{int k=d&0x80; int t=0; if(d&0x01) t=0x10; d=((d>>1)+k)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_d
cbops[0x2B] = [this]{int k=e&0x80; int t=0; if(e&0x01) t=0x10; e=((e>>1)+k)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_e
cbops[0x2C] = [this]{int k=h&0x80; int t=0; if(h&0x01) t=0x10; h=((h>>1)+k)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_h
cbops[0x2D] = [this]{int k=l&0x80; int t=0; if(l&0x01) t=0x10; l=((l>>1)+k)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_l
cbops[0x2F] = [this]{int k=a&0x80; int t=0; if(a&0x01) t=0x10; a=((a>>1)+k)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+t; cur_m=2;} //SRAr_a
cbops[0x38] = [this]{int k=0; if(b&0x01) k=0x10; b=(b>>1)&0xFF; f=0; if(!b) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_b
cbops[0x39] = [this]{int k=0; if(c&0x01) k=0x10; c=(c>>1)&0xFF; f=0; if(!c) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_c
cbops[0x3A] = [this]{int k=0; if(d&0x01) k=0x10; d=(d>>1)&0xFF; f=0; if(!d) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_d
cbops[0x3B] = [this]{int k=0; if(e&0x01) k=0x10; e=(e>>1)&0xFF; f=0; if(!e) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_e
cbops[0x3C] = [this]{int k=0; if(h&0x01) k=0x10; h=(h>>1)&0xFF; f=0; if(!h) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_h
cbops[0x3D] = [this]{int k=0; if(l&0x01) k=0x10; l=(l>>1)&0xFF; f=0; if(!l) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_l
cbops[0x3F] = [this]{int k=0; if(a&0x01) k=0x10; a=(a>>1)&0xFF; f=0; if(!a) f=0x80; f=(f&0xEF)+k; cur_m=2;} //SRLr_a
ops[0x2F] = [this]{a^=0xFF; f=0; if(!a) f=0x80; cur_m=1;} //CPL
//ops[0x3F] = [this]{a=-a; f=0; if(a<0) f=0x10; a&=0xFF; if(!a) f|=0x80; cur_m=2;} //NEG
ops[0x3F] = [this]{int k=0; if(!(f&0x10)) k=0x10; f=(f&0xEF)+k; cur_m=1;} //CCF
ops[0x37] = [this]{f|=0x10; cur_m=1;} //SCF
ops[0xC5] = [this]{sp--; Memory.wb(sp, b); sp--; Memory.wb(sp, c); cur_m=3;} //PUSHBC
ops[0xD5] = [this]{sp--; Memory.wb(sp, d); sp--; Memory.wb(sp, e); cur_m=3;} //PUSHDE
ops[0xE5] = [this]{sp--; Memory.wb(sp, h); sp--; Memory.wb(sp, l); cur_m=3;} //PUSHHL
ops[0xF5] = [this]{sp--; Memory.wb(sp, a); sp--; Memory.wb(sp, f); cur_m=3;} //PUSHAF
ops[0xC1] = [this]{c=Memory.rb(sp++); b=Memory.rb(sp++); cur_m=3;} //POPBC
ops[0xD1] = [this]{e=Memory.rb(sp++); d=Memory.rb(sp++); cur_m=3;} //POPDE
ops[0xE1] = [this]{l=Memory.rb(sp++); h=Memory.rb(sp++); cur_m=3;} //POPHL
ops[0xF1] = [this]{f=Memory.rb(sp++); a=Memory.rb(sp++); cur_m=3;} //POPAF
ops[0xC3] = [this]{pc=Memory.rw(pc); cur_m=3;} //JPnn
ops[0xE9] = [this]{pc=(h<<8)+l; cur_m=1;} //JPHL
ops[0xC2] = [this]{cur_m=3; if((f&0x80)==0x00) {pc=Memory.rw(pc); cur_m++;} else pc+=2;} //JPNZnn
ops[0xCA] = [this]{cur_m=3; if((f&0x80)==0x80) {pc=Memory.rw(pc); cur_m++;} else pc+=2;} //JPZnn
ops[0xD2] = [this]{cur_m=3; if((f&0x10)==0x00) {pc=Memory.rw(pc); cur_m++;} else pc+=2;} //JPNCnn
ops[0xDA] = [this]{cur_m=3; if((f&0x10)==0x10) {pc=Memory.rw(pc); cur_m++;} else pc+=2;} //JPCnn
ops[0x18] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc+=k+1; cur_m=3;} //JRn
ops[0x20] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; cur_m=2; if((f&0x80)==0x00) {pc+=k; cur_m++;}} //JRNZn
ops[0x28] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; cur_m=2; if((f&0x80)==0x80) {pc+=k; cur_m++;}} //JRZn
ops[0x30] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; cur_m=2; if((f&0x10)==0x00) {pc+=k; cur_m++;}} //JRNCn
ops[0x38] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; cur_m=2; if((f&0x10)==0x10) {pc+=k; cur_m++;}} //JRCn
ops[0x10] = [this]{int k=Memory.rb(pc); if(k>0x7F) k=-((~k+1)&0xFF); pc++; cur_m=2; b--; if(b) {pc+=k; cur_m++;}} //DJNZn
ops[0xCD] = [this]{sp-=2; Memory.ww(sp, pc+2); pc=Memory.rw(pc); cur_m=5;} //CALLnn
ops[0xC4] = [this]{cur_m=3; if((f&0x80)==0x00) {sp-=2; Memory.ww(sp, pc+2); pc=Memory.rw(pc); cur_m+=2;} else pc+=2;} //CALLNZnn
ops[0xCC] = [this]{cur_m=3; if((f&0x80)==0x80) {sp-=2; Memory.ww(sp, pc+2); pc=Memory.rw(pc); cur_m+=2;} else pc+=2;} //CALLZnn
ops[0xD4] = [this]{cur_m=3; if((f&0x10)==0x00) {sp-=2; Memory.ww(sp, pc+2); pc=Memory.rw(pc); cur_m+=2;} else pc+=2;} //CALLNCnn
ops[0xDB] = [this]{cur_m=3; if((f&0x10)==0x10) {sp-=2; Memory.ww(sp, pc+2); pc=Memory.rw(pc); cur_m+=2;} else pc+=2;} //CALLCnn
ops[0xC9] = [this]{pc=Memory.rw(sp); sp+=2; cur_m=3;} //RET
ops[0xD9] = [this]{ime=1; restore(); pc=Memory.rw(sp); sp+=2; cur_m=3;} //RETI
ops[0xC0] = [this]{cur_m=1; if((f&0x80)==0x00) {pc=Memory.rw(sp); sp+=2; cur_m+=2;}} //RETNZ
ops[0xC8] = [this]{cur_m=1; if((f&0x80)==0x80) {pc=Memory.rw(sp); sp+=2; cur_m+=2;}} //RETZ
ops[0xD0] = [this]{cur_m=1; if((f&0x10)==0x00) {pc=Memory.rw(sp); sp+=2; cur_m+=2;}} //RETNC
ops[0xD8] = [this]{cur_m=1; if((f&0x10)==0x10) {pc=Memory.rw(sp); sp+=2; cur_m+=2;}} //RETC
ops[0xC7] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x00; cur_m=3;} //RST00
ops[0xCF] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x08; cur_m=3;} //RST08
ops[0xD7] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x10; cur_m=3;} //RST10
ops[0xDF] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x18; cur_m=3;} //RST18
ops[0xE7] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x20; cur_m=3;} //RST20
ops[0xEF] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x28; cur_m=3;} //RST28
ops[0xF7] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x30; cur_m=3;} //RST30
ops[0xFF] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x38; cur_m=3;} //RST38
//ops[0xD8] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x40; cur_m=3;} //RST40
//ops[0xD8] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x48; cur_m=3;} //RST48
//ops[0xD8] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x50; cur_m=3;} //RST50
//ops[0xD8] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x58; cur_m=3;} //RST58
//ops[0xD8] = [this]{backup(); sp-=2; Memory.ww(sp, pc); pc=0x60; cur_m=3;} //RST60
ops[0x00] = [this]{cur_m=1;} //NOP
ops[0xF3] = [this]{ime=0; cur_m=1;} //DI
ops[0xFB] = [this]{ime=1; cur_m=1;} //EI
}
