print_string:
    ;push bp             ; Save old base pointer
    ;mov bp, sp          ; Set up stack frame
    ;mov si, [bp + 4]    ; Skip saved BP (2 bytes) and Return Addr (2 bytes)
    
    mov ah, 0x0e
    
.loop:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ;pop bp              ; Restore BP
    ret                ; Return and clean up 2 bytes (the pushed address)