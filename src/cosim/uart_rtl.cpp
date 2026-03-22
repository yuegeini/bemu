#include "transaction.h"
#include "uart_model.h"
#include "uart_rtl.h"
#include <stdio.h>

extern "C" void uart_rtl_handle(bus_txn_t *t)
{
    if (!t->write)
        return;

    if (t->addr >= 0x10000000 && t->addr <= 0x100000ff)
    {
        uart_write_reg(t->addr - 0x10000000, t->data & 0xff);
   
        printf("[RTL] UART TX: %c\n", (char)t->data);
    }
}