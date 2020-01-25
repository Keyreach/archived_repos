#include <stdio.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

int main(){
	int n;
	int curr;
	int prev;
	srand(time(NULL));
	printf("> ");
	scanf("%d",&n);
	int *arr = new int[n];
	printf("Unsorted: \n");
	for(int i = 0; i < n; i++){
		arr[i] = (rand()%50);
		printf("%d ",arr[i]);
	}
	printf("\n\n");
	for(int i = 0; i < n; i++){
		curr = arr[i];
		prev = i - 1;
		while((prev >= 0)&&(arr[prev] > curr)){
			arr[prev+1] = arr[prev];
			arr[prev] = curr;
			prev = prev - 1;
			for(int j = 0; j < n; j++){
				printf("%d ",arr[j]);
			}
			printf("\n");
		}
	}
	printf("\nSorted: \n");
	for(int j = 0; j < n; j++){
			printf("%d ",arr[j]);
	}
	printf("\n");
	return 0;
}
