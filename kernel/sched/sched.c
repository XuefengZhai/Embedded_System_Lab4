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

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

void sched_init(task_t* main_task  __attribute__((unused)))
{
	main_task->lambda = (task_fun_t)idle;
	main_task->data = NULL;
	main_task->stack_pos = system_tcb[IDLE_PRIO].kstack_high;
	main_task->C = 0;
	main_task->T = 0;
	init_task(main_task, &system_tcb[IDLE_PRIO], IDLE_PRIO);
	runqueue_add(&system_tcb[IDLE_PRIO],IDLE_PRIO);
}

void init_task(task_t *task, tcb_t *tcb,uint8_t prio)
{
        sched_context_t *context = &(tcb->context);
        tcb->native_prio = prio;
        tcb->cur_prio = prio;
        //set up context
        context->r4 = (uint32_t)task->lambda;
        context->r5 = (uint32_t)task->data;
        context->r6 = (uint32_t)task->stack_pos;
        context->sp = (void *)(tcb->kstack_high);
	context->lr = launch_task;

	context->r7 = 0x0;
	context->r9 = 0x0;
	context->r10 = 0x0;
	context->r11 = 0x0;
	tcb->holds_lock = 0;
	tcp->sleep_queue = NULL;
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
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
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	task_t *task_list = *tasks;
	unsigned int i;
	for(i = 0; i < num_tasks; i++){
		init_task(&a_tasks[i], &system_tcb[i+1], i+1);
	}
}

