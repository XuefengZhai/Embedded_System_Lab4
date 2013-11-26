/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>
#include <exports.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

#define MAX_INT 0xFFFFFFFF

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
    int i;
    
    for(i = 0; i < NUM_DEVICES; i++)
    {
        devices[i].next_match = dev_freq[i];
        devices[i].sleep_queue = NULL;
    }

}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev)
{
   // printf("dev_no %d \r\n",(int)dev);
    disable_interrupts();
    tcb_t* cur_tcb = get_cur_tcb();
    cur_tcb->sleep_queue = devices[dev].sleep_queue;
    devices[dev].sleep_queue = cur_tcb;
    dispatch_sleep();
    enable_interrupts();

}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis)
{
	int i = 0;
	unsigned long max = MAX_INT;
        tcb_t *temp_tcb;
        while(i < NUM_DEVICES){
                 if(devices[i].next_match == millis) {
			//printf("device %d, n_match %d, cur_time %d ms \r\n",i,(int)(devices[i].next_match),(int)millis);
			//printf("sleepQ: %d \r\n",(int)(devices[i].sleep_queue));
                        while(devices[i].sleep_queue != NULL) {
                                temp_tcb = devices[i].sleep_queue;
                                //printf("current_time: %d ms\r\n",(int)millis);
                                runqueue_add(temp_tcb, temp_tcb->cur_prio);
                                devices[i].sleep_queue = temp_tcb->sleep_queue;
                                temp_tcb->sleep_queue = NULL;
                        }

                        /*
                         * check overflow
                         */
                        if(max - devices[i].next_match < dev_freq[i]){
                                printf("overflow in next match: %d \n", i);
                        }
                        devices[i].next_match += dev_freq[i];
                }
                i++;
        }
}

