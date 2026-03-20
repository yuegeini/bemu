#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"
#include "bus.h"
#include "ram.h"
#include "uart.h"

extern device_t ram_device;
extern device_t uart_device;

CPU cpu;

void load_program(const char *path)
{
    FILE *f = fopen(path, "rb");

    if (!f) {
        printf("Failed to open program\n");
        return;
    }

    uint8_t buffer[RAM_SIZE];

    size_t size = fread(buffer, 1, RAM_SIZE, f);
    fclose(f);

    printf("Loaded %zu bytes\n", size);

    ram_load(buffer, size);
}

int main()
{
    memset(&cpu, 0, sizeof(cpu));
    cpu.pc = 0;
    cpu.regs[2] = RAM_SIZE;
    bus_add_device(&ram_device);
    bus_add_device(&uart_device);
    
    load_program("program.bin");

    cpu_run(&cpu);

    return 0;
}