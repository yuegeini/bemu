#include <stdio.h>
#include "device.h"
#include "uart.h"
#include "uart_model.h"
#include "transaction.h"

static uint32_t uart_read(uint32_t addr, int size)
{
    uint32_t offset = addr - 0x10000000;

    bus_txn_t t;

    t.addr  = offset;
    t.data  = 0;
    t.size  = size;
    t.write = 0;

    txn_dispatch(&t);
    printf("[UART READ] off=%x val=%02x\n", offset, t.data);
    return t.data;
}

static void uart_write(uint32_t addr, int size, uint32_t value)
{
    uint32_t offset = addr - 0x10000000;

    bus_txn_t t;

    t.addr  = offset;
    t.data  = value;
    t.size  = size;
    t.write = 1;

    printf("[UART WRITE] off=%x val=%02x\n", offset, value);

    txn_dispatch(&t);
}

device_t uart_device = {

    .name = "UART",

    .start = 0x10000000,
    .end   = 0x10000007,

    .read = uart_read,
    .write = uart_write
};
