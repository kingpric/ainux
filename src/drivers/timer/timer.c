#include <screen.h>

#include "cpu/interrupt/interrupt.h"
#include "timer.h"

static uint32_t ticks = 0;

void timer_handler(interrupt_frame_t *frame)
{
    ticks++;

    if (ticks % 100 == 0)
    {
        screen_write(".");
    }
}

void timer_init(){
    register_interrupt_handler(TIMER_INTERRUPT_NO, timer_handler);
}