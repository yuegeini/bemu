#ifndef UART_H
#define UART_H

#include "device.h"

extern device_t uart_device;

typedef struct {
    uint8_t rx_buf;
    int rx_ready;

    int tx_ready;
} uart_t;

static uart_t uart = {
    .rx_ready = 0,
    .tx_ready = 1
};

#endif