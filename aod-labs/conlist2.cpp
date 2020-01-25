#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct Item {
	int value;
	Item *next;
};

typedef Item *kitem;

kitem createItem(int val){
	kitem NewItem = new Item;
	NewItem->value = val;
	return NewItem;	
}

void addFirst(kitem &Head, kitem NewItem){
	NewItem->next = NewItem;
	Head = NewItem;
}

void addLast(kitem &Head, kitem NewItem){
	kitem q = Head;
	if(Head == NULL){
		addFirst(Head, NewItem);
		return;
	}
	while(q->next!=Head){
		q = q->next;
	}
	NewItem->next = Head;
	q->next = NewItem;
}

void removeNFold(kitem &Head, int n){
	kitem q = Head, pre;
	int k = 0;
	while(q && (q->next!=q)){
		k++;
		if(k==(n-1)){
			pre = q;
		}
		if(k==n){
			printf("%d has been removed\n",q->value);
			pre->next = q->next;
			if(q==Head)
				Head = q->next;
			delete q;
			k = 0;
		}
		q = q->next;
	}
}

int main(){
	kitem Head = NULL, p;
	int NFold, Max;
	printf("M > ");
	scanf("%d",&NFold);
	printf("N > ");
	scanf("%d",&Max);
	for(int i=0; i<Max; i++){
		p = createItem(i+1);
		addLast(Head, p);
	}
	removeNFold(Head, NFold);
	printf("\nLast element:\n%d\n",Head->value);
	return 1;
}
