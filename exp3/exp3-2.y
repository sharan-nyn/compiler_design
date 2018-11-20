%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex(void);
	void yyerror(char *);
%}

%token DIGIT LETTER

%%

stmt	:	LETTER REST {printf("Valid Variable\n\n");}
	;

REST	:	LETTER REST
	|	DIGIT REST
	|	LETTER
	|	DIGIT
	;

%%
void yyerror(char *s){
	printf("Invalid Variable\n");
	exit(0);
}

int main(void){
	while(1)
		yyparse();
	return 0;
}
