#ifndef CHN_DEFS
#define CHN_DEFS
const int CHN_EXIT=0;
const int CHN_ADD=2;
const int CHN_SUB=3;
const int CHN_MUL=4;
const int CHN_CMP=5;
const int CHN_LOAD=6;
const int CHN_STORE=7;
const int CHN_JMP=8;
const int CHN_CHAR=9;
#define CHN_NUM(x) ((x)+10)
const int CHN_RI=12; // I register
const int CHN_RM=13; // M register
const int CHN_RT=14; // T register
const int CHN_STDOUT=15; // stdout
const int CHN_MEMOFF=16; // Memory offset (=4096)
const int CHN_RT2=17; // T2 register
const int CHN_RT3=18; // T3 register
//const int CHN_RT4=19; // T4 register
#endif
