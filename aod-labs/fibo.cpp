#include <iostream>

using namespace std;

int rFib(int n){
	int res;
	if(n == 1 || n == 2){
		res = 1;
	} else {
		res = rFib(n-2) + rFib(n-1);
	}
	return res;
}

int iFib(int n){
	if(n <= 2)
		return 1;
	int a = 1;
	int b = 1;
	int c = 0;
	for(int i = 3; i <= n; i++){
		c = a + b;
		a = b;
		b = c;
	}
	return c;
}

int dFib(int a, int b, int n){
	if(n < 3){
		return b;
	} else {
		return dFib(b, a+b, --n);
	}
}

int sFib(int n){
	if(n <= 2)
		return 1;
	int *F = new int[n+1];
	F[1] = 1;
	F[2] = 1;
	for(int i = 3; i <= n; i++){
		F[i] = F[i-1] + F[i-2];
	}
	return F[n];
}

int main(){
	int n, btime;
	cout << "> ";
	cin >> n;
	cout << "\nРекурсивный\n";
	btime = clock();
	cout << "> " << rFib(n) << endl;
	cout << "Время " << (clock() - btime) << endl;
	
	cout << "\nРекурсивный #2\n";
	btime = clock();
	cout << "> " << dFib(1, 1, n) << endl;
	cout << "Время " << (clock() - btime) << endl;
	
	cout << "\nИтерационный\n";
	btime = clock();
	cout << "> " << iFib(n) << endl;
	cout << "Время " << (clock() - btime) << endl;
	
	cout << "\nИтерационный с массивом\n";
	btime = clock();
	cout << "> " << sFib(n) << endl;
	cout << "Время " << (clock() - btime) << endl;
	
	return 0;
}
