#include<stdio.h>
typedef struct pair {
	int x;
	int y;
}pair;
typedef struct node {
	pair k;
	struct node *next;
}node;
typedef node *pairstack;
void init(pairstack *p);
void push(pairstack *p, pair k);
pair pop(pairstack *p);
int isEmpty(pairstack *p);
void traverse(pairstack *p);

