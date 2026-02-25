BITS 16
ORG 0x8000

start_stage2:
    mov si, msg

print_loop:
    lodsb
    cmp al, 0
    je done

    mov ah, 0x0E
    int 0x10
    jmp print_loop

done:
    jmp $

msg db "Ainux Stage 2 Loaded!", 0 