#ifndef RAM_H
#define RAM_H

#include <stdint.h>
#include <string.h>

#define RAM_SIZE (1024 * 1024)

void ram_load(uint8_t *data, size_t size);

#endif

