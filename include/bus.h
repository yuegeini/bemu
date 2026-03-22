#ifndef BUS_H
#define BUS_H

#include "device.h"

void bus_add_device(device_t *dev);

uint32_t bus_read32(uint32_t addr);
uint32_t bus_read16(uint32_t addr);
uint32_t bus_read8(uint32_t addr);
void bus_write32(uint32_t addr, uint32_t value);
void bus_write16(uint32_t addr, uint32_t value);
void bus_write8(uint32_t addr, uint32_t value);

#endif