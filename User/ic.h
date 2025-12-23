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

#include <stdio.h>
#include <stdlib.h>
#include "ch32v00x.h"

#ifndef __IC_H__
#define __IC_H__

#define task_stack_pointer tsp

#define DELAY_MS_TIME SystemCoreClock / 8 / 1000 //The number of clock cycle for 1ms

/****************************************************************
*
* Task scheduler structures and functions
*
****************************************************************/

typedef struct Task{
    void (*function)();
    uint32_t cooldown;
}Task;

Task *Task_init(void (*function)(), uint32_t cooldown);

typedef struct task_manager{
    uint64_t timestamp;
    Task **task_stack;
    uint8_t stack_size, task_stack_pointer;
    uint8_t status;
}IC;//IC standed for internal_counter

/******************************Task Scheduler init functions******************************/

IC *IC_init(uint8_t stack_size);

/******************************Task functions******************************/

void IC_add_task(IC *ic, void (*function)(void), uint32_t cooldown);
void IC_pull_out_task(IC *ic);
void IC_clear_task_stack(IC *ic);

/******************************Time managment functions******************************/

uint8_t IC_check_timeout(IC *ic);
void IC_decount_task_cooldown(IC *ic);

/******************************Task Scheduler delay functions******************************/

void IC_delay_ms(uint32_t delay);
void IC_softsleep_ms(uint32_t delay);

/******************************Utility functions******************************/

int _IC_stack_sort(const void *a, const void *b);

#endif

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

