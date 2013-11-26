/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle __attribute__((unused)))
{
    
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	tcb_t *tmp = cur_tcb;
	if(get_cur_prio() <= highest_prio()){
		return;
	}
	else{
	//	printf("dispatch_save start!!!");
		tcb_t *rm = runqueue_remove(highest_prio());
		runqueue_add(cur_tcb,cur_tcb->native_prio);
		cur_tcb = rm; 
		/*
		printf("old context r4: %x \r\n",(int)((tmp->context).r4));
		printf("old context r5: %x \r\n",(int)((tmp->context).r5));
		printf("old context r6: %x \r\n",(int)((tmp->context).r6));
		printf("old context r7: %x \r\n",(int)((tmp->context).r7));
		printf("old context r8: %x \r\n",(int)((tmp->context).r8));
		printf("old context r9: %x \r\n",(int)((tmp->context).r9));
		printf("old context r10: %x \r\n",(int)((tmp->context).r10));
		printf("old context r11: %x \r\n",(int)((tmp->context).r11));
		printf("old context sp: %x \r\n",(int)((tmp->context).sp));
		printf("old context lr: %x \r\n",(int)((tmp->context).lr));
		printf("new context r4: %x \r\n",(int)((rm->context).r4));
		printf("new context r5: %x \r\n",(int)((rm->context).r5));
		printf("new context r6: %x \r\n",(int)((rm->context).r6));
		printf("new context r7: %x \r\n",(int)((rm->context).r7));
		printf("new context r8: %x \r\n",(int)((rm->context).r8));

		printf("new context r9: %x \r\n",(int)((rm->context).r9));
		printf("new context r10: %x \r\n",(int)((rm->context).r10));
		printf("new context r11: %x \r\n",(int)((rm->context).r11));
		printf("new context sp: %x \r\n",(int)((rm->context).sp));
		printf("new context lr: %x \r\n",(int)((rm->context).lr));
		*/
		ctx_switch_full(&(rm->context),&(tmp->context));	
	}

}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
//	printf("dispatch_nosave start!!!");
	tcb_t* rm = runqueue_remove(highest_prio());
	cur_tcb = rm;
	ctx_switch_half(&(rm->context));
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
//	printf("dispatch_sleep start!!!");
	tcb_t *tmp = cur_tcb;
	tcb_t *rm = runqueue_remove(highest_prio());
	cur_tcb = rm;
	ctx_switch_full(&(rm->context),&(tmp->context));
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
