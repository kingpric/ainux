#include "timer.h"
#include "cpu/interrupt/interrupt.h"
#include "rtc.h"
#include "time.h"

static uint32_t tick = 0;

void timer_handler(interrupt_frame_t *frame)
{
    tick++;

    time_tick();

    if (tick % 10 == 0) {
        // time_render();
        rtc_print();
    }
}

void timer_init()
{
    register_interrupt_handler(TIMER_INTERRUPT_NO, timer_handler);
}
