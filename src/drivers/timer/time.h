#ifndef TIME_H
#define TIME_H

#include <stdint.h>

typedef struct {
    uint32_t tick;
    uint32_t seconds;
    uint32_t minutes;
    uint32_t hours;

} system_time_t;

void time_tick();
void time_render();

#endif