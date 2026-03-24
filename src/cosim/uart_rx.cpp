#include <queue>
#include <stdio.h>
#include "Vuart_top.h"
#include "uart_monitor.h"
#include "uart_model.h"

static std::queue<uint8_t> rx_fifo;
extern int uart_bit_cycles;
extern void uart_feed_rx(uint8_t c);

void uart_rx_push(uint8_t c)
{
    printf("[PUSH RX] %c\n", c);
    uart_feed_rx(c); // ✅ сразу кладем в регистр
    rx_fifo.push(c);
}

void uart_rx_drive(Vuart_top *uart)
{
    static int state = 0;
    static int bit = 0;
    static uint8_t data;
    static int baud_cnt = 0;
    printf("RX drive state=%d bit=%d val=%d\n", state, bit, uart->srx_pad_i);

    if (rx_fifo.empty() && state == 0)
    {
        uart->srx_pad_i = 1;
        return;
    }

    if (baud_cnt++ < uart_bit_cycles)
        return;

    baud_cnt = 0;

    switch(state)
    {
        case 0: // start
            data = rx_fifo.front();
            rx_fifo.pop();


            uart->srx_pad_i = 0;
            break;

        case 1: // data
            uart->srx_pad_i = (data >> bit) & 1;
            bit++;

            if (bit == 8)
                state = 2;
            break;

        case 2: // stop
            uart->srx_pad_i = 1;
            state = 0;
            break;
    }
}