#include "transaction.h"
#include "uart_model.h"
#include "uart_rtl.h"
#include "uart_monitor.h"
#include <stdio.h>

static uint16_t shadow_divisor = 1;
static int dlab = 0;
void uart_monitor_update_divisor(uint32_t addr, uint8_t data)
{
    if (addr == 3) // LCR
    {
        dlab = (data & 0x80) != 0;
    }

    if (dlab && addr == 0) // DLL
    {
        shadow_divisor = (shadow_divisor & 0xff00) | data;
    }

    if (dlab && addr == 1) // DLM
    {
        shadow_divisor = (shadow_divisor & 0x00ff) | (data << 8);
    }

    if (shadow_divisor == 0)
        shadow_divisor = 1;

    uart_bit_cycles = 16 * shadow_divisor;

    printf("[UART] divisor=%d bit_cycles=%d\n",
           shadow_divisor, uart_bit_cycles);
}

extern "C" void uart_rtl_handle(bus_txn_t *t)
{
    if (t->addr > 7)
        return;

    if (t->write)
    {
        uart_write_reg(t->addr, t->data & 0xff);

        if (t->addr == 0 || t->addr == 1 || t->addr == 3)
            uart_monitor_update_divisor(t->addr, t->data & 0xff);
    }
    else
    {
        t->data = uart_read_reg(t->addr);
    }
}