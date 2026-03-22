#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "bus.h"
#include "memory.h"
#include "util.h"
#include "trace.h"

int32_t sign_extend(uint32_t val, int bits)
{
    int shift = 32 - bits;
    return (int32_t)(val << shift) >> shift;
}

void execute(CPU *cpu, uint32_t inst)
{
    uint32_t opcode = inst & 0x7f;

    uint32_t rd = (inst >> 7) & 0x1f;
    uint32_t funct3 = (inst >> 12) & 7;
    uint32_t rs1 = (inst >> 15) & 0x1f;
    uint32_t rs2 = (inst >> 20) & 0x1f;
    uint32_t funct7 = inst >> 25;

    switch (opcode)
    {

        case 0x33: // ADD
        {
            if (funct3 == 0 && funct7 == 0)
                cpu->regs[rd] = cpu->regs[rs1] + cpu->regs[rs2];

            cpu->pc += 4;
            break;
        }

        case 0x13: // ADDI
        {
            int32_t imm = sign_extend(inst >> 20, 12);
            uint32_t res = cpu->regs[rs1] + imm;

            trace("%08x  addi  x%d,x%d,%d    x%d=%08x\n",
                cpu->pc, rd, rs1, imm, rd, res);

            cpu->regs[rd] = cpu->regs[rs1] + imm;

            cpu->pc += 4;
            break;
        }
        case 0x03: // LOAD
        {
            int32_t imm = sign_extend(inst >> 20, 12);
            uint32_t addr = cpu->regs[rs1] + imm;

            switch (funct3) {

                case 0: cpu->regs[rd] = (int8_t)load8(addr);   break;   // LB
                case 1: cpu->regs[rd] = (int16_t)load16(addr); break;   // LH
                case 2: cpu->regs[rd] = load32(addr);          break;   // LW
                case 4: cpu->regs[rd] = load8(addr);           break;   // LBU
                case 5: cpu->regs[rd] = load16(addr);          break;   // LHU
            }

            trace("%08x  lw    x%d,%d(x%d)   x%d=%08x\n",
                cpu->pc, rd, imm, rs1, rd, cpu->regs[rd]);
            
            cpu->pc += 4;
            break;
        }
        case 0x23: // STORE
        {
            int32_t imm =
                ((inst >> 7) & 0x1f) |
                ((inst >> 25) << 5);

            imm = sign_extend(imm, 12);

            uint32_t addr = cpu->regs[rs1] + imm;

            switch (funct3) {

                case 0: store8(addr, cpu->regs[rs2]);       break;   // SB
                case 1: store16(addr, cpu->regs[rs2]);      break;   // SH
                case 2: store32(addr, cpu->regs[rs2]);      break;   // SW
            }

            trace("%08x  sw    x%d,%d(x%d)   mem[%08x]=%08x\n",
                cpu->pc, rs2, imm, rs1, addr, cpu->regs[rs2]);

            cpu->pc += 4;
            break;
        }
        case 0x37: // LUI
        {
            uint32_t imm = inst & 0xfffff000;

            cpu->regs[rd] = imm;

            cpu->pc += 4;

            break;
        }
        case 0x6F: // JAL
        {
            int32_t imm =
                ((inst >> 31) << 20) |
                (((inst >> 21) & 0x3ff) << 1) |
                (((inst >> 20) & 1) << 11) |
                (((inst >> 12) & 0xff) << 12);

            imm = sign_extend(imm, 21);

            cpu->regs[rd] = cpu->pc + 4;

            cpu->pc += imm;

            break;
        }
        case 0x63: // BRANCH
        {
            int32_t imm = 0;

            imm |= ((inst >> 8) & 0xF) << 1;
            imm |= ((inst >> 25) & 0x3F) << 5;
            imm |= ((inst >> 7) & 0x1) << 11;
            imm |= ((inst >> 31) & 0x1) << 12;

            imm = sign_extend(imm, 13);

            int take = 0;

            switch (funct3)
            {
                case 0: // BEQ
                    take = (cpu->regs[rs1] == cpu->regs[rs2]);
                    break;

                case 1: // BNE
                    take = (cpu->regs[rs1] != cpu->regs[rs2]);
                    break;

                case 4: // BLT
                    take = ((int32_t)cpu->regs[rs1] < (int32_t)cpu->regs[rs2]);
                    break;

                case 5: // BGE
                    take = ((int32_t)cpu->regs[rs1] >= (int32_t)cpu->regs[rs2]);
                    break;

                default:
                    printf("Unknown branch type\n");
                    exit(1);
            }

            trace("%08x  beq   x%d,x%d,%d   %s\n",
                cpu->pc, rs1, rs2, imm,
                take ? "taken" : "not taken");

            if (take)
                cpu->pc += imm;
            else
                cpu->pc += 4;

            break;
        }
        case 0x67: // JALR
        {
            int32_t imm = sign_extend(inst >> 20, 12);

            uint32_t target = cpu->regs[rs1] + imm;

            cpu->regs[rd] = cpu->pc + 4;

            cpu->pc = target & ~1;

            break;
        }
        case 0x17: // AUIPC
        {
            uint32_t imm = inst & 0xfffff000;

            cpu->regs[rd] = cpu->pc + imm;

            cpu->pc += 4;

            break;
        }
        case 0x73: // SYSTEM
        {
            uint32_t funct12 = inst >> 20;

            if (funct12 == 1) { // EBREAK
                printf("Program finished (EBREAK)\n");
                exit(0);
            }

            cpu->pc += 4;
            break;
        }

        default:
        {
            printf("Unknown opcode: %x\n", opcode);
            exit(1);
        }
    }
}