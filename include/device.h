#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

typedef struct device {

    const char *name;

    uint32_t start;
    uint32_t end;

    uint32_t (*read)(uint32_t addr, int size);
    void (*write)(uint32_t addr, int size, uint32_t value);

} device_t;

#endif