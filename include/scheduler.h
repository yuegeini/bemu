#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct clocked_device {

    const char *name;

    void (*tick)(void *opaque);

    void *opaque;
    
    int divider;

} clocked_device_t;

void scheduler_init();

#ifdef __cplusplus
extern "C" {
#endif

void scheduler_register(clocked_device_t dev);

#ifdef __cplusplus
}
#endif

void scheduler_tick();

void scheduler_run_cycles(int cycles);

#endif