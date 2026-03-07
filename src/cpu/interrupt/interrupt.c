#include <stdint.h>
#include <screen.h>
#include <io.h>
#include <config.h>

#include "interrupt.h"
#include "memory/memory.h"

interrupt_handler_t interrupt_handlers[INTERRUPT_SIZE];

void init_interrupts()
{
    memset(interrupt_handlers, 0, sizeof(interrupt_handlers));
}   

void register_interrupt_handler(uint8_t interrupt_no, interrupt_handler_t handler)
{
    interrupt_handlers[interrupt_no] = handler;
}

void interrupt_handler(interrupt_frame_t *frame)
{
    if (interrupt_handlers[frame->interrupt_no])
    {
        interrupt_handlers[frame->interrupt_no](frame);
    }

    /* Send EOI for hardware IRQs */
    if (frame->interrupt_no >= 32 && frame->interrupt_no <= 47)
    {
        if (frame->interrupt_no >= 40)
        {
            outb(0xA0, 0x20);
        }

        outb(0x20, 0x20);
    }
}