#include <stdio.h>
#include "device.h"

static uint32_t uart_read(uint32_t addr)
{
    return 0;
}

static void uart_write(uint32_t addr, uint32_t value)
{
    putchar(value);
    fflush(stdout);
}

device_t uart_device = {

    .name = "UART",

    .start = 0x10000000,
    .end   = 0xffffffff,

    .read32 = uart_read,
    .write32 = uart_write
};