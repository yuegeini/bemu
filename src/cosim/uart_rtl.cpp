#include "transaction.h"
#include "uart_model.h"
#include "uart_rtl.h"
#include "uart_monitor.h"

extern "C" void uart_rtl_handle(bus_txn_t *t)
{
    if (t->addr > 7)
        return;

    if (t->write)
    {
        uart_write_reg(t->addr, t->data & 0xff);

        /* проверить DLAB */

        uint8_t lcr = uart_read_reg(3);

        if (lcr & 0x80)
        {
            if (t->addr == 0 || t->addr == 1)
                uart_monitor_update_divisor();
        }
    }
    else
    {
        t->data = uart_read_reg(t->addr);
    }
}