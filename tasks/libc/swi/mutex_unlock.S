/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */
#include <bits/swi.h>

#include <asm.h>

	.file "mutex_unlock.S"

FUNC(mutex_unlock)
        stmfd        sp!, {r1}        @reserve r1 to sp
        swi        MUTEX_UNLOCK        @call READ_SWI
        cmp        r0, #0
        bpl        .finish                @if r0 >0 to finish
        
        ldr        r1, =errno        @load errno address to r1
        rsblt        r0, r0, #0        @absolute value of r0
        str        r0, [r1]        @store r0 in errno
        mov        r0, #-1                @move -1 in r0

.finish:
        ldmfd        sp!, {r1}        @restore r1
        mov        pc, lr                @return
