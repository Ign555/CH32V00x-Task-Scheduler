#include "rtc.h"

IC *i_see;

void IC_delay_ms(uint32_t delay){
    for(uint64_t i = 0; i < delay*DELAY_MS_TIME; i++);
}

IC *IC_init(uint8_t stack_size){

    IC *ic;

    SysTick->CTLR = 0x0; //Disable counter 

    ic = malloc(sizeof(IC));

    ic->tsp  = 0;

    //Init task_stack
    ic->task_stack = malloc(sizeof(Task *) * stack_size);
    IC_clear_task_stack(ic);

    //Enable systick interuption
    NVIC_EnableIRQ(SysTick_IRQn);

    //Init systick
    SysTick->CTLR = 0x0000;
    SysTick->CMP = DELAY_MS_TIME - 1;
    SysTick->CNT = 0x00000000;

    ic->timestamp = 0;
    SysTick->CTLR = 0xF; //Enable counter 

    return ic;

}

void IC_add_task(IC *ic, void (*function)(void), uint32_t cooldown){

    ic->task_stack[ic->tsp] = Task_init(function, cooldown);
    qsort(ic->task_stack, ic->tsp + 1, sizeof(Task *), _IC_stack_sort);
    ic->tsp++;


}
void IC_pull_out_task(IC *ic){
    for(uint8_t i = 0; i < ic->tsp; i++)ic->task_stack[i] = ic->task_stack[i+1];
    if(ic->tsp>1){ic->tsp--;}else{IC_clear_task_stack(ic);}
}

void IC_decount_task_cooldown(IC *ic){
    for(uint8_t i = 0; i < ic->tsp; i++)ic->task_stack[i]->cooldown--;
}

/*
void _task_free(IC *ic){
    free(ic->);
}
*/
void IC_clear_task_stack(IC *ic){
    //_task_free(ic);
    ic->task_stack[0] = Task_init(NULL, -1);
    ic->tsp = 0;
}

uint8_t IC_check_timeout(IC *ic){
    if(ic->task_stack[0] == NULL) return 0;
    return (ic->task_stack[0]->cooldown == 0) ? 1 : 0;
}

Task *Task_init(void (*function)(void), uint32_t cooldown){

    Task *t = malloc(sizeof(Task));
    t->cooldown = cooldown;
    t->function = function;

    return t;

}

int _IC_stack_sort(const void *a, const void *b){
    const Task *t1 = *(Task * const *)a;
    const Task *t2 = *(Task * const *)b;
    return (int)(t1->cooldown - t2->cooldown);
}
