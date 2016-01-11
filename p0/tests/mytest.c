#include "traceback.h"
#include <stdio.h>

void first(){
	traceback(stdout);
}
void second(){
	first();
}
void third(){
	second();
}
void fourth(){
	third();
}
void mess(){
	fourth();
}
int main(){
	mess();
	return 0;
}
