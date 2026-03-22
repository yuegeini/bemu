#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "memory.h"
#include "bus.h"
#include "ram.h"
#include "uart.h"
#include "debugger.h"
#include "uart_model.h"

extern device_t ram_device;
extern device_t uart_device;

CPU cpu;

void soc_init()
{
    bus_add_device(&ram_device);
    bus_add_device(&uart_device);
}

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
    CPU cpu = {0};
    debugger_t dbg = {0};
    
    soc_init();
    uart_init();

    memset(&cpu, 0, sizeof(cpu));
    cpu.pc = 0;
    cpu.regs[2] = RAM_SIZE;
    
    load_program("program.bin");

    cpu_run(&cpu);
    // debugger_run(&dbg, &cpu);
    return 0;
}

