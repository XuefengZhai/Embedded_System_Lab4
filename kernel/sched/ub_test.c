/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0
#define DEBUG
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
    task_t * t_list = *tasks;   
    unsigned int k = 0; 
    for(k = 0; k < num_tasks; k++){
        if((t_list+k)->C > (t_list+k)->T){
            return 0;
        }
    }

    int i = 0;
    int j = 0;
    //sorted here!!! can we just sort the array of pointers?
    //quickSort(t_list,0,(int)num_tasks - 1);
    
    for(i = (int)num_tasks; i > 0; i--){
        for(j = 0; j < i-1; j++){
            if(((t_list+j)->T)>((t_list+j+1)->T)){
            t_swap(t_list+j, t_list+j+1);
             }	
         }
    }
    



    return 1; // fix this; dummy return to prevent compiler warnings	
}


/**
 * 
 *
 * T = O(n)
 *
 */
 
 /*
int pivotLoc(task_t *arr, int bt, int ed)
{
	unsigned long stand;
	task_t *pivot = arr;
	
	stand = arr[bt]->T;

	while (bt < ed) {
		while (bt < ed && arr[ed]->T >= stand)	ed --;
		if (bt < ed){
			t_swap(arr[bt],arr[ed] );
			bt++;
		} 

		while (bt < ed && arr[bt]->T <= stand)	bt ++;
		if (bt < ed){
			t_swap(arr[bt],arr[ed] );
			ed--;
		}
	}

	arr[bt] = pivot;

	return bt;
}

*/
/**
 * quick sort
 *
 * T = O(nlogn)
 *
 */
 
 /*
void quickSort(task_t *arr, int bt, int ed)
{
	int pivot;

	if (bt < ed) {
		pivot = pivotLoc(arr, bt, ed);
		quickSort(arr, bt, pivot - 1);
		quickSort(arr, pivot + 1, ed);
	}
}
*/
void t_swap(task_t* t1, task_t* t2){
   
   task_t tmp;
   tmp.lambda = t1->lambda;
   tmp.data = t1 ->data;
   tmp.stack_pos = t1->stack_pos;
   tmp.C = t1->C;
   tmp.T = t1->T;
   t1->lambda = t2->lambda;
   t1->data = t2->data;
   t1->stack_pos = t2->stack_pos;
   t1->C = t2->C;
   t1->T = t2->T;
   t2->lambda = tmp.lambda;
   t2->data = tmp.data;
   t2->stack_pos = tmp.stack_pos;
   t2->C = tmp.C;
   t2->T = tmp.T;
}

