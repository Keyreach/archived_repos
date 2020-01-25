#include <iostream>

using namespace std;

int getmax(int *a, int b, int e){
	for(int i = 0; i < b; i++){
		printf(" ");
	}
	for(int i = b; i < e; i++){
		printf("#");
	}
	printf("\n");
	//getchar();
	if(e - b < 2){
		if(a[b]>a[e])
			return a[b];
		else
			return a[e];
	}
	int m = (b+e)/2;
	int u = getmax(a, b, m);
	int v = getmax(a, m, e);
	if(u > v)
		return u;
	else
		return v;
}

void displayArray(int *arr, int n){
	for(int i=0; i<n; i++){
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main(){
	int n = 20;
	int *arr = new int[n];
	srand(time(NULL));
	for(int i=0; i<n; i++){
		arr[i] = rand()%200;
	}
	displayArray(arr, n);
	cout << getmax(arr, 0, n) << endl;
	return 0;
}
