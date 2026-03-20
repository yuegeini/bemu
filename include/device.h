#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

typedef struct device {

    const char *name;

    uint32_t start;
    uint32_t end;

    uint32_t (*read32)(uint32_t addr);
    void (*write32)(uint32_t addr, uint32_t value);

} device_t;

#endif