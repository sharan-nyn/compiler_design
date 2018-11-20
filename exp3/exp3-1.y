%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex(void);
	void yyerror(char *);
%}

%token DIGIT ID

%left '+' '-'
%left '*' '/'

%%
stmt :	ID '=' exp ';' {printf("Valid Statement\n");}
	;
exp :	exp '+' exp
	|	exp '-' exp
	|	exp '*'	exp
	|	exp '/' exp
	|	'(' exp ')'
	|	DIGIT
	|	ID
	;
%%

void yyerror(char *s){
	printf("Invalid Statement\n");
	exit(0);
}

int main(void){
	while(1)
		yyparse();
	return 0;
}
