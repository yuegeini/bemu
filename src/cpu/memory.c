#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "bus.h"



uint32_t load32(uint32_t addr)
{
    return bus_read32(addr);
}


void store32(uint32_t addr, uint32_t val)
{
    bus_write32(addr, val);
}