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

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>



int check_validation(task_t** tasklist, size_t num_of_tasks){
    /* check number of tasks */
    if((num_of_tasks > OS_MAX_TASKS - 2) || (num_of_tasks <= 0))
        return EINVAL;
    
    int i = 0;
    for(i = 0; i < num_of_tasks; i++){
        if(valid_addr((unsigned int)(tasklist[i]->lambda), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("task function out of valid address \r\n");
            return EFAULT;
        }
        
        //check if the stack point are duplicated
        
        if(valid_addr((unsigned int)(tasklist[i]->stack_pos), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("task stack out of valid address \r\n");
            return EFAULT;
        }
        if(valid_addr((unsigned int)(tasklist[i]->data), 0, USR_START_ADDR, USR_END_ADDR) == 0){
            printf("data out of valid address \r\n");
            return EFAULT;
        }
        
        /* sorted when returned from assign_schedule */
        if(assign_schedule(tasklist, num_of_tasks) == 0){
            printf("tasks not schedulable \r\n");
            return ESCHED;
        }
        
        return 1;
            
    }
    
    
    
}

int task_create(task_t* tasks, size_t num_tasks)
{
    disable_interrupts();//have to disable irq until the first task being launched
    
    /* initialize the run queue, the mutex and the device */
    runqueue_init();
    mutex_init();
    dev_init();
    
    /*check the validation of stack_pos, data, function, num_tasks and schedubility
     * and sort the task
     */
    
    task_t** array_of_pointers = &tasks;
    
    int check = check_validation(array_of_pointers, num_tasks);
    if(check != 1){
        return check;
    }
    
 
    
    allocate_tasks(array_of_pointers, num_tasks);
    //sched_init();
    dispatch_nosave();
    
    return 1;/* never return */
  }

int event_wait(unsigned int dev)
{
    if(dev >= NUM_DEVICES){
        printf("device number invalid \r\n");
        return EINVAL;
    }
    dev_wait(dev);

  return 0; 
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
