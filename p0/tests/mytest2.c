#include <stdio.h>

void first(){
	int a = 3;
	a++;
	//traceback(stdout);
}
int second(int a, int b){
	first();
	return a+b;
}
void third(char* x){
	*x = 'a';
	int a = second(2, 3);
	a++;
}

int main(){
	char x = 'x';
	third(&x);
	return 0;
}
