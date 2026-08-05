#ifndef CHN_ALGO
#define CHN_ALGO
#include "defs.h"
#include "codegen.h"
void chn_exit(){ // Exit (must be run at the end of text)
	chn_emit_nilad(CHN_EXIT);
}
void chn_non_negative(){ // Checks whether the top is non-negative, NaN for negative and otherwise number (10c)
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_CHAR);
	chn_emit_nilad(CHN_ADD); // JS trick: number+string=string
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_monad(CHN_LOAD,CHN_RT); // '-' if negative, else digit
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_SUB); // JS trick: minus tries to convert string to number
}
void chn_neg(){ // Negative (4c)
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_NUM(1));
	chn_emit_nilad(CHN_SUB);
	chn_emit_nilad(CHN_MUL);
}
void chn_not(){ // Invert integer 0 <-> 1 (6c)
	chn_neg();
	chn_emit_nilad(CHN_NUM(1));
	chn_emit_nilad(CHN_ADD);
}
void chn_isntnan(){ // F if top is NaN otherwise T (9c)
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_CMP);
}
void chn_isnan(){ // T if top is NaN otherwise F (15c)
	chn_isntnan();
	chn_not();
}
void chn_eq(){ // == EQ (1c)
	chn_emit_nilad(CHN_CMP);
}
void chn_ne(){ // != NE (7c)
	chn_eq();
	chn_not();
}
void chn_lt(){ // < LT (26c)
	chn_emit_nilad(CHN_SUB);
	chn_non_negative();
	chn_isnan();
}
void chn_ge(){ // >= GE (20c)
	chn_emit_nilad(CHN_SUB);
	chn_non_negative();
	chn_isntnan();
}
void chn_gt(){ // > GT (30c)
	chn_emit_nilad(CHN_SUB);
	chn_neg();
	chn_non_negative();
	chn_isnan();
}
void chn_le(){ // <= LE (24c)
	chn_emit_nilad(CHN_SUB);
	chn_neg();
	chn_non_negative();
	chn_isntnan();
}
void chn_getc(){ // getchar, reads exactly three octals instead for Chicken doesn't have an alphabet
	chn_emit_nilad(CHN_NUM(CHN_RI));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_monad(CHN_LOAD,1);
	chn_emit_nilad(CHN_NUM(8));
	chn_emit_nilad(CHN_MUL);
	chn_emit_nilad(CHN_NUM(8));
	chn_emit_nilad(CHN_MUL);
	chn_emit_nilad(CHN_NUM(CHN_RI));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(1));
	chn_emit_nilad(CHN_ADD);
	chn_emit_monad(CHN_LOAD,1);
	chn_emit_nilad(CHN_NUM(8));
	chn_emit_nilad(CHN_MUL);
	chn_emit_nilad(CHN_NUM(CHN_RI));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(2));
	chn_emit_nilad(CHN_ADD);
	chn_emit_monad(CHN_LOAD,1);
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_SUB);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(CHN_RI));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(3));
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(CHN_RI));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_monad(CHN_LOAD,0);
	chn_isnan(); // EOF should be 0 instead of NaN
	chn_emit_nilad(CHN_NUM(4));
	chn_emit_nilad(CHN_JMP);
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
}
void chn_put(){
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_STDOUT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(CHN_STDOUT));
	chn_emit_nilad(CHN_STORE);
}
void chn_putc(){
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_STDOUT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RT));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_CHAR);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(CHN_STDOUT));
	chn_emit_nilad(CHN_STORE);
}
void chn_output(){
	chn_emit_nilad(CHN_NUM(CHN_STDOUT));
	chn_emit_monad(CHN_LOAD,0);
}
void chn_modulo_once(){
        //puts("!");
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(4));
	chn_emit_monad(CHN_LOAD,0);
	chn_lt();
	chn_emit_nilad(CHN_NUM(9));
	chn_emit_nilad(CHN_JMP);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(4));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_SUB);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
}
void chn_read_memory(){
	chn_emit_nilad(CHN_NUM(CHN_MEMOFF));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_modulo_once();
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RM));
	chn_emit_monad(CHN_LOAD,0);
	chn_gt();
	chn_emit_nilad(CHN_NUM(14));
	chn_emit_nilad(CHN_JMP);
	chn_emit_nilad(CHN_NUM(2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(5));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
}
void chn_write_memory(){ // accepts val,addr as argument
	chn_emit_nilad(CHN_NUM(CHN_MEMOFF));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_modulo_once();
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_RT3));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RM));
	chn_emit_monad(CHN_LOAD,0);
	chn_le();
	// jump (skip)
	chn_emit_nilad(CHN_NUM(25));
	chn_emit_nilad(CHN_JMP);
	chn_emit_nilad(CHN_NUM(0)); // back to here
	chn_emit_nilad(CHN_NUM(CHN_RM));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(1));
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(CHN_RM));
	chn_emit_nilad(CHN_STORE);
	chn_emit_nilad(CHN_NUM(CHN_RM));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_ne();
	// jump back
	chn_emit_nilad(CHN_NUM(0));
	chn_emit_nilad(CHN_NUM(25));
	chn_emit_nilad(CHN_SUB);
	chn_emit_nilad(CHN_JMP);
	// jump end
	chn_emit_nilad(CHN_NUM(CHN_RT3));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(CHN_RT2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(2));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_NUM(5));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_STORE);
}
void chn_jump(int src_pc){ // Conditional jump, pops condition,dst_pc
	chn_emit_nilad(CHN_NUM(3));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(5));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_ADD);
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_var_length_num(src_pc+1);
	chn_emit_nilad(CHN_NUM(3));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_NUM(5));
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_ADD);
	chn_emit_nilad(CHN_ADD);
	chn_emit_monad(CHN_LOAD,0);
	chn_emit_nilad(CHN_SUB);
	chn_emit_nilad(CHN_JMP);
}
#endif
