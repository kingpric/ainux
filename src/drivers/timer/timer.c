#include "cpu/interrupt/interrupt.h"
#include "timer.h"

static uint32_t ticks = 0;


void timer_handler(interrupt_frame_t *frame)
{
    if (ticks % 100 == 0)
        ticks++;
}


void timer_init(){
    register_interrupt_handler(TIMER_INTERRUPT_NO, timer_handler);
}