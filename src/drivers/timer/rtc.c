#include "rtc.h"
#include <io.h>
#include <screen.h>

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

static uint8_t cmos_read(uint8_t reg)
{
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}

static uint8_t bcd_to_bin(uint8_t val)
{
    return (val & 0x0F) + ((val / 16) * 10);
}

void rtc_read(rtc_time_t *t)
{
    t->second = bcd_to_bin(cmos_read(0x00));
    t->minute = bcd_to_bin(cmos_read(0x02));
    t->hour = bcd_to_bin(cmos_read(0x04));

    t->day = bcd_to_bin(cmos_read(0x07));
    t->month = bcd_to_bin(cmos_read(0x08));
    t->year = bcd_to_bin(cmos_read(0x09)) + 2000;
}

void print2(uint8_t v)
{
    if (v < 10)
        screen_put_char('0');

    screen_write_dec(v);
}

void rtc_print()
{
    rtc_time_t t;
    rtc_read(&t);

    cursor_pos_t pos;
    pos.row = 0;
    pos.col = 58; // top-right area

    cursor_pos_t cur_pos = screen_get_cursor();

    screen_set_cursor(pos);

    screen_write_dec(t.year);
    screen_put_char('-');
    print2(t.month);
    screen_put_char('-');
    print2(t.day);

    screen_put_char(' ');

    print2(t.hour);
    screen_put_char(':');
    print2(t.minute);
    screen_put_char(':');
    print2(t.second);

    screen_set_cursor(cur_pos);
}