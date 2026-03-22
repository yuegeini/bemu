#include "sim.h"

#define MAX_CLOCKED_DEVICES 32

static clocked_device_t devices[MAX_CLOCKED_DEVICES];
static int device_count = 0;

uint64_t sim_time = 0;

void sim_register_clocked(clocked_device_t dev)
{
    devices[device_count++] = dev;
}

void sim_tick()
{
    for (int i = 0; i < device_count; i++)
    {
        devices[i].tick(devices[i].opaque);
    }

    sim_time++;
}

void sim_run_cycles(int cycles)
{
    for (int i = 0; i < cycles; i++)
        sim_tick();
}