#include <stdio.h>
#include "transaction.h"
#include "uart_ref.h"
#include "uart_rtl.h"
#include "uart_model.h"
#include <stdlib.h>
void txn_dispatch(bus_txn_t *t)
{

    bus_txn_t t_ref = *t;
    bus_txn_t t_rtl = *t;

    uart_ref_handle(&t_ref);
    uart_rtl_handle(&t_rtl);

    printf("[CHECK] addr=%x %s ref=%02x rtl=%02x\n",
        t->addr,
        t->write ? "WR" : "RD",
        t_ref.data,
        t_rtl.data);

    if (t->addr == 5 && !t->write) {
        if (t_ref.data != t_rtl.data) {
            printf("❌ LSR mismatch ref=%02x rtl=%02x\n",
                t_ref.data, t_rtl.data);
        }
    }
    
    if (!t->write) {
        if (t_ref.data != t_rtl.data) {
                printf("❌ MISMATCH READ addr=%x ref=%02x rtl=%02x\n",
                    t->addr, t_ref.data, t_rtl.data);
                exit(1);
            }
    }
}