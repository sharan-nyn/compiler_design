#include <stdio.h>
#include <ctype.h>
#include <string.h>
int iskeyword(char buff[10]){
	char keywords[32][10] =  {"auto","break","case","char","const","continue","default",
							  "do","double","else","enum","extern","float","for","goto",
							  "if","int","long","register","return","short","signed",
							  "sizeof","static","struct","switch","typedef","union",
							  "unsigned","void","volatile","while"};
	int i,j,flag;
	for(i=0;i<32;i++){
		j=0;
		flag = 1;
		while(flag){
			if(buff[j] != keywords[i][j]) flag=0;
			else if(j == strlen(buff)) break;
			j++;
		}
		if(flag) return 1;
	}
	return 0;
}

int isop(char *buff){
	char aops[][2]={"+","-","*","/","%"},
	     relops[][3]={"==","!=","<=",">=",">","<"},
	     logops[][3]={"&&","||","!"},
	     asgops[][3]={"+=","-=","/=","*=","=","%="};
	int i;
	for(i=0;i<6;i++){
		if(strcmp(relops[i],buff) == 0){
			printf("%s\trelop\n",buff);
			return 1;
		}
	}
	for(i=0;i<3;i++){
		if(strcmp(logops[i],buff) == 0){
			printf("%s\tlogop\n",buff);
			return 1;
		}
	}
	for(i=0;i<6;i++){
		if(strcmp(asgops[i],buff) == 0){
			printf("%s\tasgop\n",buff);
			return 1;
		}
	}
	for(i=0;i<5;i++){
		if(strcmp(aops[i],buff) == 0){
			printf("%s\tarthop\n",buff);
			return 1;
		}
	}
	return 0;
}

int startsWith(const char *pre, const char *str){
	size_t lenpre = strlen(pre),
		   lenstr = strlen(str);
	return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

int endsWith(const char *suffix,const char *str){
	char *dot = strrchr(str, suffix[0]);
	return (dot && !strcmp(dot, suffix));
}

int readLine(char **line,FILE *fp){
	int status,read;
	size_t size = 1024;
	status = (read = getline(line,&size,fp));
	if (status != EOF){
		if ((*line)[read - 1] == '\n') (*line)[read - 1] = '\0'; // Trim new line character
		char *ptr = NULL;
		for(ptr = *line; *ptr == ' ' || *ptr == '\t'; ptr++) ; // Trim initial space and tabs
		strcpy(*line, ptr);
		return 1;
	}
	return 0;
}

void main(){
	FILE *fp=fopen("code.c","r");
	char *line = NULL,buff[10],*tok;
	int status,count=0;
	while(readLine(&line,fp)){
		line = strtok(line,";");
		if(line != NULL){
			if(startsWith("/*",line)){ //Skip multiline comments
				while(readLine(&line,fp) && !endsWith("*/",line));
			}else if (!(startsWith("//",line) || startsWith("#",line))){ //Skip single line comments & preprocessor driectives
				tok = strtok(line," ");
				while(tok != NULL){
					if(iskeyword(tok)){
						printf("%s\tkeyword\n",tok);
						count++;
					}else if(isdigit(tok[0])){
						printf("%s\tnumber\n",tok);
						count++;
					}else if(isop(tok)){
						count++;
					}else if(startsWith("\"",tok)){
						char string[1024];
						strcpy(string,tok);
						if(!endsWith("\"",tok)){ //if same word doesn't end with quotes, copy all words till end quote
							do{
								tok = strtok(0," ");
								if(tok != NULL){
									strcat(string," ");
									strcat(string,tok);
								}else break;
							}while(!endsWith("\"",tok));
						}
						printf("%s\tliteral\n",string);
						count++;
					}else if(isalpha(tok[0])){
						printf("%s\tidentifier\n",tok);
						count++;
					}
					tok = strtok(0," ");
				}
			}
		}
	}
	printf("Total tokens : %d\n",count);
	fclose(fp);
}