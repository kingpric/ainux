; boot.asm
; Our first boot sector

BITS 16
ORG 0X7C00

%define OS_NAME "Ainux"

start:
    cli             ; clear interrupt flag

    xor ax,ax       ; setting 0H (0x0000) to AX
    mov ds, ax      ; copying AX to DS (data segmrnt)
    mov es, ax      ; copying AX to ES (extra segment)
    mov ss, ax      ; copying AX to SS (stack segment)
    mov sp, 0x7C00  ; copying 0x7C00 value to SP (stack pointer)

    sti             ; set interrupt flag

    mov si, welcome_msg ; set SI (Source index) to start location of message

print_loop:
    lodsb           ; load string byte to al from [si] and increament si = si + 1 
    cmp al, 0       ; compare al to 0 (null)
    je done         ; if compare is equal then jmp to done label

    mov ah, 0eh     ; mov value 0xoe to ah
    int 10h         ; call interrupt
    jmp print_loop  ; unconditional jmp to print_loop label

done:               
    jmp $           ; unconditional jmp to here only

welcome_msg db "Welcome to ", OS_NAME, " - Stage 1", 0

times 510-($-$$) db 0   ; set remaining bytes to 0 upto 510 address
dw 0xaa55           ; boot magic number