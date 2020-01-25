#include <iostream>
#include <cstdlib>
#include <utility>
using namespace std;

int n;
void displayArray(int*, int);
void quikc(int, int, int*);

void displayArray(int *arr, int n){
	for(int i=0; i<n; i++){
		cout << arr[i] << " ";
	}
	cout << endl;
}

void quikc(int a, int b, int* arr){
	cout << a << "-" << b << endl;
	int tmp;
	int i = a;
	int j = b;
	int pivot = arr[(i+j)/2];
	while(i<=j){
		while(arr[i]<pivot){
			i++;
		}
		while(arr[j]>pivot){
			j--;
		}
		if(i<=j){
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}
	if(a < j)
		quikc(a, j, arr);
	if(b > i)
		quikc(i, b, arr);
}

void schell(int* arr, int n){
	for(int d=n/2; d>0; d/=2){
		cout << d << endl;
		for(int i = d; i < n; i++){
			for(int j = i; j >= d && arr[j-d] > arr[j]; j-=d){
					swap(arr[j],arr[j-d]);
			}
		}
	}
}

int main(){
	n = 25;
	int* arr = new int[n];
	srand(time(NULL));
	for(int i=0; i<n; i++){
		arr[i] = rand()%100;
	}
	displayArray(arr, n);
	quikc(0, n-1, arr);
	//schell(arr, n);
	displayArray(arr, n);
}
