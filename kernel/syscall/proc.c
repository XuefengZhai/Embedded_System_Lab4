/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <lock.h>
#include <../sched/sched_i.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int mutex_init_flag = 1;


int check_validation(task_t** tasklist, size_t num_of_tasks){
    /* check number of tasks */
    task_t * t_list = *tasklist;
    if((num_of_tasks > OS_MAX_TASKS - 2) || (num_of_tasks == 0)){
        return -EINVAL;
    }

    unsigned int i = 0;
    for(i = 0; i < num_of_tasks; i++){
        
        //check if the task located in USR space.
        if(valid_addr((void *)(t_list+i), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("task created out of user space \r\n");
            return -EFAULT;
        }
        
        //check if the task's function located in USR space.
        if(valid_addr((void *)((t_list+i)->lambda), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("task function out of valid address \r\n");
            return -EFAULT;
        }
        
        //check if the task's stack located in USR space.
        if(valid_addr(((t_list+i)->stack_pos), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("task stack out of valid address \r\n");
            return -EFAULT;
        }
        /*
        if(valid_addr((tasklist[i]->data), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("data out of valid address \r\n");
            return -EFAULT;
        }
         */
        
        /* sorted when returned from assign_schedule */
        if(assign_schedule(tasklist, num_of_tasks) == 0){
            printf("tasks not schedulable \r\n");
            return -ESCHED;
        }
	
	
        
        
            
    }
    
    //check if different tasks using the same stack.
    unsigned int j = 0;
    for(i = 0; i < num_of_tasks-1; i++){
	for (j = i; j < num_of_tasks-1; j++){
		if(((t_list+i)->stack_pos) == ((t_list+j+1)->stack_pos)){
			printf("stack position duplicated \r\n");
			return -EFAULT;	
		}
	}
    }
    return 1;

    
    
}

int task_create(task_t* tasks, size_t num_tasks)
{
    
    /* initialize the run queue, the mutex and the device */
    runqueue_init();
    
    dev_init();
    
    /*check the validation of stack_pos, data, function, num_tasks and schedubility
     * and sort the task
     */


    
    int check = check_validation(&tasks, num_tasks);
    if(check != 1){
        return check;
    }
    
 
    
    allocate_tasks(&tasks, num_tasks);
    //sched_init();
    dispatch_nosave();//dispatch the highest priority task
    
    return 1;/* never return */
  }

int event_wait(unsigned int dev)
{

    enable_interrupts();
    if(dev >= NUM_DEVICES){
        printf("device number invalid \r\n");
        return -EINVAL;
    }
    
    if(dev_wait(dev) != 1){
    	printf("current task holding lock(s)");
	return -EHOLDSLOCK;
    }

  return 0; 
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
