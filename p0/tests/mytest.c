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
	
	char *e = "argE\1";
	const char *f[] = {"f1", "f2", "f3"};
	second(a, b, c, d, e, (char **)f, (void *)&a);
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
