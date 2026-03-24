#include <stdio.h>
#include "uart_ref.h"

#define FIFO_SIZE 16

typedef struct {
    uint8_t lcr;

    // FIFO
    uint8_t rx_fifo[FIFO_SIZE];
    int rx_head, rx_tail;

    uint8_t tx_fifo[FIFO_SIZE];
    int tx_head, tx_tail;

} uart_ref_t;

static uart_ref_t u;

void uart_ref_init()
{
    u.lcr = 0;
    u.rx_head = u.rx_tail = 0;
    u.tx_head = u.tx_tail = 0;
}

static int fifo_empty(int head, int tail)
{
    return head == tail;
}

static int fifo_full(int head, int tail)
{
    return ((head + 1) % FIFO_SIZE) == tail;
}

void uart_ref_push_rx(uint8_t c)
{
    if (!fifo_full(u.rx_head, u.rx_tail)) {
        u.rx_fifo[u.rx_head] = c;
        u.rx_head = (u.rx_head + 1) % FIFO_SIZE;
    } else {
        printf("[REF] RX FIFO overflow\n");
    }
}
void uart_ref_handle(bus_txn_t *t)
{
    uint32_t addr = t->addr;

    if (t->write)
    {
        switch (addr)
        {
            case 3: // LCR
                u.lcr = t->data;
                break;

            case 0: // THR / DLL
                if (u.lcr & 0x80) {
                    // DLL — пока игнорим
                } else {
                    // TX FIFO push
                    if (!fifo_full(u.tx_head, u.tx_tail)) {
                        u.tx_fifo[u.tx_head] = t->data;
                        u.tx_head = (u.tx_head + 1) % FIFO_SIZE;

                        printf("[REF TX enqueue] %02x\n", t->data);
                    } else {
                        printf("[REF] TX FIFO overflow\n");
                    }
                }
                break;

            case 1:
                // DLM — игнор
                break;
        }
    }
    else
    {
        switch (addr)
        {
            case 0: // RBR
                if (!fifo_empty(u.rx_head, u.rx_tail)) {
                    t->data = u.rx_fifo[u.rx_tail];
                    u.rx_tail = (u.rx_tail + 1) % FIFO_SIZE;

                    printf("[REF RX pop] %02x\n", t->data);
                } else {
                    t->data = 0;
                }
                break;

            case 5: // LSR
            {
                uint8_t lsr = 0;

                // DR
                if (!fifo_empty(u.rx_head, u.rx_tail))
                    lsr |= 0x01;

                // THRE (FIFO not full)
                if (!fifo_full(u.tx_head, u.tx_tail))
                    lsr |= 0x20;

                // TEMT (FIFO empty)
                if (fifo_empty(u.tx_head, u.tx_tail))
                    lsr |= 0x40;

                t->data = lsr;
                break;
            }

            case 3:
                t->data = u.lcr;
                break;

            default:
                t->data = 0;
                break;
        }
    }
}