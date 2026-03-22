#include <stdio.h>
#include "uart_ref.h"

#define UART_BASE 0x10000000
#define UART_END  0x10000007


void uart_ref_handle(bus_txn_t *t)
{
    if (!t->write)
        return;

    if (t->addr == 0x10000000)
        printf("[REF] UART TX: %c\n", (char)t->data);
}