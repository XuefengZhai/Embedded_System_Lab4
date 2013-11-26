/** @file crt0.S
 *
 * @brief Execution startup routine
 * @author Johnny Xue <jiangx@andrew.cmu.edu>
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:51:29 -0400
 */

#include <asm.h>

	.file "crt0.S"

FUNC(_start)
	@ needs code.  Put your names at the top.
    ldr	r0, [sp]	@load argc in r0
	add	r1, sp, #4	@load argv address in r1
	bl	main		@call main
loop_crt:
    b   loop_crt
