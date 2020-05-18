#include"dirList.h"
void dinit(dir *d) {
	*d = NULL;
}
void dappend(dir *d, char *str, char *str1) {
	node1 *temp = (node1 *)malloc(sizeof(node1));
	strcpy(temp->u.name, str);
	strcpy(temp->u.path, str1);
	if(*d == NULL) {
		*d = temp;
		(*d)->next = NULL;
	}
	else {
		node1 *ptr = *d;
		while(ptr->next)
			ptr = ptr->next;
		ptr->next = temp;
		temp->next = NULL;
	}
}
void printdir(dir d) {
	node1 *ptr = d;
        while(ptr) {
		printf("%s\n%s\n", ptr->u.name, ptr->u.path);
		ptr = ptr->next;
	}	
}
unit dget(dir *d, int index) {//indexing start from 1
	node1 *ptr= *d;
	int i = 0;
	unit dir;
	while(i < index) {
		ptr = ptr->next;
		i++;
	//	if(ptr)
	//		break;
	}
	strcpy(dir.name, ptr->u.name);
	strcpy(dir.path, ptr->u.path);
	return dir;
}
int dlen(dir d) {
	node1 *ptr = d;
	int i = 0;
	while(ptr) {
		ptr = ptr->next;
		i++;
	}
	return i;
}
void dsort(dir *d) {
	node1 *ptr1 = *d;
	node1 *ptr2;
	unit temp;
	while(ptr1->next) {
		ptr2 = ptr1->next;
		while(ptr2) {
			if(strcmp(ptr1->u.name, ptr2->u.name) > 0) {
				strcpy(temp.name, ptr1->u.name);
				strcpy(ptr1->u.name, ptr2->u.name);
				strcpy(ptr2->u.name, temp.name);
				strcpy(temp.path, ptr1->u.path);
				strcpy(ptr1->u.path, ptr2->u.path);
				strcpy(ptr2->u.path, temp.path);
			}
			ptr2 = ptr2->next;
		}
		ptr1 = ptr1->next;
	}
}

