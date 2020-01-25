#include <iostream>
#include <string>
#include <cmath>
#include <stack>
using namespace std;

int priority(char in){
    return
        in == '+' || in == '-' ? 1 :
        in == '*' || in == '/' ? 2 :
        in == '^' ? 3 :
        -1;
}

string convertToRPN(string raw){
	stack<string> yard;
	string tmp = "";
	string str = "";
	for(unsigned int i = 0; i < raw.size(); i++){
        if(isdigit(raw[i])){
            while(i < raw.size() && isdigit(raw[i])){
                tmp += raw[i];
                ++i;
            }
            str += tmp + " ";
            --i;
            tmp = "";
        } else if(raw[i]=='(') {
            yard.push(string(1,raw[i]));
        } else if(raw[i]==')') {
            while(yard.top()!="("){
                str += yard.top() + " ";
                yard.pop();
            }
            yard.pop();
        } else {
			if(!yard.empty())
			while((priority(yard.top()[0])>=priority(raw[i]))&&(!yard.empty())){
                str += yard.top() + " ";
              	yard.pop();
              	if(yard.empty())
					break;
			}
            yard.push(string(1,raw[i]));
        }
    }
    while(yard.size()>0){
        str += yard.top() + " ";
        yard.pop();
    }
    return str;
}

float calculateRPN(string str){
	float a, b;
	string tmp = "";
	stack<float> stack1;
	tmp = "";
	for(unsigned int i=0; i<str.size(); i++){
  		if(isdigit(str[i])){
			while(i < str.size() && isdigit(str[i])){
                tmp += str[i];
                ++i;
            }
    		stack1.push(::atof(tmp.c_str()));
    		--i;
    		tmp = "";
  		} else {
			switch(str[i]){
				case '+':
					b = stack1.top();
					stack1.pop();
					a = stack1.top();
					stack1.pop();
					stack1.push(a+b);
					break;
				case '-':
					b = stack1.top();
					stack1.pop();
					a = stack1.top();
					stack1.pop();
					stack1.push(a-b);
					break;
				case '/':
					b = stack1.top();
					stack1.pop();
					a = stack1.top();
					stack1.pop();
					stack1.push(a/b);
					break;
				case '*':
					b = stack1.top();
					stack1.pop();
					a = stack1.top();
					stack1.pop();
					stack1.push(a*b);
					break;
                case '^':
					b = stack1.top();
					stack1.pop();
					a = stack1.top();
					stack1.pop();
					stack1.push(pow(a,b));
					break;
			}
		}
	}
	return stack1.top();
}

int main(){
	string raw, parsed;
	//cout << "\n TYPE \"quit\" TO EXIT\n" << endl;
	while(1){
		cout << ">> ";
		cin >> raw;
		if(raw=="quit")
			break;
		parsed = convertToRPN(raw);
		cout << "== " << parsed << endl;
		cout << "== " << calculateRPN(parsed) << endl;
	}
}
