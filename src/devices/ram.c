#include "device.h"
#include "memory.h"
#include "ram.h"

static uint8_t ram[RAM_SIZE];

void ram_load(uint8_t *data, size_t size)
{
    if (size > RAM_SIZE)
        size = RAM_SIZE;

    memset(ram, 0, RAM_SIZE);
    memcpy(ram, data, size);
}

static uint32_t ram_read(uint32_t addr, int size)
{
    if (addr + size > RAM_SIZE) {
        printf("RAM read OOB: %x\n", addr);
        exit(1);
    }

    uint32_t val = 0;

    for (int i = 0; i < size; i++)
        val |= ram[addr+i] << (8*i);

    return val;
}

static void ram_write(uint32_t addr, int size, uint32_t val)
{
    if (addr + size > RAM_SIZE) {
        printf("RAM write OOB: %x\n", addr);
        exit(1);
    }

    for (int i = 0; i < size; i++)
        ram[addr+i] = (val >> (8*i)) & 0xff; 

}

device_t ram_device = {
    .name = "RAM",
    .start = 0x00000000,
    .end   = RAM_SIZE - 1,
    .read = ram_read,
    .write = ram_write
};