#include <mruby.h>
#include "mrb_interrupt.h"

void user_interrupt_handler(){

  set_interrupt_flag(1);

}