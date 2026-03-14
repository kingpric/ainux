print_char:
    mov ah, 0x0E
    int 0x10
    ret
;;;;;;;;;;;;;;;;;;;;

print_newline:

    mov al, 13
    call print_char

    mov al, 10
    call print_char

    ret    
;;;;;;;;;;;;;;;;;;;

print_hex32:

    push ax
    push bx
    push cx
    push dx

    mov cx, 8

.hex_loop:

    rol eax, 4

    mov dl, al
    and dl, 0x0F

    cmp dl, 9
    jbe .digit

    add dl, 'A' - 10
    jmp .print

.digit:
    add dl, '0'

.print:

    mov al, dl
    call print_char

    loop .hex_loop

    pop dx
    pop cx
    pop bx
    pop ax

    ret

;;;;;;;;;;;;;;;;;;;;;;

print_memory_map:

    mov si, msg_map
    call print_string

    mov ecx, [MEMORY_MAP_ADDR]
    mov di, MEMORY_MAP_ADDR + 4

.next:

    cmp ecx, 0
    je .done

    mov si, msg_region
    call print_string

    mov eax, [di]
    call print_hex32

    mov al, ' '
    call print_char

    mov si, msg_len
    call print_string

    mov eax, [di+8]
    call print_hex32

    mov al, ' '
    call print_char

    mov si, msg_type
    call print_string

    mov eax, [di+16]
    call print_hex32

    mov al, ' '
    call print_char

    call print_newline

    add di, 24
    dec ecx
    jmp .next

.done:
    ret

; print_memory_map:

;     mov si, msg_map
;     call print_string

;     mov ecx, [MEMORY_MAP_ADDR]
;     mov si, MEMORY_MAP_ADDR + 4

; .next:

;     cmp ecx, 0
;     je .done

;     push si

;     mov si, msg_region
;     call print_string

;     pop si
;     mov eax, [si]
;     call print_hex32
;     push si

;     mov si, msg_len
;     call print_string

;     pop si
;     mov eax, [si+8]
;     call print_hex32
;     push si

;     mov si, msg_type
;     call print_string

;     pop si
;     mov eax, [si+16]
;     call print_hex32
    

;     call print_newline

;     add si, 24
;     dec ecx
;     jmp .next

; .done:
;     ret


; print_hex32:

;     pusha

;     mov ecx, 8

; .hex_loop:
;     rol eax, 4

;     mov dl, al
;     and dl, 0x0F

;     cmp dl, 9
;     jbe .digit

;     add dl, 'A' - 10
;     jmp .print

; .digit:
;     add dl, '0'

; .print:
;     mov al, dl
;     call print_char

;     loop .hex_loop

;     popa
;     ret


; print_newline:

;     mov ah, 0x0E

;     mov al, 13
;     int 0x10

;     mov al, 10
;     int 0x10

;     ret    