#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct Item {
	int value;
	Item *next;
};

typedef Item *kItem;

kItem createItem(int val){
	kItem NewItem = new Item;
	NewItem->value = val;
	return NewItem;	
}

void addFirst(kItem &Head, kItem NewItem){
	NewItem->next = Head;
	Head = NewItem;
}

void addLast(kItem &Head, kItem NewItem){
	kItem q = Head;
	if(Head == NULL){
		addFirst(Head, NewItem);
		return;
	}
	while(q->next)
		q = q->next;
	NewItem->next = q->next;
	q->next = NewItem;
}

void removeNFold(kItem &Head, int n){
	kItem q = Head, pre;
	int k = 0;
	while(q && (q->next!=q)){
		k++;
		if(k==(n-1)){
			pre = q;
		}
		if(k==n){
			pre->next = q->next;
			if(q==Head)
				Head = q->next;
			printf("-> Removing %d\n",q->value);
			delete q;
			k = 0;
		}
		q = q->next;
	}
}

void removeAll(kItem &Head){
	kItem r;
	while(Head){
		r = Head->next;
		printf("-> Removing %d\n",Head->value);
		delete Head;
		Head = r;
	}
}

int main(){
	kItem Head = NULL, p, q;
	int n, k;
	printf("Number of list elements -> ");
	scanf("%d",&n);
	printf("Remove n-fold elements -> ");
	scanf("%d",&k);
	for(int i=0; i<n; i++){
		p = createItem(i+1);
		addLast(Head, p);
	}
	q = Head;
	printf("-> 1\n");
	while(q){
		printf("%d, ",q->value);
		q = q->next;
	}
	printf("\n");
	removeNFold(Head, k);
	printf("\n-> 2\n");
	q = Head;
	while(q!=NULL){
		printf("%d, ",q->value);
		q = q->next;
	}
	printf("\n");
	removeAll(Head);
	if(Head==NULL)
		printf("Head is NULL\n");
	return 1;
}
