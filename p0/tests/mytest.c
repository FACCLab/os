#include "traceback.h"
#include <stdio.h>

void first(){
	traceback(stdout);
}
void second(int a, float b, double c, char d, char *e, char**f, void *g){
	first();
}
void third(){
	int a = 8;
	float b = 4.5;
	double c = 3.3;
	char d = 'd';
	
	char *e = "argE0123456789012345678901";
	const char *f[] = {"1234567890123456789012345678", "f2", "f3", "f4"};
	second(a, b, c, d, e, (char **)f, (void *)&a);
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
