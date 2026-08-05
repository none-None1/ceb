#ifndef CHN_CODEGEN
#define CHN_CODEGEN
#include "defs.h"
#define chn_num_node struct _chn_num_node
#define chn_code struct _chn_code
#ifndef LOCAL
#ifndef PREP
#include<ir/ir.h>
#include<target/util.h>
#endif
#else
#define emit_str printf
#endif
struct _chn_num_node{
	int num;
	chn_num_node*nxt;
};
struct _chn_code{ // The whole code as a list, also used for jump table
	chn_num_node*head,*tail;
};
static chn_code *chn_nodes; // The code instance
static chn_code *chn_jump_table;
int chn_len_code; // Number of commands in code+jump table
static int chn_len_nojump; // Number of commands in code
void chn_init(){ // Initialize (must be run first)
	chn_nodes=(chn_code*)malloc(sizeof(chn_code));
	chn_nodes->head=chn_nodes->tail=NULL;
	chn_jump_table=(chn_code*)malloc(sizeof(chn_code));
	chn_jump_table->head=chn_jump_table->tail=NULL;
}
void chn_emit_num(int x){ // Emit a number
        //printf("NUM (%d)\n",x);
	chn_num_node *newnode=(chn_num_node*)malloc(sizeof(chn_num_node));
	newnode->num=x;
	newnode->nxt=NULL;
	if(chn_nodes->head==NULL){
		chn_nodes->head=chn_nodes->tail=newnode;
		return;
	}
	chn_nodes->tail->nxt=newnode;
	chn_nodes->tail=newnode;
}
void chn_emit_num_jmp(int x){ // Emit a number in jump table
	chn_num_node *newnode=(chn_num_node*)malloc(sizeof(chn_num_node));
	newnode->num=x;
	newnode->nxt=NULL;
	if(chn_jump_table->head==NULL){
		chn_jump_table->head=chn_jump_table->tail=newnode;
		return;
	}
	chn_jump_table->tail->nxt=newnode;
	chn_jump_table->tail=newnode;
}
void chn_emit_nilad(int x){ // Emit a single wide instruction not in the jump table
	assert(x!=CHN_LOAD);
	chn_emit_num(x);
	++chn_len_code;
        //printf("LEN (%d)\n",chn_len_code);
	++chn_len_nojump;
}
void chn_emit_nilad_jmp(int x){ // Emit a single wide instruction that initializes the jump table
	assert(x!=CHN_LOAD);
	chn_emit_num(x);
	++chn_len_code;
	//printf("LEN (%d)\n",chn_len_code);
}
void chn_emit_monad(int x,int y){ // Emit a double wide instruction
	assert(x==CHN_LOAD);
	chn_emit_num(x);
	chn_emit_num(y);
	chn_len_code+=2;
	chn_len_nojump+=2;
	//printf("LEN (%d)\n",chn_len_code);
}
void chn_emit_monad_jmp(int x,int y){ // Emit a double wide instruction that initializes the jump table
	assert(x==CHN_LOAD);
        chn_emit_num(x);
        chn_emit_num(y);
        chn_len_code+=2;
	//printf("LEN (%d)\n",chn_len_code);
}
void chn_emit_static_length_num(int x){ // Emit a 24b number with exactly 25 instructions directly to stdout
	emit_str("%d ",CHN_NUM(0));
	emit_str("%d %d %d %d ",CHN_NUM(16),CHN_MUL,CHN_NUM((x>>20)&15),CHN_ADD);
	emit_str("%d %d %d %d ",CHN_NUM(16),CHN_MUL,CHN_NUM((x>>16)&15),CHN_ADD);
	emit_str("%d %d %d %d ",CHN_NUM(16),CHN_MUL,CHN_NUM((x>>12)&15),CHN_ADD);
	emit_str("%d %d %d %d ",CHN_NUM(16),CHN_MUL,CHN_NUM((x>>8)&15),CHN_ADD);
	emit_str("%d %d %d %d ",CHN_NUM(16),CHN_MUL,CHN_NUM((x>>4)&15),CHN_ADD);
	emit_str("%d %d %d %d ",CHN_NUM(16),CHN_MUL,CHN_NUM(x&15),CHN_ADD);
}
void chn_emit_header(){ // Emit the header
	chn_emit_static_length_num(chn_len_code);
	emit_str("%d %d ",CHN_NUM(2),CHN_STORE);
	chn_emit_static_length_num(chn_len_nojump);
	emit_str("%d %d ",CHN_NUM(3),CHN_STORE);
	emit_str("%d ",CHN_NUM(1));
	for(int i=0;i<6;i++)emit_str("%d %d ",CHN_NUM(16),CHN_MUL);
	emit_str("%d %d ",CHN_NUM(4),CHN_STORE);
	chn_emit_static_length_num(140); // header+stack+input is 140c long
	emit_str("%d %d ",CHN_NUM(5),CHN_STORE);
	for(int i=6;i<14;i++){ // initialize the registers
		emit_str("%d %d %d ",CHN_NUM(0),CHN_NUM(i),CHN_STORE);
	}
	emit_str("%d %d %d %d ",CHN_NUM(10),CHN_CHAR,CHN_NUM(CHN_STDOUT),CHN_STORE); // output always starts with a line feed
	emit_str("%d ",CHN_NUM(1));
	for(int i=0;i<3;i++)emit_str("%d %d ",CHN_NUM(16),CHN_MUL);
	emit_str("%d %d ",CHN_NUM(CHN_MEMOFF),CHN_STORE); // store memory offset (4096)
	emit_str("%d %d %d 0 %d ",CHN_NUM(1),CHN_NUM(3),CHN_LOAD,CHN_JMP);
}
void chn_emit_var_length_num(int x){ // Emit a 24b number with the minimum number of instructions to code instance
	//printf("%d %d %d %d?\n",x,(x>>4)&15,x&15,chn_len_code);
	chn_emit_nilad(CHN_NUM(0));
	if(x>=0x100000)chn_emit_nilad(CHN_NUM(16)),chn_emit_nilad(CHN_MUL),chn_emit_nilad(CHN_NUM((x>>20)&15)),chn_emit_nilad(CHN_ADD);
	if(x>=0x10000)chn_emit_nilad(CHN_NUM(16)),chn_emit_nilad(CHN_MUL),chn_emit_nilad(CHN_NUM((x>>16)&15)),chn_emit_nilad(CHN_ADD);
	if(x>=0x1000)chn_emit_nilad(CHN_NUM(16)),chn_emit_nilad(CHN_MUL),chn_emit_nilad(CHN_NUM((x>>12)&15)),chn_emit_nilad(CHN_ADD);
	if(x>=0x100)chn_emit_nilad(CHN_NUM(16)),chn_emit_nilad(CHN_MUL),chn_emit_nilad(CHN_NUM((x>>8)&15)),chn_emit_nilad(CHN_ADD);
	if(x>=0x10)chn_emit_nilad(CHN_NUM(16)),chn_emit_nilad(CHN_MUL),chn_emit_nilad(CHN_NUM((x>>4)&15)),chn_emit_nilad(CHN_ADD);
	if(x>=0x1)chn_emit_nilad(CHN_NUM(16)),chn_emit_nilad(CHN_MUL),chn_emit_nilad(CHN_NUM(x&15)),chn_emit_nilad(CHN_ADD);
}
void chn_emit_var_length_num_jmp(int x){ // Emit a 24b number with the minimum number of instructions to code instance
	chn_emit_nilad_jmp(CHN_NUM(0));
	if(x>=0x100000)chn_emit_nilad_jmp(CHN_NUM(16)),chn_emit_nilad_jmp(CHN_MUL),chn_emit_nilad_jmp(CHN_NUM((x>>20)&15)),chn_emit_nilad_jmp(CHN_ADD);
	if(x>=0x10000)chn_emit_nilad_jmp(CHN_NUM(16)),chn_emit_nilad_jmp(CHN_MUL),chn_emit_nilad_jmp(CHN_NUM((x>>16)&15)),chn_emit_nilad_jmp(CHN_ADD);
	if(x>=0x1000)chn_emit_nilad_jmp(CHN_NUM(16)),chn_emit_nilad_jmp(CHN_MUL),chn_emit_nilad_jmp(CHN_NUM((x>>12)&15)),chn_emit_nilad_jmp(CHN_ADD);
	if(x>=0x100)chn_emit_nilad_jmp(CHN_NUM(16)),chn_emit_nilad_jmp(CHN_MUL),chn_emit_nilad_jmp(CHN_NUM((x>>8)&15)),chn_emit_nilad_jmp(CHN_ADD);
	if(x>=0x10)chn_emit_nilad_jmp(CHN_NUM(16)),chn_emit_nilad_jmp(CHN_MUL),chn_emit_nilad_jmp(CHN_NUM((x>>4)&15)),chn_emit_nilad_jmp(CHN_ADD);
	if(x>=0x1)chn_emit_nilad_jmp(CHN_NUM(16)),chn_emit_nilad_jmp(CHN_MUL),chn_emit_nilad_jmp(CHN_NUM(x&15)),chn_emit_nilad_jmp(CHN_ADD);
}
void chn_emit_jump_table_initializer(){ // Emit code that initializes the jump table
	chn_emit_nilad_jmp(CHN_NUM(3));
	chn_emit_monad_jmp(CHN_LOAD,0);
	chn_emit_nilad_jmp(CHN_NUM(5));
	chn_emit_monad_jmp(CHN_LOAD,0);
	chn_emit_nilad_jmp(CHN_ADD);
	chn_emit_nilad_jmp(CHN_NUM(6));
	chn_emit_nilad_jmp(CHN_STORE); // Temporarily use the register A for the location to store
	for(chn_num_node*i=chn_jump_table->head;i!=NULL;i=i->nxt){
		chn_emit_var_length_num_jmp(i->num);
		chn_emit_nilad_jmp(CHN_NUM(6));
		chn_emit_monad_jmp(CHN_LOAD,0);
		chn_emit_nilad_jmp(CHN_STORE);
		chn_emit_nilad_jmp(CHN_NUM(6));
		chn_emit_monad_jmp(CHN_LOAD,0);
		chn_emit_nilad_jmp(CHN_NUM(1));
		chn_emit_nilad_jmp(CHN_ADD);
		chn_emit_nilad_jmp(CHN_NUM(6));
		chn_emit_nilad_jmp(CHN_STORE);
	}
	chn_emit_nilad_jmp(CHN_NUM(0)); // Restore the register A
	chn_emit_nilad_jmp(CHN_NUM(6));
	chn_emit_nilad_jmp(CHN_STORE);
	chn_emit_nilad_jmp(CHN_NUM(1)); // Jump back to text
	chn_emit_nilad_jmp(CHN_NUM(0));
	chn_emit_nilad_jmp(CHN_NUM(2));
	chn_emit_monad_jmp(CHN_LOAD,0);
	chn_emit_nilad_jmp(CHN_SUB);
	chn_emit_nilad_jmp(CHN_JMP);
}
void chn_emit_whole_program(){
	chn_emit_header();
	for(chn_num_node*i=chn_nodes->head;i!=NULL;i=i->nxt) emit_str("%d ",i->num);
}
#endif

