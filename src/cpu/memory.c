#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "bus.h"


uint8_t load8(uint32_t addr)
{
    return bus_read8(addr);
}

uint16_t load16(uint32_t addr)
{
    return bus_read16(addr);
}

uint32_t load32(uint32_t addr)
{
    return bus_read32(addr);
}

void store8(uint32_t addr, uint8_t val)
{
    bus_write8(addr, val);
}

void store16(uint32_t addr, uint16_t val)
{
    bus_write16(addr, val);
}

void store32(uint32_t addr, uint32_t val)
{
    bus_write32(addr, val);
}