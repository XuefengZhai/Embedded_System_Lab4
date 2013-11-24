/** @file kernel.h
 *
 * @brief Main kernel -- primary interface.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-17
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <types.h>

extern uint32_t global_data;
int valid_addr(const void* start, size_t count, uintptr_t base, uintptr_t bound);
void irq_wrapper();
void s_handler();
int switchUser(unsigned int stackAddr);
int wire_in (unsigned int* VectorTable, unsigned int MyHandler);


#endif /* KERNEL_H */
