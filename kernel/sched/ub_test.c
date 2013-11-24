/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks, size_t num_tasks)
{
    
    int i = 0;
    int j = 0;
    for(i = 0; i < num_tasks; i++){
        if(tasks[i]->C > tasks[i]->T)){
            return 0;
        }
    }
    task_t* tmp = NULL;
    
    //sorted here!!! can we just sort the array of pointers?
    
    for(i = num_tasks; i > 0; i--){
        for(j = 0; j < i-j-1; j++){
            if((tasks[j]->T)>(tasks[j+1]->T)){
                tmp = tasks[j];
                tasks[j] = tasks[j+1];
                tasks[j+1] = tmp;
            }
            
        }
    }
    
    

	return 1; // fix this; dummy return to prevent compiler warnings	
}
	


