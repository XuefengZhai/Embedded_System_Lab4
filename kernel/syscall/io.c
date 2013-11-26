/*
 * io.c: read system call
 *
 * Authors: Long Tu 	 1 <longt@andrew.cmu.edu>
 *          Jiang Xue    2 <jiangx@andrew.cmu.edu>
 *	    Xuefeng Zhai 3 <xzhai@cmu.edu>
 * Date:    Oct 25th, 2:26AM
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/swi.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>
#include <arm/exception.h>
#define buffersize 0x4000000
#define heapbtm 0xa0000000
#define freetop 0xa3efffff
#define freebtm 0xa3edf000
#define EOT 0x4
#define backspace 127

#define SDRAM_BEGIN 0xa0000000
#define ROM_BEGIN 0x00000000
#define ROM_END 0x00ffffff
#define SDRAM_END 0xa3ffffff


/*
 * get SVC sp
 */
unsigned int getsp(void);

ssize_t read(int fd, void *buf, size_t count)
{
	/*
	 * Define & initialize variables.
	 * Currentsp save SVC sp
	 * Readcount is initialized with -1 
	 * because the first of buf[] start with buf[0].
	 * charbuf in local copy of buf.
	 */
	enable_interrupts();
	
	int unsigned currentsp = getsp();
	char ch;
	ssize_t readcount = -1; 
	char *charbuf=(char *)buf; 
	
	/*
	 * Return -DBADF if not from stdin
	 */
	if (fd != STDIN_FILENO)
	return -EBADF;
			
	/*
	 * Return -EFAULT when:
	 * 1. read more then 64MB buffer size
	 * 2. read can not exceed heap or free 
	 */
	if
	(
		count > buffersize || 
		(
		 !(
                  			
			(
				((char *)charbuf + count) < ((char *)currentsp) && 
				((char *)charbuf) >= ((char *)heapbtm)
			)
		  ||
		 
			(
				((char *)charbuf + count) <= ((char *)freetop) && 
 				((char *)charbuf) >= ((char *)freebtm)
  			)
		 
		  )
		)
	)
		return -EFAULT;
	
	/*
	 * read in the string
	 */		
	while(1)
	{
		ch = (char)getc();
		
		/*
		 * Return immediately when EOT
		 */	
		if (ch == EOT) //EOT
		{
		return (readcount + 1);
		}
		
		/*
		 * Print "\b \b" when backspace
		*/  	
		else if (ch == backspace)
		{
			if(readcount >= 0)
			{
		 		charbuf[readcount] = 0;
		  		readcount -= 1;
		  		putc('\b');
		  		putc(' ');
		  		putc('\b');
			}
		}
		
		/*
		 * Return when a newline or carriage return
		 */
		else if( ch == '\n' || ch == '\r')
		{	
			if ((readcount+1)<(ssize_t)count) 
		  	{
		  		readcount += 1;
		  		charbuf[readcount]='\n';
		  		putc('\n');
				return (readcount+1);
		  	}
		  else
		  	return (readcount+1);
		}
		
		/*
		 * Read the char into the buf
		 */    
		else
		{
			if ((readcount+1)<(ssize_t)count)
			{
		    		readcount += 1;
		    		charbuf[readcount] = ch;
		    		putc(ch);
		  }
		    else
		    		return (readcount + 1);

		}
	}
}

ssize_t write(int fd, const void *buf, size_t count){
	enable_interrupts();
	int i;
	char *Buf = (char *)buf;
	/*
	 *if the first parameter is wrong, return
	 */
	if(fd != STDOUT_FILENO) return -EBADF;
	/*
	 *compute the max address
	 */
	unsigned max_addr = (unsigned)buf + (unsigned)count;

	/*
	 * ROM_BEGIN   ROM_END    RAM_BEGIN    UBOOTSTART   UBOOTEND/RAM_END
	 *   [] ----------[]         []----------------------------[]  
	 *   |<-writable->|          |<--writable------------------>| 
	 */
	if(!(((max_addr<=(unsigned)ROM_END))||
		(((unsigned)buf>=(unsigned)SDRAM_BEGIN)&&(max_addr<=(unsigned)SDRAM_END)))){
		return -EFAULT;
	}


	/*
	 *traverse through the Buffer, stop if there is an '\0'
	 * or i reaches count
	 */
	for(i=0;i<(signed int)count&&Buf[i]!='\0';i++){
		 putc(Buf[i]);
	}

	return count;

}

