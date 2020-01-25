#include <iostream>
#include <cstdio>
#include <ctime>
using namespace std;

struct treeNode {
	int value;
	treeNode *left;
	treeNode *right;
};

typedef treeNode* ptree;

ptree mkTree(int arr[], int &from, int n){
	ptree Tree;
	int n1, n2;
	if(n == 0) return NULL;
	Tree = new treeNode;
	Tree->value = arr[from++];
	n1 = n / 2;
	n2 = n - n1 - 1;
	Tree->left = mkTree(arr, from, n1);
	Tree->right = mkTree(arr, from, n2);
	return Tree;
}

void showTree(ptree root, int lvl, char side){
	if(root->right)
		showTree(root->right, lvl+1, 1);
	if(root->left)
		showTree(root->left, lvl+1, 0);
	for(int i=0; i<lvl-1; i++)
		printf("┃  ");
	switch(side){
		case 1: printf("┏"); break;
		case 0: printf("┣"); break;
	}
	printf(" %d\n", root->value);
}

void displayArray(int *arr, int n){
	for(int i=0; i<n; i++){
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main(){
	ptree treeRoot = NULL;
	int n = 15;
	int o = 0;
	int *arr = new int[n];
	srand(time(NULL));
	for(int i=0; i<n; i++){
		arr[i] = rand()%100;
	}
	displayArray(arr, n);
	treeRoot = mkTree(arr, o, n);
	showTree(treeRoot, 1, 0);
	return 0;
}
