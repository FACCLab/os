/** @file traceback.c
 *  @brief The traceback function
 *
 *  This file contains the traceback function for the traceback library
 *      
 *  @time 2016-01-04 Mon 07:37 PM
 *  @author Harry Q. Bovik (hqbovik)
 *  @bug Unimplemented
 */

#include <stdio.h>

#include "contracts.h"
#include "traceback_internal.h"
extern void *getFirstStack();

int funcs_find(void *addr){
	ASSERT(addr != NULL);
	int index = 0;
	while(index <= FUNCTS_MAX_NUM){
		if(functions[index].addr == addr){
			return index;
		}
		index++;
	}
	return 0;
}

void traceback(FILE *fp)
{
	/* the following just makes a sample access to "functions" array. 
	 * note if "functions" is not referenced somewhere outside the 
	 * file that it's declared in, symtabgen won't be able to find
	 * the symbol. So be sure to always do something with functions */

	/* remove this line once you've got real code here */
	ASSERT(fp != NULL);
	void *ebp;
	int eip;
	void *func_addr;
	int func_index;
	ebp = getFirstStack();
	while(ebp){
		// Read Eip
		eip = *(int *)(ebp+1);
		func_addr = (void *)(eip + (*(int *)(eip - 4)));
		if((func_index = funcs_find((void *)func_addr)) >= 0){
			// Found function information in functions array.
#ifdef DEBUG
			fprintf(stdout, "Found function %p at [%d]", 
					func_addr, func_index);
#endif
		}else{
			// Our functions array didnot hold infor about this function.
			fprintf(fp, "Unknown %p", func_addr);	
		}
		printf("ebp: %p\n", ebp);
		break;
		//ebp =(void *) *ebp;
	}
}


