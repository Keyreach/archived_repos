#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct stringnode {
	string s;
	stringnode *next;
};
struct floatnode {
	float s;
	floatnode *next;
};
/*
	String stack
*/
class fakestack {
	/* String stack size */
	int sp = 0;
	/* Float stack size */
	int fp = 0;
	/* String stack pointer */
	stringnode *top = NULL;
	/* Float stack pointer */
	floatnode *ftop = NULL;
	public:
	/* Push string to stack */
	void push(string s){
		stringnode *nsn = new stringnode;
		nsn->s = s;
		nsn->next = top;
		top = nsn;
		++sp;
	}
	/* Pop string out of stack */
	string pop(){
		string s = top->s;
		stringnode *nsn = top;
		top = top->next;
		delete nsn;
		--sp;
		return s;
	}
	/* Peek top string */
	string peek(){
        if(sp==0){
            return "0";
        }
		string s = top->s;
		return s;
	}
	/* List string stack */
	string list(){
		stringnode *q = top;
		string s;
		while(q){
			s += q->s + ", ";
			q = q->next;
		}
		return s;
	}
	/* Get particular string from stack */
	string operator[](int ix){
		stringnode *q = top;
		for(int i=0;i<(sp-ix-1);i++){
			q = q-> next;
		}
		return q->s;
	}
	/* Get stack size */
	int size(){
		return sp;
	}
	/* Push float to stack */
	void fpush(float s){
		floatnode *nsn = new floatnode;
		nsn->s = s;
		nsn->next = ftop;
		ftop = nsn;
		++fp;
	}
	/* Pop float out of stack */
	float fpop(){
		float s = ftop->s;
		floatnode *nsn = ftop;
		ftop = ftop->next;
		delete nsn;
		--fp;
		return s;
	}
	/* Show top float */
	float fpeek(){
		float s = ftop->s;
		return s;
	}
	/* Show particular float in stack */
	float fget(int ix){
		floatnode *q = ftop;
		for(int i=0;i<(sp-ix-1);i++){
			q = q-> next;
		}
		return q->s;
	}
	/* Show float stack size */
	int fsize(){
		return fp;
	}
};

int priority(char in){
    return
        in == '+' || in == '-' ? 1 :
        in == '*' || in == '/' || in == '%' ? 2 :
        in == '^' || in == '?' ? 3 :
        -1;
}

int main(){
	float a, b;
	fakestack str, yard, stack1;
	string raw;
	string tmp = "";
	cout << "Enter valid expression\n-> ";
	cin >> raw;
	/*
        Parsing raw string
	*/
    for(int i = 0; i < raw.size(); i++){
        if(isalnum(raw[i])){
            while(i < raw.size() && isalnum(raw[i])){
                tmp += raw[i];
                ++i;
            }
            str.push(tmp);
            --i;
            tmp = "";
        } else if(raw[i]=='(') {
            yard.push(string(1,raw[i]));
        } else if(raw[i]==')') {
            while(yard.peek()!="("){
                str.push(yard.pop());
            }
            yard.pop();
        } else {
            while(priority(raw[i])<=priority(yard.peek()[0])){
                str.push(yard.pop());
            }
            yard.push(string(1,raw[i]));
        }
    }
    while(yard.size()>0){
        str.push(yard.pop());
    }
    cout << str.list() << endl;
	/*
		Calculate RPN
	*/
	for(int i=0; i<str.size(); i++){
  		if(isalnum(str[i][0])){
    		stack1.fpush(::atof(str[i].c_str()));
  		} else {
			switch(str[i][0]){
				case '+':
					b = stack1.fpop();
					a = stack1.fpop();
					stack1.fpush(a+b);
					break;
				case '-':
					b = stack1.fpop();
					a = stack1.fpop();
					stack1.fpush(a-b);
					break;
				case '/':
					b = stack1.fpop();
					a = stack1.fpop();
					stack1.fpush(a/b);
					break;
				case '*':
					b = stack1.fpop();
					a = stack1.fpop();
					stack1.fpush(a*b);
					break;
                case '^':
					b = stack1.fpop();
					a = stack1.fpop();
					stack1.fpush(pow(a,b));
					break;
				case '?':
					b = stack1.fpop();
					stack1.fpush(sqrt(b));
			}
		}
	}
	cout << stack1.fpeek() << endl;
}
