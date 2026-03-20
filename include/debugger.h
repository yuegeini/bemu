#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "cpu.h"
#include <string.h>
#include "memory.h"

#define MAX_HISTORY 10000

typedef struct {

    uint32_t pc;
    uint32_t regs[32];

} cpu_snapshot_t;

typedef struct {

    cpu_snapshot_t history[MAX_HISTORY];
    int step;
    uint32_t breakpoints[32];
    int bp_count;

} debugger_t;

void debugger_run(debugger_t *dbg, CPU *cpu);


#endif