#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {

    uint32_t pc;

    uint32_t regs[32];

} CPU;

void cpu_run(CPU *cpu);
void cpu_step(CPU *cpu);

#endif