#ifndef PREP
#include <ir/ir.h>
#include <target/util.h>
#endif
#include "defs.h"
#include "codegen.h"
#include "algo.h"

static int chn_cur_pc=0;
static void init_state_chn(Data* data) {
  chn_init();
  for (int mp = 0; data; data = data->next, mp++) {
    if (data->v) {
      //printf("%d\n",data->v);
      chn_emit_var_length_num(data->v);
      chn_emit_var_length_num(mp);
      chn_write_memory();
    }
  }
}

static void chn_emit_func_prologue(int func_id) {
}

static void chn_emit_func_epilogue(void) {
}

static void chn_emit_pc_change(int pc) {
  chn_cur_pc=pc;
  chn_emit_num_jmp(chn_len_code);
}
static void chn_load_val(Value*val){
  if(val->type==IMM)
  {
    chn_emit_var_length_num(val->imm);
  }else{
    chn_emit_nilad(CHN_NUM((int)val->reg+6));
    chn_emit_monad(CHN_LOAD,0);
  }
}
static void chn_store_reg(Value*val){
  chn_emit_nilad(CHN_NUM((int)val->reg+6));
  chn_emit_nilad(CHN_STORE);
}
static void chn_emit_inst(Inst* inst) {
  //printf("%d!",inst->op);
  switch (inst->op) {
  case MOV:
    chn_load_val(&inst->src);
    chn_store_reg(&inst->dst);
    break;

  case ADD:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_emit_nilad(CHN_ADD);
    chn_modulo_once();
    chn_store_reg(&inst->dst);
    break;

  case SUB:
    chn_load_val(&inst->dst);
    chn_emit_nilad(CHN_NUM(4));
    chn_emit_monad(CHN_LOAD,0);
    chn_load_val(&inst->src);
    chn_emit_nilad(CHN_SUB);
    chn_emit_nilad(CHN_ADD);
    chn_modulo_once();
    chn_store_reg(&inst->dst);
    break;

  case LOAD:
    chn_load_val(&inst->src);
    chn_read_memory();
    chn_store_reg(&inst->dst);
    break;

  case STORE:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_write_memory();
    break;

  case PUTC:
    chn_load_val(&inst->src);
    chn_putc();
    break;

  case GETC:
    chn_getc();
    chn_store_reg(&inst->dst);
    break;

  case EXIT:
    chn_output();
    chn_exit();
    break;

  case DUMP:
    break;

  case EQ:
   chn_load_val(&inst->dst);
   chn_load_val(&inst->src);
   chn_eq();
   chn_emit_nilad(CHN_NUM(0));
   chn_emit_nilad(CHN_ADD);
   chn_store_reg(&inst->dst);
   break;
  case NE:
   chn_load_val(&inst->dst);
   chn_load_val(&inst->src);
   chn_ne();
   chn_store_reg(&inst->dst);
   break;
  case LT:
   chn_load_val(&inst->dst);
   chn_load_val(&inst->src);
   chn_lt();
   chn_store_reg(&inst->dst);
   break;
  case GT:
   chn_load_val(&inst->dst);
   chn_load_val(&inst->src);
   chn_gt();
   chn_store_reg(&inst->dst);
   break;
  case LE:
   chn_load_val(&inst->dst);
   chn_load_val(&inst->src);
   chn_le();
   chn_store_reg(&inst->dst);
   break;
  case GE:
   chn_load_val(&inst->dst);
   chn_load_val(&inst->src);
   chn_ge();
   chn_store_reg(&inst->dst);
   break;

  case JEQ:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_eq();
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;
  case JNE:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_ne();
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;
  case JLT:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_lt();
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;
  case JGT:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_gt();
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;
  case JLE:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_le();
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;
  case JGE:
    chn_load_val(&inst->dst);
    chn_load_val(&inst->src);
    chn_ge();
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;
  case JMP:
    chn_emit_nilad(CHN_NUM(1));
    chn_load_val(&inst->jmp);
    chn_jump(chn_cur_pc);
    break;

  default:
    error("oops");
  }
}

void target_chn(Module* module) {
  init_state_chn(module->data);

  int num_funcs = emit_chunked_main_loop(module->text,
                                         chn_emit_func_prologue,
                                         chn_emit_func_epilogue,
                                         chn_emit_pc_change,
                                         chn_emit_inst);

  chn_output();
  chn_exit();
  chn_emit_jump_table_initializer();
  chn_emit_whole_program();
}
