#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<string.h>
#include<dirent.h>
typedef struct unit {
	char name[1024];
	char path[1024];
}unit;
typedef struct node1{
	unit u;
	struct node1 *next;
}node1;
typedef node1 *dir;
void dinit(dir *d);
void dappend(dir *d, char *str, char *str1);
void printdir(dir d);
unit dget(dir *d, int index);
int dlen(dir d);
void dsort(dir *d);
