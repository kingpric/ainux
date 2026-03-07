#ifndef TIMER_H
#define TIMER_H

#define TIMER_INTERRUPT_NO 32

#include "cpu/interrupt/interrupt.h"

void timer_handler(interrupt_frame_t *frame);
void timer_init();

#endif