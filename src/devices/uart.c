#include <stdio.h>
#include "device.h"
#include "uart.h"
#include "uart_model.h"
#include "transaction.h"

static uint32_t uart_read(uint32_t addr, int size)
{
    uint32_t offset = addr - 0x10000000;


    switch (offset)
    {
        case 0x0: // RX
        {
            uart.rx_ready = 0;
            return uart.rx_buf;
        }

        case 0x4: // STATUS
        {
            uint32_t status = 0;

            if (uart.rx_ready)
                status |= 1;

            if (uart.tx_ready)
                status |= (1 << 5);

            return status;
        }

        default:
            return 0;
    }
}

static void uart_write(uint32_t addr, int size, uint32_t value)
{
       
    bus_txn_t t; 
    t.addr  = addr;
    t.data  = value;
    t.size  = size;
    t.write = 1;

    txn_dispatch(&t);
}

device_t uart_device = {

    .name = "UART",

    .start = 0x10000000,
    .end   = 0x100000ff,

    .read = uart_read,
    .write = uart_write
};
