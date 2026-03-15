[BITS 32]

global load_page_directory
global enable_paging


;---------------------------------------
; load_page_directory(uint32_t *pd)
;---------------------------------------

load_page_directory:

    mov eax, [esp+4]
    mov cr3, eax

    ret


;---------------------------------------
; enable_paging()
;---------------------------------------

enable_paging:

    mov eax, cr0
    or eax, 0x80000000      ; Set PG bit (bit 31)
    mov cr0, eax

    ret