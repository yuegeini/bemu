#ifndef UART_MONITOR_H
#define UART_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

struct Vuart_top;

void uart_monitor(struct Vuart_top *uart);

void uart_monitor_update_divisor();

#ifdef __cplusplus
}
#endif

#endif