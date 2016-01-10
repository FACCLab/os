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

int funcs_find(int addr){
	int index = 0;
	while(index <= FUNCTS_MAX_NUM){
		if(((int)functions[index].addr) == addr){
			return index;
		}
		index++;
	}
	return -1;
}

void traceback(FILE *fp)
{
	/* the following just makes a sample access to "functions" array. 
	 * note if "functions" is not referenced somewhere outside the 
	 * file that it's declared in, symtabgen won't be able to find
	 * the symbol. So be sure to always do something with functions */

	/* remove this line once you've got real code here */
	//int count = 0;
	//for(count = 0; count < FUNCTS_MAX_NUM; count++){
	//	printf("functions[%d]: %s at %p\n", count, 
	//			functions[count].name, functions[count].addr);
	//}
	ASSERT(fp != NULL);
	int ebp;
	int eip;
	int func_addr;
	int func_index;
	ebp = getFirstStack();
	while(ebp){
		// Read Eip
		eip = *(int *)(ebp+4);
		if(*(int *)(eip-4) == 0x602454ff){
			// Found <main> here.
			func_addr =*(int *) (ebp + 8 + 0x60);
			func_index = funcs_find(func_addr);
#ifdef DEBUG
			fprintf(fp, "Found function %s(%p) at [%d]\n", 
					functions[func_index].name, (void *)func_addr, func_index);
			fflush(fp);
#endif
			return;
	
		}else{
			func_addr = eip + (*(int *)(eip - 4));
		}
		if((func_index = funcs_find(func_addr)) >= 0){
			// Found function information in functions array.
#ifdef DEBUG
			fprintf(fp, "Found function %s(%p) at [%d]\n", 
					functions[func_index].name, (void *)func_addr, func_index);
			fflush(fp);
#endif
		}else{
			// Our functions array didnot hold infor about this function.
			fprintf(fp, "Unknown %p\n", (void *)func_addr);	
		}
		ebp = * (int *)ebp;
	}
}


