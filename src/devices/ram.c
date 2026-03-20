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

static uint32_t ram_read(uint32_t addr)
{
    if (addr + 3 >= RAM_SIZE) {
        printf("RAM read OOB: %x\n", addr);
        exit(1);
    }

    uint32_t val = 0;

    val |= ram[addr];
    val |= ram[addr + 1] << 8;
    val |= ram[addr + 2] << 16;
    val |= ram[addr + 3] << 24;

    return val;
}

static void ram_write(uint32_t addr, uint32_t val)
{
    if (addr + 3 >= RAM_SIZE) {
        printf("RAM write OOB: %x\n", addr);
        exit(1);
    }

    ram[addr] = val & 0xff;
    ram[addr + 1] = (val >> 8) & 0xff;
    ram[addr + 2] = (val >> 16) & 0xff;
    ram[addr + 3] = (val >> 24) & 0xff;
}

device_t ram_device = {
    .name = "RAM",
    .start = 0x00000000,
    .end   = RAM_SIZE - 1,
    .read32 = ram_read,
    .write32 = ram_write
};