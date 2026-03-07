[BITS 32]

section .asm

global idt_load

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret