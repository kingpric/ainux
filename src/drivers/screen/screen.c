#include "screen.h"
#include <stdint.h>


#define VGA_MEMORY 0xB8000
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

static volatile uint16_t* const vga = (uint16_t*) VGA_MEMORY;

/* Internal cursor state */
static size_t row = 0;
static size_t col = 0;

/* Default color: white on black */
static uint8_t color = 0x0F;

/**
 * @brief Clears entire VGA text buffer.
 */
void screen_clear()
{
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        vga[i] = ((uint16_t)color << 8) | ' ';
    }

    row = 0;
    col = 0;
}

/**
 * @brief Initializes screen module.
 */
void screen_init()
{
    screen_clear();
}

/**
 * @brief Writes single character at current cursor position.
 */
void screen_put_char(char c)
{
    if (c == '\n')
    {
        col = 0;
        row++;

        if (row >= VGA_HEIGHT)
        {
            row = 0;   // temporary wrap
        }

        return;
    }

    size_t index = row * VGA_WIDTH + col;

    uint16_t entry = ((uint16_t)color << 8) | (uint8_t)c;
    vga[index] = entry;

    col++;

    if (col >= VGA_WIDTH)
    {
        col = 0;
        row++;

        if (row >= VGA_HEIGHT)
        {
            row = 0;
        }
    }
}

/**
 * @brief Writes a null-terminated string.
 */
void screen_write(const char* str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        screen_put_char(str[i]);
    }
}