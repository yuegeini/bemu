#ifndef UART_MONITOR_H
#define UART_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

extern int uart_bit_cycles;
extern int uart_in_monitor;
struct Vuart_top;

void uart_monitor(struct Vuart_top *uart);
void uart_rx_drive(struct Vuart_top *uart);

void uart_rx_push(uint8_t data); 

void uart_monitor_update_divisor();

#ifdef __cplusplus
}
#endif

#endif