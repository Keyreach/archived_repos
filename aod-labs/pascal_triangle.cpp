#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

int main(){
	int n, k;
	int tmp = 0;
	printf("n > ");
	scanf("%d",&n);
	printf("k > ");
	scanf("%d",&k);
	printf("\nPascal's triangle:\n");
	int *row = new int[n];
	int *neu = new int[n];
	row[0] = 1;
	for(int i=0;i<n+1;i++){
		int rowl = i+1;
		for(int l=0; l<((160-tmp)/2); l++){
			printf(" ");
		}
		if(i==n)
			rowl = k+1;
		for(int c=0; c<rowl; c++){
			printf("%d ",row[c]);
		}
		if(i==n){
			printf("<\nResult: %d",row[k]);
		}
		printf("\n",tmp);
		neu[0] = 1;
		tmp = 1;
		for(int j=1; j<rowl; j++){
			neu[j] = (row[j]+row[j-1]);
			tmp += log10(neu[j])+2;
			if((i==n)&&(j==k))
				break;
		}
		neu[rowl] = 1;
		tmp += 1;
		memcpy(row,neu,100);
	}
	return 0;
}
