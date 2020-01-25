#include <stdio.h>
#include <bitset>
using namespace std;

const int n = 1024;

int main(){
	int n0, q, i, j;
	n0 = n/2;
	bitset<n/2> A;
	for(i = 1; i < n0; i++){
		A[i] = 1;
	}
	for(i = 1; i < n0; i++){
		if(A[i] != 0){
			q = 2 * i + 1;
			for(j = 2 * q; j < n; j += q){
				if(j % 2 == 1){
					A[(j-1)/2] = 0;
				}
			}
		}
	}
	printf("1 2 ");
	for(i = 1; i < n0; i++){
		if(A[i]==1)
			printf("%d ",2*i+1);
	}
	printf("\n%d\n",sizeof(A));
	return 0;
}
