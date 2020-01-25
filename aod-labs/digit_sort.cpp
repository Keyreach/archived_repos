#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

using namespace std;

struct Node {
	int num;
	Node *next;
};

typedef Node *node;

node mkNode(int newnum){
	node newItem = new Node;
	newItem->num = newnum;
	newItem->next = NULL;
	return newItem;
}

void addNode(node &Head, int newnum){
	node p = mkNode(newnum);
	node q = Head;
	if(Head==NULL){
		Head = p;
		return;
	}
	while(q->next)
		q = q->next;
	p->next = q->next;
	q->next = p;
}

void displayArray(int *arr, int n){
	for(int i=0; i<n; i++){
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main(){
	int x = 0;
	int n = 50;
	int *arr = new int[n];
	int *src = new int[n];
	node p;
	srand(time(NULL));
	for(int i=0; i<n; i++){
		arr[i] = rand()%1000;
	}
	node heads[10];
	for(int i=0;i<10;i++){
		heads[i] = NULL;
	}
	for(int j=0; j<3; j++){
		x = 0;
		for(int i=0; i<n; i++){
			int a = (int)pow(10, j);
			//int b = (int)pow(10, j+1);
			int dgt = (arr[i]/a)%10;
			printf("adding %d to %d list\n",arr[i],dgt);
			addNode(heads[dgt],i);
		}
		for(int i=0; i<10; i++){
			printf("%d)\t",i);
			p = heads[i];
			while(p){
				printf("%d ",arr[p->num]);
				src[x] = arr[p->num];
				p = p->next;
				x++;
			}
			printf("\n");
		}
		for(int i=0; i<n; i++){
			arr[i] = src[i];
		}
		displayArray(arr,n);
		node r;
		for(int i=0; i<10; i++){
			while(heads[i]){
				r = heads[i]->next;
				delete heads[i];
				heads[i] = r;
			}
		}
	}
	return 0;
}
