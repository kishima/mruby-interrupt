#ifndef MRB_INTERRUPT_H__
#define MRB_INTERRUPT_H__

#include <stdint.h>

#define MRB_INTERRUPT_MAX_CB (15)
#define MRB_INTERRUPT_EVT_CLEAR (0x0000)

void mrb_mruby_interrupt_set_irq(int index);

void mrb_mruby_interrupt_user_mutex_init(mrb_state *mrb);
void mrb_mruby_interrupt_user_mutex_final(mrb_state *mrb);
void mrb_mruby_interrupt_user_mutex(mrb_state *mrb,int state);

#endif


