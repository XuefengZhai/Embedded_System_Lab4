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
        printf("There are no available mutex!\n");
        enable_interrupts();
        return -ENOMEM;
    }
	
}

int mutex_lock(int mutex  __attribute__((unused)))
{
	tcb_t * cur_tcb;
    mutex_t mutex_tmp;
    
    disable_interrupts();
    
    
    if (mutex < 0 || mutex >= OS_NUM_MUTEX)
    {
        enable_interrupts();
        printf("mutes number is invalid");
        return -EINVAL;
    }
    
    
    
}

int mutex_unlock(int mutex  __attribute__((unused)))
{
	return 1; // fix this to return the correct value
}

