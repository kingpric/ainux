#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t row;
    size_t col;
} cursor_pos_t;

// Initializes the screen module.
void screen_init();

// Clears the entire screen.
void screen_clear();

// Writes a single character at the current cursor position.
void screen_put_char(char c);

// Writes a null-terminated string.
void screen_write(const char *str);

// Removes the last character from the screen.
void screen_backspace();

// Writes a hexadecimal value to the screen.
void screen_write_hex(uint32_t value);

// Writes a decimal value to the screen.
void screen_write_dec(uint32_t value);

// Writes a newline to the screen.
void screen_newline();

// Set cursor position
void screen_set_cursor(cursor_pos_t pos);

// Get cursor position
cursor_pos_t screen_get_cursor();

#endif