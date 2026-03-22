#include "scheduler.h"
#include "sim.h"

#define MAX_DEVICES 32

static clocked_device_t devices[MAX_DEVICES];
static int device_count = 0;

void scheduler_init()
{
    device_count = 0;
}

void scheduler_register(clocked_device_t dev)
{
    devices[device_count++] = dev;
}

void scheduler_tick()
{
    for (int i = 0; i < device_count; i++)
    {
        if (sim_time % devices[i].divider == 0)
            devices[i].tick(devices[i].opaque);
    }
}

void scheduler_run_cycles(int cycles)
{
    for (int i = 0; i < cycles; i++)
    {
        scheduler_tick();
    }
}

