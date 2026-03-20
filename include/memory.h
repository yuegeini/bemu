#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


uint8_t  load8(uint32_t addr);
uint16_t load16(uint32_t addr);
uint32_t load32(uint32_t addr);

void store8(uint32_t addr, uint8_t val);
void store16(uint32_t addr, uint16_t val);
void store32(uint32_t addr, uint32_t val);

#endif