#include "time.h"
#include <screen.h>

#define TIMER_HZ 18

static system_time_t sys_time;

void time_tick()
{
    sys_time.tick++;

    if (sys_time.tick % TIMER_HZ == 0) {
        sys_time.seconds++;

        if (sys_time.seconds == 60) {
            sys_time.seconds = 0;
            sys_time.minutes++;

            if (sys_time.minutes == 60) {
                sys_time.minutes = 0;
                sys_time.hours++;
            }
        }
    }
}

void time_render()
{
    cursor_pos_t pos;
    pos.row = 0;
    pos.col = 70; // top-right area

    cursor_pos_t cur_pos = screen_get_cursor();

    screen_set_cursor(pos);

    screen_write_dec(sys_time.hours);
    screen_put_char(':');
    screen_write_dec(sys_time.minutes);
    screen_put_char(':');
    screen_write_dec(sys_time.seconds);

    screen_set_cursor(cur_pos);
}