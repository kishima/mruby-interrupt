#include <mruby.h>
#include "mrb_interrupt.h"

static uint16_t *_interrupt_flag;
static mrb_value callback_proc[MRB_INTERRUPT_MAX_CB];

// for user code
void mrb_mruby_interrupt_set_irq(int index){
  (*_interrupt_flag) |= (1 << index);
  fprintf(stderr,"set bit:_interrupt_flag %04X\n",*_interrupt_flag);
}

// for mruby core
static void mrb_mruby_interrupt_handler(struct mrb_state *mrb,int index){
  if(index<0 || index>=MRB_INTERRUPT_MAX_CB){
    mrb_raise(mrb,E_ARGUMENT_ERROR,"invalid irq slot number");
  }
  fprintf(stderr,"interrupt callback in mgem\n");
  if(!mrb_nil_p(callback_proc[index])){
    fprintf(stderr,"call proc(%d)\n",index);
    mrb_funcall(mrb,callback_proc[index],"call",0);
  }
}

void mrb_set_interrupt_block(struct mrb_state *mrb,int slot,mrb_value blk)
{
  if(slot<0 || slot>=MRB_INTERRUPT_MAX_CB){
    mrb_raise(mrb,E_ARGUMENT_ERROR,"invalid irq slot number");
  }
  callback_proc[slot] = blk;
}

static mrb_value mrb_mruby_interrupt_register(struct mrb_state *mrb, mrb_value self)
{
  mrb_int slot;
  mrb_value proc;
  mrb_get_args(mrb,"i&",&slot,&proc);
  fprintf(stderr,"register callback[%d] tt:%d\n",slot,proc.tt);
  mrb_set_interrupt_block(mrb,slot,proc);
  return self;
}

static mrb_value mrb_mruby_interrupt_deregister(struct mrb_state *mrb, mrb_value self)
{
  mrb_int slot;
  mrb_get_args(mrb,"i",&slot);
  fprintf(stderr,"deregister callback[%d]\n",slot);
  mrb_set_interrupt_block(mrb,slot,mrb_nil_value());
  return self;
}


void
mrb_mruby_interrupt_gem_init(mrb_state *mrb)
{
  for(int i=0;i<MRB_INTERRUPT_MAX_CB;i++)
  {
    callback_proc[i] = mrb_nil_value();
  }
  
  mrb_mruby_interrupt_user_mutex_init(mrb);

  mrb->interrupt_func = mrb_mruby_interrupt_handler;
  mrb->user_mutex_func = mrb_mruby_interrupt_user_mutex;
  _interrupt_flag = &mrb->interrupt_flag;
  *_interrupt_flag = MRB_INTERRUPT_EVT_CLEAR;

  struct RClass *interrupt_module = mrb_define_module(mrb,"Interrupt");
  mrb_define_module_function(mrb, interrupt_module, "register", mrb_mruby_interrupt_register, MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, interrupt_module, "deregister", mrb_mruby_interrupt_deregister, MRB_ARGS_REQ(1));
}

void
mrb_mruby_interrupt_gem_final(mrb_state *mrb)
{
  mrb_mruby_interrupt_user_mutex_final(mrb);
}
