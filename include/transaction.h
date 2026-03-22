#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>

typedef struct {

    uint32_t addr;
    uint32_t data;

    int size;

    int write;

} bus_txn_t;

void txn_dispatch(bus_txn_t *t);

#endif