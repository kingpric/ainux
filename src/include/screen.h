#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Initializes the screen module.
 *
 * Clears the VGA text buffer and resets cursor position to (0,0).
 */
void screen_init();

/**
 * @brief Clears the entire screen.
 *
 * Fills the VGA buffer with blank characters using current color.
 * Resets internal cursor position to top-left.
 */
void screen_clear();

/**
 * @brief Writes a single character at the current cursor position.
 *
 * Automatically advances cursor to the next column.
 * Does not handle scrolling yet.
 *
 * @param c Character to write.
 */
void screen_put_char(char c);

/**
 * @brief Writes a null-terminated string.
 *
 * Sequentially prints characters using screen_put_char().
 *
 * @param str Pointer to null-terminated string.
 */
void screen_write(const char *str);

/**
 * @brief Removes the last character from the screen.
 */
void screen_backspace();

// Writes a hexadecimal value to the screen.
void screen_write_hex(uint32_t value);

// Writes a decimal value to the screen.
void screen_write_dec(uint32_t value);

// Writes a newline to the screen.
void screen_newline();

#endif