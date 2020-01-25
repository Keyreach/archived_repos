#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;


struct intnode {
	int s;
	intnode *next;
};

class fakestack {
	int sp = 0;
	intnode *top = NULL;
	public:
	void push(int s){
		intnode *nsn = new intnode;
		nsn->s = s;
		nsn->next = top;
		top = nsn;
		++sp;
	}
	int pop(){
		int s = top->s;
		intnode *nsn = top;
		top = top->next;
		delete nsn;
		--sp;
		return s;
	}
	int peek(){
        if(sp==0){
            return 0;
        }
		int s = top->s;
		return s;
	}
	string list(){
		intnode *q = top;
		string s;
		while(q){
			s += to_string(q->s) + " ";
			q = q->next;
		}
		reverse(s.begin(), s.end());
		return s;
	}
	int operator[](int ix){
		intnode *q = top;
		for(int i=0;i<(sp-ix-1);i++){
			q = q-> next;
		}
		return q->s;
	}
	int size(){
		return sp;
	}
};

fakestack f, s, t;

void hanoi(int num, fakestack * src, fakestack * dest, fakestack * buf){
	if(num!=0){
		hanoi(num - 1, src, buf, dest);
		//printf("Move %d from %d to %d\n", num, src, dest);
		dest->push(src->pop());
		system("clear");
		printf("1) %s\n", f.list().c_str());
		printf("2) %s\n", s.list().c_str());
		printf("3) %s\n\n", t.list().c_str());
		getchar();
		hanoi(num - 1, buf, dest, src);
	}
}

int main(){
	int q = 5;
	for(int i=0; i<q; i++){
		f.push(q-i);
	}
	hanoi(q,&f,&t,&s);
	return 0;
}
