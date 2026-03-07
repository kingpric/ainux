#include <stdint.h>
#include "idt.h"
#include "config.h"

idt_entry_t idt[INTERRUPT_SIZE];
idtr_t idtr;

extern void idt_load(idtr_t* idtr);

/*
 * Set IDT gate
 */
void idt_set_gate(int interrupt_no, uint32_t handler_address)
{
    idt[interrupt_no].offset_low  = handler_address & 0xFFFF;
    idt[interrupt_no].selector    = KERNEL_CODE_SELECTOR;       // kernel code segment
    idt[interrupt_no].zero        = 0;
    idt[interrupt_no].type_attr   = 0x8E;                       // interrupt gate
    idt[interrupt_no].offset_high = (handler_address >> 16) & 0xFFFF;
}

/*
 * Initialize IDT
 */
void idt_init()
{
    idtr.limit = sizeof(idt_entry_t) * INTERRUPT_SIZE - 1;
    idtr.base  = (uint32_t)&idt;

    idt_load(&idtr);
}