#include"pairstack.h"
#include<stdlib.h>
void init(pairstack *p) {
	*p = NULL;
}
void push(pairstack *p, pair s) {
	node *temp = (node *)malloc(sizeof(node));
	temp->k = s;
	temp->next = *p;
	*p = temp;
}
pair pop(pairstack *p) {
	node *temp = *p;
	pair z = temp->k;
       	*p = temp->next;
	free(temp);
	return z;
}
int isEmpty(pairstack *p) {
	return (*p == NULL);
}
void traverse(pairstack *p){
	node *ptr = *p;
	while(ptr){
		printf("[%d, %d]\n", ptr->k.x, ptr->k.y);
		ptr = ptr->next;
	}
}
