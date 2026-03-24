#ifndef SIM_H
#define SIM_H

#include <stdint.h>
#include "scheduler.h"


void sim_register_clocked(clocked_device_t dev);

void sim_tick();

void sim_run_cycles(int cycles);
extern int uart_rx_ready();

extern uint64_t sim_time;

#endif