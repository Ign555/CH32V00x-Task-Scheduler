/****************************************************************
*
*
* Task scheduler for CH32V00X
* Created by Ign555
* Version : v1.0
* File Creation : 19/11/2025
*
*
****************************************************************/

#include "ic.h"

IC *i_see;

/****************************************************************
*
* Task scheduler interrupt functioon
*
****************************************************************/

__attribute__((interrupt("WCH-Interrupt-fast")))
void SysTick_Handler(){

    i_see->timestamp++;

    if(i_see->status){
        if(IC_check_timeout(i_see)){
            i_see->task_stack[0]->function();
            IC_pull_out_task(i_see);
        }
        IC_decount_task_cooldown(i_see);
    }

    SysTick->SR = 0; //Clear interrupt flag
    
}

/****************************************************************
*
* Task scheduler init functions
*
****************************************************************/

IC *IC_init(uint8_t stack_size){

    IC *ic;

    SysTick->CTLR = 0x0; //Disable counter 

    ic = malloc(sizeof(IC));

    ic->tsp  = 0;
    ic->stack_size = stack_size;

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

    ic->status = 1;

    return ic;

}

Task *Task_init(void (*function)(void), uint32_t cooldown){

    Task *t = malloc(sizeof(Task));
    t->cooldown = cooldown;
    t->function = function;

    return t;

}

/****************************************************************
*
* Task functions
*
****************************************************************/

void IC_add_task(IC *ic, void (*function)(void), uint32_t cooldown){

    if(ic->tsp < ic->stack_size){
        ic->task_stack[ic->tsp] = Task_init(function, cooldown);
        qsort(ic->task_stack, ic->tsp + 1, sizeof(Task *), _IC_stack_sort);
        ic->tsp++;
    }

}
void IC_pull_out_task(IC *ic){
    for(uint8_t i = 0; i < ic->tsp; i++)ic->task_stack[i] = ic->task_stack[i+1];
    if(ic->tsp>1){ic->tsp--;}else{IC_clear_task_stack(ic);}
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

/****************************************************************
*
* Time management function
*
****************************************************************/

uint8_t IC_check_timeout(IC *ic){
    if(ic->task_stack[0] == NULL) return 0;
    return (ic->task_stack[0]->cooldown == 0) ? 1 : 0;
}

void IC_decount_task_cooldown(IC *ic){
    for(uint8_t i = 0; i < ic->tsp; i++)ic->task_stack[i]->cooldown--;
}

/****************************************************************
*
* Utility function
*
****************************************************************/

int _IC_stack_sort(const void *a, const void *b){
    const Task *t1 = *(Task * const *)a;
    const Task *t2 = *(Task * const *)b;
    return (int)(t1->cooldown - t2->cooldown);
}

/****************************************************************
*
* Delay function
*
****************************************************************/

void IC_delay_ms(uint32_t delay){
    for(uint64_t i = 0;i < delay*DELAY_MS_TIME;i++){__asm volatile("nop");}
}

void IC_softsleep_ms(uint32_t delay){
    i_see->status = 0;
    for(uint64_t i = 0;i < delay*DELAY_MS_TIME;i++){__asm volatile("nop");}
    i_see->status = 1;
}

/*
                                                                                                    
                                                                                                    
                                                                       .                            
                                                .                          .                        
                .                                                                                   
                                  .                           .                                     
                                          .                                                      .  
                                                  .                     .                           
                            .                                .                                      
                 .                 .        .     .                                                 
                                                          .                             .           
                                                                                                    
                                                                                       .            
                                                                                              .     
                                  .                                                                 
                                                                               .                    
                                                                                                  . 
                                           .                                                        
                                                                                                    
                                     .                                             .                
                                           .                                                        
                   @@@@@@            =@@@@@@            @@@@@@*       .    @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
              .    @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*    .       @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@   .               
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@            =@@@@@@      .     @@@@@@*            @@@@@@    .              
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@        .   =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
                   @@@@@@            =@@@@@@            @@@@@@*            @@@@@@                   
  .                      @@@@@@@@@@@@*                        =@@@@@@@@@@@@                         
                         @@@@@@@@@@@@*                        =@@@@@@@@@@@@                 .       
                         @@@@@@@@@@@@*                        =@@@@@@@@@@@@                         
                         @@@@@@@@@@@@*                        =@@@@@@@@@@@@                         
      .............        .                                             .       .............      
      .............        .                                           .         .............      
      .............     . .                                      .               ..............     
      .............                  =@@@@@@    . @@@@@@      =@@@@@@            .............      
      .............                  =@@@@@@.     @@@@@@      =@@@@@@            .............      
      .............  .               =@@@@@@      @@@@@@      =@@@@@@            .............      
      .............                  =@@@@@@      @@@@@@      =@@@@@@            .............     .
       .                       .            @@@@@@      @@@@@@*                          .          
                                            @@@@@@      @@@@@@*                                     
                    .                       @@@@@@      @@@@@@*     .            .                  
                                                                                                    
                                                                                                    
                                           .           .         . .        .                       
                          .                                                                         
     .                                                                   .                          
                                                                         .      .                   
*/                                                                                                    

