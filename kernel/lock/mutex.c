/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Xuefeng Zhai xzhai@cmu.edu
 *
 *
 * @date
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>

#define DEBUG_MUTEX //temp
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];

void mutex_init()
{
    //Initiate everything in gtMutex
    int i = 0;
    for (i = 0; i < OS_NUM_MUTEX; i++)
    {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].bLock = FALSE;
        gtMutex[i].pSleep_queue = NULL;
    }
    
}

int mutex_create(void)
{
    int j;
    disable_interrupts();
    
    for (j = 0; j < OS_NUM_MUTEX; j++)
    {
        if (gtMutex[j].bAvailable)
            break;
    }
    
    if ( j!= OS_NUM_MUTEX)
    {
        gtMutex[j].bAvailable = FALSE;
        enable_interrupts();
        return j;
    }
    else
    {
        enable_interrupts(); //No available mutex
        return -ENOMEM;
    }
	
}

int mutex_lock(int mutex)
{
	tcb_t * cur_tcbget_cur_tab();
    mutex_t *mutex_tmp=&gtMutex[mutex];
    
    disable_interrupts();
    
    
    if (mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    
    else if(mutex_tmp->bAvailable)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    else if(mutex_tmp->pHolding_Tcb == cur_tcb)
    {
        enable_interrupts();
        return -EDEADLOCK;
    }
    
    else //add to sleep queue
    {
        if (mutex_tmp->block)
        {
            tcb_t *sleep_tcb;
            tcb_t *tmp_tcb = NULL;
        
            if (mutex_tmp->pSleep_queue == NULL)
            {
                mutex_tmp->pSleep_queue = cur_tcb;
                cur_tcb->sleep_queue = NULL;
            }
            else
            {
                sleep_tcb = mutex_tmp->pSleep_queue;
                while(sleep_tcb != NULL)
                {
                tmp_tcb = sleep_tcb;
                sleep_tcb = sleep_tcb->sleep_queue;
                }
            tmp_tcb->sleep_queue = cur_tcb;
            cur_tcb->sleep_queue = NULL;
            
            }
        
            dispatch_sleep();
        }
    }
    
    mutex_tmp->block = TRUE;
    mutex_tmp->pHolding_Tcb = cur_tcb;
    enable_interrupts();
    
    return 0;
}

int mutex_unlock(int mutex)
{
    tcb_t *cur_tcb = get_cur_tcb();
    tcb_t *next_tcb;
    mutex_t *mutex_tmp = &gtMutex[mutex];

    disable_interrupts();
    
    if (mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    
    else if(mutex_tmp->bAvailable)
    {
        enable_interrupts();
        return -EINVAL;
    }
    
    
    else if (cur_tcb != mutex_tmp.pHolding_Tcb)
    {
        enable_interrupts();
        return -EPERM;
    }
    
    else
    {
        
        mut->bLock = FALSE;
        mut->pHolding_Tcb = NULL;
        
        if(mut->pSleep_queue != NULL)
        {
            next_tcb = mut->pSleep_queue;
            mut->pSleep_queue = next_tcb->sleep_queue;
            next_tcb->sleep_queue = NULL;
            runqueue_add(next_tcb, next_tcb->cur_prio);
        }
        
        enable_interrupts();
        return 0;

    }
    
    
}

