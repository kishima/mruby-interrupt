#include <mruby.h>
#include "mrb_interrupt.h"
//user header
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
static SemaphoreHandle_t xSemaphore;

void mrb_mruby_interrupt_user_mutex_init(mrb_state *mrb)
{
  xSemaphore = xSemaphoreCreateMutex();
  if( xSemaphore == NULL )
  {
    mrb_raise(mrb,E_RUNTIME_ERROR,"xSemaphoreCreateMutex error");
  }
}

void mrb_mruby_interrupt_user_mutex_final(mrb_state *mrb)
{
  if(xSemaphore!=NULL){
    vSemaphoreDelete(xSemaphore);
  }
}

void mrb_mruby_interrupt_user_mutex(mrb_state *mrb,int state)
{
  /* implement mutex function */
  if(xSemaphore){
    if(state){
      xSemaphoreTake(xSemaphore,0);//lock
    }else{
      xSemaphoreGive(xSemaphore);//unlock
    }
  }
}


void user_interrupt_handler(int i){
  mrb_mruby_interrupt_user_mutex(NULL,1);//lock

  mrb_mruby_interrupt_set_irq(i);

  mrb_mruby_interrupt_user_mutex(NULL,0);//unlock
}

