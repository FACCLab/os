
void empty(int a, int b){
		a = a+b;
}

int main(){
	empty(2, 3);
	int c = 2;
	int d = 3;
	c = d;
	d = c;
	return 0;
}
