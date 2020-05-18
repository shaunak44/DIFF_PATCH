#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include"fileList.h"
int main(int argc, char *argv[]) {
	//printf("%d\n", argc);
	if(argc == 3) {
		FILE *fp = fopen(argv[1], "r+");
		FILE *patch = fopen(argv[2], "r+");
		char ch, str[2048], str1[2048];
		ch = fgetc(patch);
		int i = 0; 
		fileList f1;
		printf("patching file...\n");
		while(fgets(str, 2048, patch)) {
			int j = 0;
			if(!strstr(str, "-")){
				if(!strstr(str, "*")){
					for(i = 2; i < 2048; i++) {
						str1[j] = str[i];
						j++;
					}
					fprintf(fp, "%s", str1);
				}
			}
		}
	}
	else if(argc == 4 && !strcmp(argv[1], "-R")) {
		FILE *fp = fopen(argv[2], "r+");
		FILE *patch = fopen(argv[3], "r+");
		char ch, str[2048], str1[2048];
		ch = fgetc(patch);
		int i = 0; 
		printf("reversing the patch....\n");
		fileList f1;
		while(fgets(str, 2048, patch)) {
			int j = 0;
			if(!strstr(str, "---"))
				if(!strstr(str, "+")){
					if(!strstr(str, "*")){
					//	printf("%s", str);
						for(i = 2; i < 2048; i++) {
							str1[j] = str[i];
							j++;
						}
						fprintf(fp, "%s", str1);
					}
				}
		}
	}
}
