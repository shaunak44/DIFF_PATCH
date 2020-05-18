#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<ctype.h>
#include<time.h>
#include<sys/stat.h>
#include"fileList.h"
#include"pairstack.h"
#include"dirList.h"
#define max(m, n) (m >= n)?m:n
#define D 1
#define F 2
int compareFlag = 0;//flag for -i option
int neglectFlag = 0;//flag for -w option
int comnegFlag = 0;//flag for -iw option
int existFlag = 0;//flag for -b option
int strcmpb(char *s1, char *s2) {
	int i = 0, j = 0 ;
	int count = 0;
	int flag = 0;
	while(i < strlen(s1) && j < strlen(s2)) {
		if(s1[i] == s2[j] || ((s1[i]) == ' ' && s2[j] == '\t') || (s1[i] == '\t' && s2[j] == '\t')) {
			if(s1[i] == ' ' || s1[i] == '\t') {
				while(s1[i] == ' ' || s1[i] == '\t')
					i++;
				while(s2[j] == ' ' || s2[j] == '\t')
					j++;
			}
			else {
				i++;
				j++;
			}
		}
		else 
			return 1;
	}
	if(s1[i] == '\0' && s2[j] == '\0')
		return 0;
	else 
		return 1;
}
int strcmpi(char *s1, char *s2) {
	int i;
	if(strlen(s1) != strlen(s2))
		return -1;
	for(i = 0; i < strlen(s1); i++)
		if(toupper(s1[i]) != toupper(s2[i]))
			return s1[i] - s2[i];
	return 0;	
}
int strcmpiw(char *str1, char *str2) {
	int i = 0, j = 0, s1, s2;
	while(str1[i] != '\0' && str2[j] != '\0') {
		while(str1[i] == ' ' || str1[i] == '\t')
			i++;
		while(str2[j] == ' ' || str2[j] == '\t')
			j++;
		s1 = str1[i];
		s2 = str2[j];
		if((s1 == s2) || (s1 == (s2 - 32)) || (s1 == (s2 + 32))) {
			i++;
			j++;
		}
		else
			return s1 - s2;
	}
	if(str1[i] == '\0' && str2[j] == '\0')
		return 0;
	else
		return str1[i] - str2[j];
}
int strcmpw(char *str1, char *str2) {
	int i = 0, j = 0;
	while(str1[i] != '\0' && str2[j] != '\0') {
		while(str1[i] == ' ' || str1[i] == '\t')
			i++;
		while(str2[j] == ' ' || str2[j] == '\t')
			j++;
		if(str1[i] == str2[j]) {
			i++;
			j++;
		}
		else
			return str1[i] - str2[j];
	}
	if(str1[i] == '\0' && str2[j] == '\0')
		return 0;
	else
		return str1[i] - str2[j];
}
void LCS_file(fileList f1, fileList f2, pairstack *trace){
	int m = flength(&f1), n = flength(&f2);
	int l[m+1][n+1];
	int i, j;
	char *str1, *str2;
	pair p;
	//pairstack trace;
	//init(&trace);
	node *ptr = *trace;
	int count = 0;
	for(i = 0; i <= m; i++) {
		if(i > 0)
			str1 = fgetline(&f1, i - 1);
		for(j = 0;j <= n; j++){
			if(j > 0)
				str2 = fgetline(&f2, j - 1);
			if(i == 0 || j == 0)
				l[i][j] = 0;
			else if(!(strcmp(str1, str2)))
				l[i][j] = l[i-1][j-1] + 1;
			else 
				l[i][j] = max(l[i-1][j], l[i][j-1]);

		} 
	}
	int index = l[m][n];
	//printf("%d", index);
	/*for(i = 0; i <= m; i++)
		for(j = 0; j <= n; j++)
			printf("%d\n", l[i][j]);*/
	char lcs[index + 1][2048];//lcs array stores the LCS of two files
	i = m; j = n;
	str1 = fgetline(&f1, i - 1);
	str2 = fgetline(&f2, j - 1);
	p.x = i;
	p.y = j;
	push(trace, p);
	while(i > 0 && j > 0) {
		if(!strcmp(str1, str2)){
			i--;
			j--;
			strcpy(lcs[index - 1], str1);
			index--;
			count++;
			p.x = i;
			p.y = j;
			push(trace, p);
			str1 = fgetline(&f1, i - 1);
			str2 = fgetline(&f2, j - 1);
		}
		else if(l[i - 1][j] > l[i][j - 1]){
			i--;
			p.x = i;
			p.y = j;
			push(trace, p);
			str1 = fgetline(&f1, i - 1);
		}
		else {
			j--;
			p.x = i;
			p.y = j;
			push(trace, p);
			str2 = fgetline(&f2, j - 1);
		}
	}
	while(i == 0 && j > 0){
		j--;
		p.x = i;
		p.y = j;
		push(trace, p);
	}
	while(i > 0 && j == 0){
		i--;
		p.x = i;
		p.y = j;
		push(trace, p);

	}
	//traverse(trace);
	//for(i = 0; i < (index + count); i++)//printing the Longest common subsequence
	//	printf("%s", lcs[i]);
}
/*printing the output in -c format*/
void cprint(fileList f1, fileList f2, pairstack *trace){
	 node *ptr = *trace;
	 int x = 0, y = 0;
	 int i = 1;
	 /*while(ptr->next) {//loop to find series of operation
	 	if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)){
			printf("delete\n");
		}
		else if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
			printf("insert\n");
		}
		else { 
			printf("nothing\n");
		}
	       ptr = ptr->next;
	 }*/
	 printf("***%d,%d***\n", i, flength(&f1));
	 ptr = *trace;
	 while(ptr->next) {//for file1 output
	 	if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)){
			printf("- %s", fgetline(&f1, ptr->k.x));
		//	printf("delete\n");
		}
		else if((ptr->k.x) == (ptr->next->k.x)){
			//printf("  %s", fgetline(&f1, ptr->k.x));
		}
		else {
			printf("  %s", fgetline(&f1, ptr->k.x));
		}
		ptr = ptr->next;
	 }
	 ptr = *trace;
	 printf("\n---%d,%d---\n", i, flength(&f2));	 
	 while(ptr->next) {//for file2 output
	 	if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
			printf("+ %s", fgetline(&f2, ptr->k.y));
		}
		else if((ptr->k.y) == (ptr->next->k.y)) {
			
		}
		else {
			printf("  %s", fgetline(&f2, ptr->k.y));
		}
		ptr = ptr->next;
	 }
	 printf("\n");
}
char *get_line(int fd) {
	int i = 0;
	static char string1[2048];
	while(read(fd, &string1[i], 1)){
		if(string1[i] == '\n')
			break;
		i++;
	}
	i++;
	string1[i] = '\0';
	return string1;	
}
void uprint(fileList f1, fileList f2, pairstack *trace, char *file1, char *file2){
	node *ptr = *trace;
	int x = 0, y = 0;
	int flag = 0;
	struct stat filestat1, filestat2;
	stat(file1, &filestat1);
	stat(file2, &filestat2);
	printf("*** %s\t%s", file1, ctime(&filestat1.st_mtime));
	printf("--- %s\t%s", file2, ctime(&filestat2.st_mtime));
	printf("***************\n");
	int temp;
	printf("@@ -1,%d +1,%d @@\n", flength(&f1), flength(&f2));
	while(ptr->next){
		flag = 0;
		if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)){
			printf("- %s", fgetline(&f1, ptr->k.x));
			flag = 1;
		//	printf("delete\n");
		}	
		if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
			printf("+ %s", fgetline(&f2, ptr->k.y));
			flag = 2;
		}
		if(flag == 0){
			printf("  %s", fgetline(&f1, ptr->k.x));
		}
		ptr = ptr->next;
	}
}
void modcprint(fileList f1, fileList f2, pairstack *trace, char *file1, char *file2) {
	node *ptr = *trace;
	node *ptr1;
	int x = 0, y = 0;
	int i = 1, j = 0;
	int flagD = 0, flagC = 0;
	struct stat filestat1, filestat2;
	stat(file1, &filestat1);
	stat(file2, &filestat2);
	printf("*** %s\t%s", file1, ctime(&filestat1.st_mtime));
	printf("--- %s\t%s", file2, ctime(&filestat2.st_mtime));
	printf("***************\n");
        printf("***%d,%d***\n", i, flength(&f1));	
	while(ptr->next) {//for file1 output
		ptr1 = ptr;
		flagD = 0;
		flagC = 0;
		while(ptr1->next) {
			if((ptr1->k.x) < (ptr1->next->k.x) && (ptr1->k.y) == (ptr1->next->k.y)){
				//printf("delete\n");
				flagD++;
			}
			else if((ptr1->k.x) == (ptr1->next->k.x) && (ptr1->k.y) < (ptr1->next->k.y)){
				//printf("insert\n");
				if(flagD){
					for(j = 0; j < flagD; j++)
						printf("! %s", fgetline(&f1 , ptr->k.x + j));
					flagC = 1;
					ptr = ptr1->next;
					//printf("! %s", fgetline(&f2, ptr->k.y));
				}
				break;
			}
			else 
				break;
			ptr1 = ptr1->next;
		}
		if(flagC == 0) {
			if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)){
				printf("- %s", fgetline(&f1, ptr->k.x));
				//	printf("delete\n");
			}
			else if((ptr->k.x) == (ptr->next->k.x)){
				//printf("%s", fgetline(&f1, ptr->k.x));
			}
			else {
				printf("  %s", fgetline(&f1, ptr->k.x));
			}
			ptr = ptr->next;
		}
		//ptr = ptr->next;
	}
	printf("\n---%d,%d---\n", i, flength(&f2));	 
	ptr = *trace;
	while(ptr->next) {//for file2 output
		ptr1 = ptr;
		flagD = 0;
		flagC = 0;
		while(ptr1->next) {
			if((ptr1->k.x) < (ptr1->next->k.x) && (ptr1->k.y) == (ptr1->next->k.y)){
				//printf("delete\n");
				//if(flagD)
				//	flagI = 0;
				flagD++;
			}
			else if((ptr1->k.x) == (ptr1->next->k.x) && (ptr1->k.y) < (ptr1->next->k.y)){
				//printf("insert\n");
				if(flagD){
					//printf("! %s", fgetline(&f1 , ptr->k.x));
					printf("! %s", fgetline(&f2, ptr1->k.y));
					flagC++;
					ptr = ptr1->next;
				}
				break;
			}
			else {
				//ptr = ptr1->next;
				break;
			}
			ptr1 = ptr1->next;
		}
		if(flagC == 0) {
			if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
				printf("+ %s", fgetline(&f2, ptr->k.y));
			}
			else if((ptr->k.y) == (ptr->next->k.y)) {

			}
			else {
				printf("  %s", fgetline(&f2, ptr->k.y));
			}
		ptr = ptr->next;	
		}
	}	
}
void defprint(fileList f1, fileList f2, pairstack *trace) {
	node *ptr = *trace;
	node *ptr1;
	int flagD = 0;
	int flagC = 0;
	int i = 0;
	compareFlag = 0;
	while(ptr->next) {
		ptr1 = ptr;
		flagD = 0;
		flagC = 0;
		while(ptr1->next) {
			if((ptr1->k.x) < (ptr1->next->k.x) && (ptr1->k.y) == (ptr1->next->k.y)){
				//printf("delete\n");
				flagD++;
			}
			else if((ptr1->k.x) == (ptr1->next->k.x) && (ptr1->k.y) < (ptr1->next->k.y)){
				//printf("insert\n");
				if(flagD > 1){
					printf("%d,%dc%d\n", ptr->k.x + 1, ptr->k.y + flagD, ptr->k.y + 1);
					for(i = 0; i < flagD; i++)
						printf("< %s", fgetline(&f1, ptr->k.x + i));
					printf("---\n");
					printf("> %s", fgetline(&f2, ptr->k.y));
					flagC = 1;
					ptr = ptr1->next;
					//printf("! %s", fgetline(&f2, ptr->k.y));
				}
				else if(flagD == 1) {
					flagC = 1;
					printf("%dc%d\n", ptr->k.x + 1, ptr->k.y + 1);
					printf("< %s", fgetline(&f1, ptr->k.x));
					printf("---\n");
					printf("> %s", fgetline(&f2, ptr->k.y));
					ptr = ptr1->next;
				}
				break;
			}
			else 
				break;
			ptr1 = ptr1->next;
		}
		if(flagC == 0) {
			if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
				//printf("+ %s", fgetline(&f2, ptr->k.y));
				printf("%da%d\n", ptr->k.x, ptr->next->k.y);
				printf("> %s", fgetline(&f2, ptr->k.y));
			}
			else if((ptr->k.y) == (ptr->next->k.y) && (ptr->k.x) < (ptr->next->k.x)) {
				if(flagD > 1) {
					printf("%d,%dd%d\n", ptr->k.x + 1, ptr->k.y + flagD, ptr->k.y);
					for(i = 0; i < flagD; i++)
						printf("< %s", fgetline(&f1, ptr->k.x + i));
					ptr = ptr1;
				}
				else if(flagD == 1) {
					printf("%dd%d\n", ptr->k.x + 1, ptr->k.y);
					printf("< %s", fgetline(&f1, ptr->k.x));
				}
			}
			else {
			//	printf("  %s", fgetline(&f2, ptr->k.y));
			}
		ptr = ptr->next;
		}
	}
}
void createpatch(fileList f1, fileList f2, pairstack *trace, char *str, char* file1, char *file2) {
	FILE *patch;
	patch = fopen(str, "w+");
	node *ptr = *trace;
	int x = 0, y = 0;
	int flag = 0;
	int temp;
	struct stat filestat1, filestat2;
	stat(file1, &filestat1);
	stat(file2, &filestat2);
	fprintf(patch, "*** %s\t%s", file1, ctime(&filestat1.st_mtime));
	fprintf(patch, "--- %s\t%s", file2, ctime(&filestat2.st_mtime));
	fprintf(patch, "***************\n");
	while(ptr->next){
		flag = 0;
		if((ptr->k.x) < (ptr->next->k.x) && (ptr->k.y) == (ptr->next->k.y)){
			fprintf(patch, "- %s", fgetline(&f1, ptr->k.x));
			flag = 1;
		//	printf("delete\n");
		}	
		if((ptr->k.x) == (ptr->next->k.x) && (ptr->k.y) < (ptr->next->k.y)){
			fprintf(patch, "+ %s", fgetline(&f2, ptr->k.y));
			flag = 2;
		}
		if(flag == 0){
			fprintf(patch, "  %s", fgetline(&f1, ptr->k.x));
		}
		ptr = ptr->next;
	}
}
void finaldiffforfile(char *file1, char *file2) {
	FILE *fp1, *fp2;
	fp1 = fopen(file1, "r");
	fp2 = fopen(file2, "r");
	pairstack trace;
	init(&trace);
	char str1[2048];
	fileList f1, f2;
	finit(&f1);
	finit(&f2);
	int temp;	
	while(fgets(str1, 2048, fp1) != NULL)
		fappend(&f1, str1);
	while(fgets(str1, 2048, fp2) != NULL)
		fappend(&f2, str1);
	LCS_file(f1, f2, &trace);
	defprint(f1, f2, &trace);
	fdestroy(&f1);
	fdestroy(&f2);
}
void diffdir(dir d1, dir d2) {
	int com, i = 0, j = 0;
	int dlen1 = dlen(d1), dlen2 = dlen(d2);
	char fulldir1[1024], fulldir2[1024];//storing the full name of dir.
	unit u1, u2;
	dsort(&d1);
	dsort(&d2);
	while(i < dlen1 || j < dlen2) {
		if(i < dlen1 && j < dlen2) {
			u1 = dget(&d1, i);
			u2 = dget(&d2, j);
			//printf("%s\n%s", u1.name, u2.name);
			com = strcmp(u1.name, u2.name);
			//printf("\n%d\n", com);
		}
		else {
			if(i == dlen1){
				u2 = dget(&d2, j);
				com = 1;
			}
			else if (j == dlen2){
				u1 = dget(&d1, i);
				com = -1;
			}
		}
		if(com < 0) {
			printf("only in %s: %s\n", u1.path, u1.name);
			if(i < dlen1)
				i++;
		}
		else if(com > 0) {
			printf("only in %s: %s\n", u2.path, u2.name);
			if(j < dlen2)
				j++;
		}
		else {
			strcpy(fulldir1, u1.path);
			strcat(fulldir1, "/");
			strcat(fulldir1, u1.name);
			strcpy(fulldir2, u2.path);
			strcat(fulldir2, "/");
			strcat(fulldir2, u2.name);
			//printf("diff %s/%s %s/%s", u1.path, u1.name, u2.path, u2.name);
			finaldiffforfile(fulldir1, fulldir2);
			if(i < dlen1)
				i++;
			if(j < dlen2)
				j++;
		}
	//	printf("[%d, %d]", i, j);
	}
}
int isDirectory(char *name) {
	DIR *dir1 = opendir(name);
	struct dirent *entry1;
	while((entry1 = readdir(dir1)) != NULL) {
		if(entry1->d_type == DT_DIR) {
			if(strcmp(entry1->d_name, ".") == 0 || strcmp(entry1->d_name, "..") == 0)
				continue;
			return 1;
		}
		else
			return 0;
	}
	return 0;
}
int main(int argc, char *argv[]){
	//FILE *fp1, *fp2;
	//printf("%d\n", argc);
	int flag = 0, dflag1 = 0, dflag2 = 0, pflag = 0;;//flag for directory 1 and 2
	FILE *fp1, *fp2;
	if(argc <= 3){
		fp1 = fopen(argv[1], "r");
		if(!fp1) {
			printf("%s failed to open\n", argv[1]);
			return 0;
		}
		fp2 = fopen(argv[2], "r");
		if(!fp2){
			printf("%s failed to open\n", argv[2]);
			return 0;
		}
		flag = 0;
	}
	else if(argc == 4){
		fp1 = fopen(argv[2], "r");
		if(!fp1) {
			printf("%s failed to open\n", argv[2]);
			return 0;
		}
		fp2 = fopen(argv[3], "r");
		if(!fp2) {
			printf("%s failed to open\n", argv[3]);
			return 0;
		}
		flag = 11;
	}
	else if(argc == 6) {
		fp1 = fopen(argv[2], "r");
		if(!fp1) {
			printf("%s failed to open\n", argv[2]);
			return 0;
		}
		fp2 = fopen(argv[3], "r");
		if(!fp2) {
			printf("%s failed to open\n", argv[3]);
			return 0;
		}
		pflag = 1;
	}
	if(!strcmp(argv[1], "-c")){
		flag = 1;
	}
	else if(!strcmp(argv[1], "-u") && pflag == 0) {
		flag = 2;
	} 
	else if(!(strcmp(argv[1], "-i"))) {
		flag = 3;
		compareFlag = 1;
	}
	else if(!(strcmp(argv[1], "-w"))) {
		flag = 4;
		neglectFlag = 1;
	}	
	else if(!(strcmp(argv[1], "-iw"))) {
		flag = 5;
		comnegFlag = 1;
	}
	else if(!(strcmp(argv[1], "-b"))) {
		flag = 6;
		existFlag = 1;
	}
	else if(!strcmp(argv[1], "-u") && pflag == 1) {
		flag = 7;
	}
        else if(flag == 0) {
		
	}	
	else if(flag == 11){
		printf("diff: invalid option -- \n");
		return 0;
	}
	pairstack trace;//store the track of graph
	init(&trace);
	char str1[2048];
	fileList f1, f2;
	finit(&f1);
	finit(&f2);
	int temp;	
	while(fgets(str1, 2048, fp1) != NULL)
		fappend(&f1, str1);
	while(fgets(str1, 2048, fp2) != NULL)
		fappend(&f2, str1);
	LCS_file(f1, f2, &trace);
	switch(flag){
		case 1:
			modcprint(f1, f2, &trace, argv[2], argv[3]);
			break;
		case 2:
			uprint(f1, f2, &trace, argv[2], argv[3]);
			break;
		case 3:
			defprint(f1, f2, &trace);
			break;
		case 4:
			defprint(f1, f2, &trace);
			break;
		case 5:
			defprint(f1, f2, &trace);
			break;
		case 6:
			defprint(f1, f2, &trace);
			break;
		case 7:
			createpatch(f1, f2, &trace, argv[5], argv[2], argv[3]);
			break;
		case 0:
			defprint(f1, f2, &trace);
			if(strstr(argv[1], ".txt") && strstr(argv[2], ".txt") || strstr(argv[1], ".c") && strstr(argv[2], ".c"))
				flag = 1;
			break;
		default:
			return 0;
			break;
	}
	//createpatch(f1, f2, &trace, argv[3]);
	fdestroy(&f1);
	fdestroy(&f2);
	if(flag == 0) {
		if(!isDirectory(argv[1]))
			dflag1 = 1;
		else 
			dflag1 = 0;
		if(!isDirectory(argv[2]))
			dflag2 = 1;
		else 
			dflag2 = 0;
		//printf("%d%d", dflag1, dflag2);
		if(dflag1 && dflag2) {
			FILE *fp;
			char filename[1024];
			struct dirent *de1, *de2;
			DIR *dr1 = opendir(argv[1]);
			DIR *dr2 = opendir(argv[2]);
			dir d1, d2;
			unit u1, u2;
			dinit(&d1);
			dinit(&d2);
			while((de1 = readdir(dr1)) != NULL) {
				if(!strcmp(de1->d_name, "."))
					continue;
				if(!strcmp(de1->d_name, ".."))
					continue;
				//printf("$$%s$$\n", de->d_name);
				sprintf(filename, "%s/%s", argv[1], de1->d_name);
				//printf("%s\n", filename);
				//if(de1->d_type == DT_DIR){
				dappend(&d1, de1->d_name, argv[1]);//dir type
				//}
				//else
				//	dappend(&d1, de1->d_name, argv[1]);//dir type
			}
			while((de2 = readdir(dr2)) != NULL) {
				if(!strcmp(de2->d_name, "."))
					continue;
				if(!strcmp(de2->d_name, ".."))
					continue;
				//printf("$$%s$$\n", de->d_name);
				sprintf(filename, "%s/%s", argv[2], de2->d_name);
				//printf("%s\n", filename);
				//if(de1->d_type == DT_DIR)
				dappend(&d2, de2->d_name, argv[2]);
				//else
				//	dappend(&d2, de2->d_name, argv[2]);	
			}
			//printdir(d1);
			//printdir(d2);
			//printf("%d\n", dlen(d1));
			u1 = dget(&d1, 2);
			//printf("%s\n%s\n", u1.name, u1.path);
			diffdir(d1, d2);

		}
	}

}
