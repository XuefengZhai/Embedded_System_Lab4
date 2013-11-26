/** @file sched.c:
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"
#include <exports.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */
static void idle(void);

void sched_init(task_t* main_task __attribute__((unused)))
{
}

void init_task(task_t *task, tcb_t *tcb,uint8_t prio)
{
    //printf("C_init_task: %d \r\n", (int)task->C);
    //printf("T_init_task: %d \r\n", (int)task->T);
    //printf("priority: %d \r\n", (int)prio);
    sched_context_t *context = &(tcb->context);
    tcb->native_prio = prio;
    tcb->cur_prio = prio;
    //printf("priority_dupl_nat: %d \r\n",(int)(tcb->native_prio));
    //printf("priority_dupl_cur: %d \r\n",(int)(tcb->cur_prio));
    //set up context
    context->r4 = (uint32_t)task->lambda;
    context->r5 = (uint32_t)task->data;
    context->r6 = (uint32_t)task->stack_pos;
    //printf("usr_st: %x \r\n", (unsigned int)(context->r6));
    context->sp = (void *)(tcb->kstack_high);
    if(prio != IDLE_PRIO)
    	context->lr = launch_task;
    else
	context->lr = &(idle);

    context->r7 = 0x0;
    context->r9 = 0x0;
    context->r10 = 0x0;
    context->r11 = 0x0;
    tcb->holds_lock = 0;
    tcb->sleep_queue = NULL;
    
    runqueue_add(&system_tcb[prio],prio);
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void idle(void)
{
    enable_interrupts();
    while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks, size_t num_tasks)
{
    task_t* t_list = *tasks;
    /* create an idle task*/

    task_t idle_task;
    
    idle_task.lambda = (task_fun_t)idle;
    idle_task.data = NULL;
    /* give svc stack to idle using as usr stack*/
    idle_task.stack_pos = system_tcb[IDLE_PRIO].kstack_high;
    idle_task.C = 0;
    idle_task.T = 0;
    
    /*put the idle task to tcb and make it runnable*/
    init_task(&idle_task, &system_tcb[IDLE_PRIO], IDLE_PRIO);
    
    /* make user tasks into tcb and make them runnable*/
    unsigned int i = 0;
    while(i < num_tasks){
    	init_task(t_list+i, &system_tcb[i+1], i+1);
    	i++;
    }
    
    
}

