[BITS 32]
ORG 0x100000

kernel_entry:

    mov edi, 0xB8000
    mov esi, kernel_message

print_kernel:
    lodsb
    cmp al, 0
    je done_kernel
    mov ah, 0x0A
    stosw
    jmp print_kernel

done_kernel:
    jmp $

kernel_message db "Ainux Kernel @ 1MB", 0