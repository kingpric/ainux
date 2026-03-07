#include <stdint.h>
#include "idt.h"

#define IDT_SIZE 256

idt_entry_t idt[IDT_SIZE];
idtr_t idtr;

extern void idt_load(idtr_t* idtr);

void idt_set_gate(int n, uint32_t handler)
{
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = 0x08;      // kernel code segment
    idt[n].zero        = 0;
    idt[n].type_attr   = 0x8E;      // interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init()
{
    idtr.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idtr.base  = (uint32_t)&idt;

    idt_load(&idtr);
}