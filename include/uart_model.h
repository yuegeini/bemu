#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_init();
void uart_write_reg(uint32_t addr, uint8_t val);
uint8_t uart_read_reg(uint32_t addr);
void uart_tick();
void uart_register_clock();

#ifdef __cplusplus
}
#endif