#include <stdio.h>
#include "Vuart_top.h"
#include "uart_monitor.h"
#include "uart_model.h"

int uart_bit_cycles = 32;
int uart_in_monitor = 0;

enum {
    UART_WAIT_START,
    UART_READ_BITS,
    UART_STOP_BIT
};

static int state = UART_WAIT_START;

static int bit_count = 0;
static uint8_t rx_byte = 0;
static int bit_timer = 0;
static int last_tx = 1;

static uint16_t divisor = 2;
static int bit_cycles = 16;

/* число тактов на бит
   нужно подобрать (обычно ~16 * divisor)
*/



void uart_monitor_update_divisor()
{
    uart_in_monitor = 1;
    uint8_t lcr = uart_read_reg(3);

    uart_write_reg(3, lcr | 0x80); // DLAB=1

    uint8_t dll = uart_read_reg(0);
    uint8_t dlm = uart_read_reg(1);


    uart_in_monitor = 0;
    uint16_t divisor = dll | (dlm << 8);

    if (divisor == 0)
        divisor = 1;

    uart_bit_cycles = 16 * divisor;

    printf("[UART] divisor=%d bit_cycles=%d\n", divisor, uart_bit_cycles);
}

void uart_monitor(Vuart_top *uart)
{

    int tx = uart->stx_pad_o;

    switch (state)
    {
        case UART_WAIT_START:

            if (last_tx == 1 && tx == 0)
            {
                state = UART_READ_BITS;
                bit_count = 0;
                rx_byte = 0;
                bit_timer = uart_bit_cycles + uart_bit_cycles / 2;

                printf("[RTL] UART START BIT\n");
    printf("[UART TX Start bit %d] = %d\n", bit_count, tx);
            }

            break;

        case UART_READ_BITS:

            if (--bit_timer == 0)
            {
                rx_byte |= (tx << bit_count);

                bit_count++;

                bit_timer = uart_bit_cycles;

                if (bit_count == 8)
                    state = UART_STOP_BIT;
                
    printf("[UART TX Read bit %d] = %d\n", bit_count, tx);
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