#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

long double iteratMul(int *arr, int n){
	long double result = 1;
	for(int i=0; i<=n; i++){
		result *= arr[i];
	}
	return result;
}

long double recursMul(int *arr, int n){
	if(n==0)
		return arr[0];
	else
		return arr[n]*recursMul(arr,n-1);
}

int main(){
	unsigned int st;
	int n;
	printf("> ");
	scanf("%d",&n);
	int *arr = new int[n];
	srand(time(NULL));
	for(int i=0; i<n; i++){
		arr[i] = rand()%2+1;
	}
	st = clock();
	cout << iteratMul(arr,n-1) << endl;
	cout << "Time: " << (clock()-st) << endl;
	st = clock();
	cout << recursMul(arr,n-1) << endl;
	cout << "Time: " << (clock()-st) << endl;
	return 0;
}
