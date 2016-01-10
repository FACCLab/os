#include "traceback.h"
#include <stdio.h>

void first(){
	traceback(stdout);
}
int main(){
	first();
	return 0;
}
