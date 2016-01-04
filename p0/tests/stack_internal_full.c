
void add1(int a, int b){
		a = a + 1;
		b = b + 1;
}

void empty1(){}
void empty2(){}
void empty3(){}

int max(int a, int b, int c){
		if(a > b){
				if(a > c){
						return a;
				}else{
						return c;
				}
		} else{
				if(b > c){
						return b;
				}else{
						return c;
				}
		}
}

int main(){
		empty1();
		empty2();
		empty3();
		add1(2, 3);
		int c = max(1, 2, 3);
		c++;
		return 0;
}




