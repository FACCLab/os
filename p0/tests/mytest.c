#include "traceback.h"
#include <stdio.h>

typedef struct ARG{
	int a;
	int b;
}ARG;
void first(unsigned int a, long b, ARG* c){
	traceback(stdout);
}
void second(int a, float b, double c, char d, char *e, char**f, void *g, int h){
	ARG test1 = {.a = 2, .b = 3};
	first(23, 23, &test1);
}
void third(){
	int a = 8;
	float b = 4.5;
	double c = 3.3;
	char d = 'd';
	
	char *e = "argE0123456789012345678901";
	const char *f[] = {"1234567890123456789012345678", "f2", "f3", "f4"};
	second(a, b, c, d, e, (char **)f, (void *)&a, 2);
}
void fourth(int a, int b){
	a = a+b;
	third();
}
void mess(){
	fourth(2, 3);
}
int main(){
	mess();
	return 0;
}
