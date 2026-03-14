#include <stdint.h>
#include <io.h>
#include <screen.h>

#include "cpu/interrupt/interrupt.h"
#include "keyborad.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define INTERRUP_KEYBOARD_NO 33

static int shift_pressed = 0;

static char normal_map[128] =
{
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', 
    '\t', 'q','w','e','r','t','y','u','i','o','p','[',']','\n', 
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
};
static char shift_map[128] =
{
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t', 'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0, 'A','S','D','F','G','H','J','K','L',':','"','~',
    0, '|','Z','X','C','V','B','N','M','<','>','?',
    0, '*', 
    0, ' ',
};

void keyboard_handler(struct interrupt_frame *frame)
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    /* key release */
    if (scancode & 0x80)
    {
        if (scancode == 0xAA || scancode == 0xB6)
            shift_pressed = 0;

        return;
    }

    /* shift pressed */
    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        return;
    }

    if (scancode >= 128)
        return;

    if (scancode == 0x0E)
    {
        screen_backspace();
        return;
    }

    char c;

    if (shift_pressed)
        c = shift_map[scancode];
    else
        c = normal_map[scancode];

    if (c)
        screen_put_char(c);
}

void keyboard_init() {
    register_interrupt_handler(INTERRUP_KEYBOARD_NO, keyboard_handler);
}

