#include <stdio.h>
#include <stdlib.h>
#include "ch32v00x.h"

#ifndef __RTC_H__
#define __RTC_H__

#define task_stack_pointer tsp

#define DELAY_MS_TIME SystemCoreClock/1000 //The number of clock cycle for 1ms

typedef struct RTC{

    int year;
    uint8_t month, day, hour, minute, second;

}RTC;

typedef struct Task{
    void (*function)();
    uint32_t cooldown;
}Task;

Task *Task_init(void (*function)(), uint32_t cooldown);

typedef struct internal_counter{

    uint64_t timestamp;

    Task **task_stack;
    uint8_t task_stack_pointer;

}IC;

IC *IC_init(uint8_t stack_size);
void IC_add_task(IC *ic, void (*function)(void), uint32_t cooldown);
void IC_pull_out_task(IC *ic);
void IC_delay_ms(uint32_t delay);
void IC_decount_task_cooldown(IC *ic);
uint8_t IC_check_timeout(IC *ic);
void IC_clear_task_stack(IC *ic);
int _IC_stack_sort(const void *a, const void *b);

#endif