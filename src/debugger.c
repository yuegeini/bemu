#include <stdio.h>
#include "debugger.h"

static void save_snapshot(debugger_t *dbg, CPU *cpu)
{
    cpu_snapshot_t *s = &dbg->history[dbg->step];

    s->pc = cpu->pc;

    for (int i = 0; i < 32; i++)
        s->regs[i] = cpu->regs[i];

    dbg->step++;
}


static void restore_snapshot(debugger_t *dbg, CPU *cpu)
{
    if (dbg->step == 0) {
        printf("No history\n");
        return;
    }

    dbg->step--;

    cpu_snapshot_t *s = &dbg->history[dbg->step];

    cpu->pc = s->pc;

    for (int i = 0; i < 32; i++)
        cpu->regs[i] = s->regs[i];
}


void debugger_run(debugger_t *dbg, CPU *cpu)
{
    char cmd[128];

    while (1) {

        printf("emu> ");
        if (!fgets(cmd, sizeof(cmd), stdin))
            break;

        if (strncmp(cmd, "step", 4) == 0) {

            save_snapshot(dbg, cpu);
            for (int i = 0; i < dbg->bp_count; i++)
            {
                if (cpu->pc == dbg->breakpoints[i]) {
                    printf("Breakpoint hit at %08x\n", cpu->pc);
                    return;
                }
            }
            cpu_step(cpu);

        }

        else if (strncmp(cmd, "back", 4) == 0) {

            restore_snapshot(dbg, cpu);

        }

        else if (strncmp(cmd, "regs", 4) == 0) {

            for (int i = 0; i < 32; i++)
                printf("x%d = %08x\n", i, cpu->regs[i]);

        }

        else if (strncmp(cmd, "mem", 3) == 0) {

            uint32_t addr;

            sscanf(cmd, "mem %x", &addr);

            printf("mem[%08x] = %08x\n",
                   addr,
                   load32(addr));

        }

        else if (strncmp(cmd, "continue", 8) == 0) {

            while (1) {

                save_snapshot(dbg, cpu);
                cpu_step(cpu);

            }

        }

        else if (strncmp(cmd, "quit", 4) == 0) {

            break;

        }

    }
}