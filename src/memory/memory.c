#include "memory.h"

void memset(void *ptr, uint8_t value, size_t size){
    while (size--){
        *(char *)ptr = value;
        ptr++;
    }
}