/**
 * @file mutex.c
 *
 * @brief Implements mutex.
 *
 * @author Xuefeng Zhai xzhai@cmu.edu
 *
 * @date    11/22/2013
 */


#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>

//#define DEBUG_MUTEX

#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];


/*
 * Initiate the Mutex in gtMutex
 */
void mutex_init()
{
    
    int i = 0;
    for (i = 0; i < OS_NUM_MUTEX; i++)
    {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].bLock = FALSE;
        gtMutex[i].pSleep_queue = NULL;
    }
    
}

/*
 * Create Mutex
 */
int mutex_create(void)
{
    int j;
    disable_interrupts();
    
    /*
     *Find available mutex
     */
    for (j = 0; j < OS_NUM_MUTEX; j++)
    {
        if (gtMutex[j].bAvailable)
            break;
    }
    
    /*
     * Mark as umavailable
     */
    if ( j!= OS_NUM_MUTEX)
    {
        gtMutex[j].bAvailable = FALSE;
        enable_interrupts();
        return j;
    }
    
    /*
     * If no available Mutex
     */

    else
    {
        enable_interrupts();
        return -ENOMEM;
    }
	
}


/*
 *Implement mutex lock
 */
int mutex_lock(int mutex)
{
	tcb_t * cur_tcb = get_cur_tcb();
    mutex_t *mutex_tmp = &gtMutex[mutex];
    
    disable_interrupts();
    
    
    /*
     * If mutex number is not valid
     */
    if (mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    /*
     * If mutex is not created
     */
    else if(mutex_tmp->bAvailable)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    /*
     * If the task is already using the mutex
     */
    else if(mutex_tmp->pHolding_Tcb == cur_tcb)
    {
        enable_interrupts();
        return -EDEADLOCK;
    }
    
    /*
     * Lock the mutex
     * Add task to sleep queue if mutex is using
     */
    else
    {
        if (mutex_tmp->bLock)
        {
            tcb_t *sleep_tcb;
            
            /*
             *If the sleep_queue is empty
             */
            if (mutex_tmp->pSleep_queue == NULL)
            {
                mutex_tmp->pSleep_queue = cur_tcb;
                cur_tcb->sleep_queue = NULL;
            }
            
            /*
             *If the sleep_queue is not empty
             */
            else
            {
                sleep_tcb = mutex_tmp->pSleep_queue;
                while(sleep_tcb->sleep_queue != NULL)
                {
                sleep_tcb = sleep_tcb->sleep_queue;
                }
            sleep_tcb->sleep_queue = cur_tcb;
            cur_tcb->sleep_queue = NULL;
            
            }
        
            dispatch_sleep();
        }
    }
    
    mutex_tmp->bLock = TRUE;
    mutex_tmp->pHolding_Tcb = cur_tcb;
    cur_tcb->holds_lock += 1;
    cur_tcb->cur_prio = 0; // Priority Inheritance by HLP
    enable_interrupts();
    
    return 0;
}

/*
 *Implement mutex unlock
 */

int mutex_unlock(int mutex)
{
    tcb_t *cur_tcb = get_cur_tcb();
    mutex_t *mutex_tmp = &gtMutex[mutex];

    disable_interrupts();
    
    /*
     * If mutex number is not valid
     */
    if (mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    
    /*
     * If mutex is not created
     */
    else if(mutex_tmp->bAvailable)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    /*
     * If the task is already using the mutex
     */
    else if (cur_tcb != mutex_tmp->pHolding_Tcb)
    {
        enable_interrupts();
        return -EPERM;
    }
    
    /*
     * Unlock the mutex
     */
    else
    {
        
        mutex_tmp->bLock = FALSE;
        mutex_tmp->pHolding_Tcb = NULL;
        
        tcb_t *next_tcb;
        
        /*
         *If the sleep_queue is not empty
         *run queue add for the next tcb
         */
        if(mutex_tmp->pSleep_queue != NULL)
        {
            next_tcb  = mutex_tmp->pSleep_queue;
            mutex_tmp->pSleep_queue = next_tcb->sleep_queue;
            next_tcb->sleep_queue = NULL;
            runqueue_add(next_tcb, next_tcb->cur_prio);
        }
        
	cur_tcb->cur_prio = cur_tcb->native_prio;// Priority Inheritance by HLP
	cur_tcb->holds_lock -= 1;
        enable_interrupts();

        return 0;

    }
    
    
}

