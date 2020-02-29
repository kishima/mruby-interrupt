#include <mruby.h>

static uint16_t *interrupt_flag;
mrb_value callback_proc;

// for mruby int
void mrb_mruby_interrupt_set_irq(uint16_t flag){
  *interrupt_flag = 0x0001;
}

// for mruby
static void mrb_mruby_interrupt_handler(struct mrb_state *mrb){
  fprintf(stderr,"interrupt callback in mgem\n");
  if(!mrb_nil_p(callback_proc)){
    fprintf(stderr,"call proc\n");
    mrb_funcall(mrb,callback_proc,"call",0);
  }
}

static mrb_value mrb_mruby_interrupt_register(struct mrb_state *mrb, mrb_value self)
{
  mrb_value proc;
  mrb_get_args(mrb,"o",&proc);
  fprintf(stderr,"register callback tt:%d\n",proc.tt);
  callback_proc = proc;
  return self;
}


void
mrb_mruby_interrupt_gem_init(mrb_state *mrb)
{
  mrb->interrupt_func = mrb_mruby_interrupt_handler;
  interrupt_flag = &(mrb->interrupt_flag);
  *interrupt_flag = 0x0000;
  SET_NIL_VALUE(callback_proc);

  struct RClass *interrupt_module = mrb_define_module(mrb,"Interrupt");
  mrb_define_module_function(mrb, interrupt_module, "register", mrb_mruby_interrupt_register, MRB_ARGS_BLOCK());
  //Semaphore
}

void
mrb_mruby_interrupt_gem_final(mrb_state *mrb)
{

}
