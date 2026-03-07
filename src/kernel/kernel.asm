[BITS 32]

global _start
extern kernel_main

section .text

_start:

    ; Set stack pointer to 2MB,
    ; providing 1MB of space from the kernel start
    mov esp, 0x200000      

    call kernel_main

.hang:
    hlt
    jmp .hang