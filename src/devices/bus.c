#include <stdio.h>
#include <stdlib.h>

#include "transaction.h"
#include "bus.h"

#define MAX_DEVICES 16

static device_t *devices[MAX_DEVICES];
static int device_count = 0;

void bus_add_device(device_t *dev)
{
    printf("BUS: add device %s [%08x-%08x]\n",
           dev->name, dev->start, dev->end);

    devices[device_count++] = dev;
}

static device_t *bus_find(uint32_t addr)
{
    for (int i = 0; i < device_count; i++) {

        if (addr >= devices[i]->start &&
            addr <= devices[i]->end)
            return devices[i];
    }

    return NULL;
}

uint32_t bus_read32(uint32_t addr)
{
    device_t *dev = bus_find(addr);

    if (!dev) {
        printf("BUS READ 32 unknown addr %08x\n", addr);
        exit(1);
    }
    printf("BUS READ 32 addr=%08x -> device [%08x-%08x] (%s)\n",
           addr, dev->start, dev->end, dev->name);
    return dev->read(addr, 4);
}

uint32_t bus_read16(uint32_t addr)
{
    device_t *dev = bus_find(addr);

    if (!dev) {
        printf("BUS READ 16 unknown addr %08x\n", addr);
        exit(1);
    }
    printf("BUS READ 16 addr=%08x -> device [%08x-%08x] (%s)\n",
           addr, dev->start, dev->end, dev->name);
    return dev->read(addr, 2);
}

uint32_t bus_read8(uint32_t addr)
{
    device_t *dev = bus_find(addr);

    if (!dev) {
        printf("BUS READ 8 unknown addr %08x\n", addr);
        exit(1);
    }
    printf("BUS READ 8 addr=%08x -> device [%08x-%08x] (%s)\n",
           addr, dev->start, dev->end, dev->name);
    return dev->read(addr, 1);
}

void bus_write32(uint32_t addr, uint32_t value)
{
    device_t *dev = bus_find(addr);

    if (!dev) {
        printf("BUS WRITE 32 unknown addr %08x\n", addr);
        exit(1);
    }
    printf("BUS WRITE 32 addr=%08x value=%08x -> device [%08x-%08x] (%s)\n",
           addr, value, dev->start, dev->end, dev->name);

    dev->write(addr, 4, value);
    
}

void bus_write16(uint32_t addr, uint32_t value)
{
    device_t *dev = bus_find(addr);

    if (!dev) {
        printf("BUS WRITE 16 unknown addr %08x\n", addr);
        exit(1);
    }
    printf("BUS WRITE 16 addr=%08x value=%08x -> device [%08x-%08x] (%s)\n",
           addr, value, dev->start, dev->end, dev->name);

    dev->write(addr, 2, value);
    
}

void bus_write8(uint32_t addr, uint32_t value)
{
    device_t *dev = bus_find(addr);

    if (!dev) {
        printf("BUS WRITE 8 unknown addr %08x\n", addr);
        exit(1);
    }
    printf("BUS WRITE 8 addr=%08x value=%08x -> device [%08x-%08x] (%s)\n",
           addr, value, dev->start, dev->end, dev->name);

    dev->write(addr, 1, value);
    
}