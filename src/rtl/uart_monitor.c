#include <stdio.h>
#include "Vuart_top.h"
#include "uart_monitor.h"
#include "uart_model.h"

enum {
    UART_WAIT_START,
    UART_READ_BITS,
    UART_STOP_BIT
};

static int state = UART_WAIT_START;

static int bit_count = 0;
static int bit_timer = 0;

static uint8_t rx_byte = 0;

static int last_tx = 1;

static uint16_t divisor = 1;
static int bit_cycles = 16;

/* число тактов на бит
   нужно подобрать (обычно ~16 * divisor)
*/


void uart_monitor_update_divisor()
{
    uint8_t dll = uart_read_reg(0);
    uint8_t dlm = uart_read_reg(1);

    divisor = dll | (dlm << 8);

    if (divisor == 0)
        divisor = 1;

    bit_cycles = 16 * divisor;

    printf("[UART MON] divisor=%d bit_cycles=%d\n", divisor, bit_cycles);
}

void uart_monitor(Vuart_top *uart)
{
    int tx = uart->stx_pad_o;
    // printf("[UART bit %d] = %d\n", bit_count, tx);
    switch (state)
    {
        case UART_WAIT_START:

            if (last_tx == 1 && tx == 0)
            {
                state = UART_READ_BITS;
                bit_count = 0;
                rx_byte = 0;
                bit_timer = bit_cycles;

                printf("[RTL] UART START BIT\n");
            }

            break;

        case UART_READ_BITS:

            if (--bit_timer == 0)
            {
                rx_byte |= (tx << bit_count);

                bit_count++;

                bit_timer = bit_cycles;

                if (bit_count == 8)
                    state = UART_STOP_BIT;
            }

            break;

        case UART_STOP_BIT:

            if (--bit_timer == 0)
            {
                printf("[RTL UART RX] %c (0x%02x)\n", rx_byte, rx_byte);

                state = UART_WAIT_START;
            }

            break;
    }

    last_tx = tx;
}