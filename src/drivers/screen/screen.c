#include <io.h>
#include <screen.h>
#include <stdint.h>

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static volatile uint16_t *const vga = (uint16_t *)VGA_MEMORY;

/* Internal cursor state */
static size_t cursor_row = 0;
static size_t cursor_col = 0;

/* Default color: white on black */
static uint8_t color = 0x0F;

/**
 * @brief Scrolls the screen up by one row.
 */
static void screen_scroll()
{
    // Move rows up
    for (size_t r = 1; r < VGA_HEIGHT; r++) {
        for (size_t c = 0; c < VGA_WIDTH; c++) {
            vga[(r - 1) * VGA_WIDTH + c] = vga[r * VGA_WIDTH + c];
        }
    }

    // Clear last row
    for (size_t c = 0; c < VGA_WIDTH; c++) {
        vga[(VGA_HEIGHT - 1) * VGA_WIDTH + c] = ((uint16_t)color << 8) | ' ';
    }

    cursor_row = VGA_HEIGHT - 1;
}

void screen_set_cursor(cursor_pos_t cur_pos)
{
    uint16_t pos = cur_pos.row * VGA_WIDTH + cur_pos.col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

    cursor_row = cur_pos.row;
    cursor_col = cur_pos.col;
}

cursor_pos_t screen_get_cursor()
{
    cursor_pos_t pos;
    pos.row = cursor_row;
    pos.col = cursor_col;
    return pos;
}

static void screen_update_cursor(void)
{
    cursor_pos_t pos = {cursor_row, cursor_col};
    screen_set_cursor(pos);
}

// Clears entire VGA text buffer.
void screen_clear()
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = ((uint16_t)color << 8) | ' ';
    }

    cursor_row = 0;
    cursor_col = 0;
}

// Initializes screen module.
void screen_init()
{
    screen_clear();
}

// Writes single character at current cursor position.
void screen_put_char(char c)
{

    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;

        if (cursor_row >= VGA_HEIGHT) {
            screen_scroll();
        }

        screen_update_cursor();
        return;
    }

    size_t index = cursor_row * VGA_WIDTH + cursor_col;

    uint16_t entry = ((uint16_t)color << 8) | (uint8_t)c;
    vga[index] = entry;

    cursor_col++;

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;

        if (cursor_row >= VGA_HEIGHT) {
            screen_scroll();
        }
    }

    screen_update_cursor();
}

// Writes a null-terminated string.
void screen_write(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++) {
        screen_put_char(str[i]);
    }
}

void screen_backspace()
{
    if (cursor_col > 0) {
        cursor_col--;
        vga[cursor_row * VGA_WIDTH + cursor_col] = ' ';
        screen_update_cursor();
    }
}

void screen_write_hex(uint32_t value)
{
    char hex[] = "0123456789ABCDEF";

    for (int i = 28; i >= 0; i -= 4) {
        uint8_t digit = (value >> i) & 0xF;
        screen_put_char(hex[digit]);
    }
}

void screen_write_dec(uint32_t value)
{
    char buffer[16];
    int i = 0;

    if (value == 0) {
        screen_put_char('0');
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--) {
        screen_put_char(buffer[i]);
    }
}

void screen_newline()
{
    screen_put_char('\n');
}
