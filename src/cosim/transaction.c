#include <stdio.h>
#include "transaction.h"
#include "uart_ref.h"
#include "uart_rtl.h"
#include "uart_model.h"

void txn_dispatch(bus_txn_t *t)
{

    /* REF model */

    uart_ref_handle(t);

    /* RTL model (пока заглушка) */

    uart_rtl_handle(t);

}