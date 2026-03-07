#ifndef AINUX_ARCH_X86_IO_H
#define AINUX_ARCH_X86_IO_H

#include <stdint.h>

/**
 * @brief Write a byte to an I/O port
 */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

/**
 * @brief Write a word (16-bit) to an I/O port
 */
static inline void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile (
        "outw %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

/**
 * @brief Read a byte from an I/O port
 */
static inline uint8_t inb(uint16_t port)
{
    uint8_t value;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

/**
 * @brief Read a word (16-bit) from an I/O port
 */
static inline uint16_t inw(uint16_t port)
{
    uint16_t value;

    __asm__ volatile (
        "inw %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

/**
 * @brief Small I/O delay
 *
 * Used when programming some hardware (like PIC).
 */
static inline void io_wait(void)
{
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

#endif