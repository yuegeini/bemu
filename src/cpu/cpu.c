#include "cpu.h"
#include "memory.h"
#include "bus.h"
#include "util.h"

extern void execute(CPU *cpu, uint32_t inst);

extern int32_t sign_extend(uint32_t val, int bits);

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

    uint32_t funct3 = (inst >> 12) & 7;
    uint32_t rs2    = (inst >> 20) & 0x1f;
    uint32_t funct7 = inst >> 25;


    switch (opcode)
    {
        case 0x33:
        {
            if (funct3 == 0 && funct7 == 0)
                printf("add x%d,x%d,x%d\n", rd, rs1, rs2);

            else if (funct3 == 0 && funct7 == 0x20)
                printf("sub x%d,x%d,x%d\n", rd, rs1, rs2);

            else if (funct3 == 7)
                printf("and x%d,x%d,x%d\n", rd, rs1, rs2);

            else if (funct3 == 6)
                printf("or x%d,x%d,x%d\n", rd, rs1, rs2);

            else if (funct3 == 4)
                printf("xor x%d,x%d,x%d\n", rd, rs1, rs2);

            break;
        }
        case 0x13:
        {
            int32_t imm = sign_extend(inst >> 20, 12);

            if (funct3 == 0)
                printf("addi x%d,x%d,%d\n", rd, rs1, imm);

            break;
        }
        case 0x03:
        {
            int32_t imm = sign_extend(inst >> 20, 12);

            switch (funct3) {

                case 0:
                    printf("lb x%d,%d(x%d)\n", rd, imm, rs1);
                    break;

                case 1:
                    printf("lh x%d,%d(x%d)\n", rd, imm, rs1);
                    break;

                case 2:
                    printf("lw x%d,%d(x%d)\n", rd, imm, rs1);
                    break;

                case 4:
                    printf("lbu x%d,%d(x%d)\n", rd, imm, rs1);
                    break;

                case 5:
                    printf("lhu x%d,%d(x%d)\n", rd, imm, rs1);
                    break;
            }

            break;
        }
        case 0x23: // STORE
        {
            int32_t imm =
                ((inst >> 7) & 0x1f) |
                ((inst >> 25) << 5);

            imm = sign_extend(imm, 12);

            switch (funct3) {

                case 0:
                    printf("sb x%d,%d(x%d)\n", rs2, imm, rs1);
                    break;

                case 1:
                    printf("sh x%d,%d(x%d)\n", rs2, imm, rs1);
                    break;

                case 2:
                    printf("sw x%d,%d(x%d)\n", rs2, imm, rs1);
                    break;
            }

            break;
        }
        case 0x63:
        {
            int32_t imm =
                ((inst >> 31) << 12) |
                (((inst >> 7) & 1) << 11) |
                (((inst >> 25) & 0x3f) << 5) |
                (((inst >> 8) & 0xf) << 1);

            imm = sign_extend(imm, 13);

            switch (funct3) {

                case 0:
                    printf("beq x%d,x%d,%d\n", rs1, rs2, imm);
                    break;

                case 1:
                    printf("bne x%d,x%d,%d\n", rs1, rs2, imm);
                    break;

                case 4:
                    printf("blt x%d,x%d,%d\n", rs1, rs2, imm);
                    break;

                case 5:
                    printf("bge x%d,x%d,%d\n", rs1, rs2, imm);
                    break;
            }

            break;
        }
        case 0x6f:
        {
            int32_t imm =
                ((inst >> 31) << 20) |
                (((inst >> 21) & 0x3ff) << 1) |
                (((inst >> 20) & 1) << 11) |
                (((inst >> 12) & 0xff) << 12);

            imm = sign_extend(imm, 21);

            printf("jal x%d,%d\n", rd, imm);

            break;
        }
        case 0x67:
        {
            int32_t imm = sign_extend(inst >> 20, 12);

            printf("jalr x%d,%d(x%d)\n", rd, imm, rs1);

            break;
        }
        case 0x37:
        {
            printf("lui x%d,0x%x\n", rd, inst >> 12);
            break;
        }
        case 0x17:
        {
            printf("auipc x%d,0x%x\n", rd, inst >> 12);
            break;
        }
        case 0x73:
        {
            if (inst == 0x00000073)
                printf("ecall\n");

            else if (inst == 0x00100073)
                printf("ebreak\n");

            break;
        }
    }
}
void cpu_run(CPU *cpu)
{
    while (1) {


        uint32_t inst = load32(cpu->pc);
        
        // printf("PC=%08x INST=%08x\n", cpu->pc, inst);
        // disasm(inst);

        execute(cpu, inst);

        // dump_regs(cpu);

        cpu->regs[0] = 0;
    }
}

void cpu_step(CPU *cpu)
{
    uint32_t inst = load32(cpu->pc);

    printf("PC=%08x INST=%08x\n", cpu->pc, inst);
    // disasm(inst);

    execute(cpu, inst);

    cpu->regs[0] = 0;
}