#include <stdio.h>
#include "uart_ref.h"

static uint8_t lcr = 0;

void uart_ref_handle(bus_txn_t *t)
{
    if (!t->write)
        return;

    switch (t->addr)
    {
        case 3:
            lcr = t->data;
            break;

        case 0:
            if (!(lcr & 0x80))
                printf("[REF UART TX] %x\n", (uint32_t)t->data);
            break;
    }
}