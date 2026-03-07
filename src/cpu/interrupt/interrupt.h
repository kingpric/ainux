#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

/**
 * CPU interrupt stack frame
 */
typedef struct interrupt_frame
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t interrupt_no;
    uint32_t error_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;

} interrupt_frame_t;

typedef void (*interrupt_handler_t)(interrupt_frame_t *frame);

void init_interrupts();
void register_interrupt_handler(uint8_t interrupt_no, interrupt_handler_t handler);
void interrupt_handler(interrupt_frame_t *frame);

#endif