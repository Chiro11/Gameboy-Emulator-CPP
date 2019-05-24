using namespace std;

void init() {
    a=0, b=0, c=0, d=0, e=0, h=0, l=0, f=0;
    sp=0, pc=0;
    cur_m=0, tot_m=0;
    ime=1;
}
void LDrr_bb() {b=b; cur_m=1;}
void LDrr_bc() {b=c; cur_m=1;}
void LDrr_bd() {b=d; cur_m=1;}
void LDrr_be() {b=e; cur_m=1;}
void LDrr_bh() {b=h; cur_m=1;}
void LDrr_bl() {b=l; cur_m=1;}
void LDrr_ba() {b=a; cur_m=1;}
void LDrr_cb() {c=b; cur_m=1;}
void LDrr_cc() {c=c; cur_m=1;}
void LDrr_cd() {c=d; cur_m=1;}
void LDrr_ce() {c=e; cur_m=1;}
void LDrr_ch() {c=h; cur_m=1;}
void LDrr_cl() {c=l; cur_m=1;}
void LDrr_ca() {c=a; cur_m=1;}
void LDrr_db() {d=b; cur_m=1;}
void LDrr_dc() {d=c; cur_m=1;}
void LDrr_dd() {d=d; cur_m=1;}
void LDrr_de() {d=e; cur_m=1;}
void LDrr_dh() {d=h; cur_m=1;}
void LDrr_dl() {d=l; cur_m=1;}
void LDrr_da() {d=a; cur_m=1;}
void LDrr_eb() {e=b; cur_m=1;}
void LDrr_ec() {e=c; cur_m=1;}
void LDrr_ed() {e=d; cur_m=1;}
void LDrr_ee() {e=e; cur_m=1;}
void LDrr_eh() {e=h; cur_m=1;}
void LDrr_el() {e=l; cur_m=1;}
void LDrr_ea() {e=a; cur_m=1;}
void LDrr_hb() {h=b; cur_m=1;}
void LDrr_hc() {h=c; cur_m=1;}
void LDrr_hd() {h=d; cur_m=1;}
void LDrr_he() {h=e; cur_m=1;}
void LDrr_hh() {h=h; cur_m=1;}
void LDrr_hl() {h=l; cur_m=1;}
void LDrr_ha() {h=a; cur_m=1;}
void LDrr_lb() {l=b; cur_m=1;}
void LDrr_lc() {l=c; cur_m=1;}
void LDrr_ld() {l=d; cur_m=1;}
void LDrr_le() {l=e; cur_m=1;}
void LDrr_lh() {l=h; cur_m=1;}
void LDrr_ll() {l=l; cur_m=1;}
void LDrr_la() {l=a; cur_m=1;}
void LDrr_ab() {a=b; cur_m=1;}
void LDrr_ac() {a=c; cur_m=1;}
void LDrr_ad() {a=d; cur_m=1;}
void LDrr_ae() {a=e; cur_m=1;}
void LDrr_ah() {a=h; cur_m=1;}
void LDrr_al() {a=l; cur_m=1;}
void LDrr_aa() {a=a; cur_m=1;}
void LDrHLm_b() {b=MMU.rb((h<<8)+l}); cur_m=2;}
void LDrHLm_c() {c=MMU.rb((h<<8)+l}); cur_m=2;}
void LDrHLm_d() {d=MMU.rb((h<<8)+l}); cur_m=2;}
void LDrHLm_e() {e=MMU.rb((h<<8)+l}); cur_m=2;}
void LDrHLm_h() {h=MMU.rb((h<<8)+l}); cur_m=2;}
void LDrHLm_l() {l=MMU.rb((h<<8)+l}); cur_m=2;}
void LDrHLm_a() {b=MMU.rb((h<<8)+l}); cur_m=2;}
void LDHLmr_b() {MMU.wb((h<<8)+l, b); cur_m=2;}
void LDHLmr_c() {MMU.wb((h<<8)+l, c); cur_m=2;}
void LDHLmr_d() {MMU.wb((h<<8)+l, d); cur_m=2;}
void LDHLmr_e() {MMU.wb((h<<8)+l, e); cur_m=2;}
void LDHLmr_h() {MMU.wb((h<<8)+l, h); cur_m=2;}
void LDHLmr_l() {MMU.wb((h<<8)+l, l); cur_m=2;}
void LDHLmr_a() {MMU.wb((h<<8)+l, a); cur_m=2;}
