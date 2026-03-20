#include "cpu.h"
#include "memory.h"
#include "bus.h"

extern void execute(CPU *cpu, uint32_t inst);

void dump_regs(CPU *cpu)
{
    for (int i = 0; i < 32; i++) {
        printf("x%d=%08x ", i, cpu->regs[i]);

        if ((i % 4) == 3)
            printf("\n");
    }
}
void disasm(uint32_t inst)
{
    uint32_t opcode = inst & 0x7f;
    uint32_t rd  = (inst >> 7) & 0x1f;
    uint32_t rs1 = (inst >> 15) & 0x1f;
    int32_t imm  = (int32_t)inst >> 20;

    if (opcode == 0x13) {
        printf("addi x%d,x%d,%d\n", rd, rs1, imm);
    }
}
void cpu_run(CPU *cpu)
{
    while (1) {


        uint32_t inst = load32(cpu->pc);
        
        printf("PC=%08x INST=%08x\n", cpu->pc, inst);
        disasm(inst);

        execute(cpu, inst);

        // dump_regs(cpu);

        cpu->regs[0] = 0;
    }
}

void cpu_step(CPU *cpu)
{
    uint32_t inst = load32(cpu->pc);

    printf("PC=%08x INST=%08x\n", cpu->pc, inst);
    disasm(inst);

    execute(cpu, inst);

    cpu->regs[0] = 0;
}