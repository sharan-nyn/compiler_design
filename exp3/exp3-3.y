%{
	#include <stdio.h>
	#include <stdlib.h>
	int yylex(void);
	void yyerror(char *);
%}
%start stmt

%token DIGIT
%left '+' '-'
%left '*' '/'

%%
stmt	:	exp		{printf("%d\n\n",$1);}
	;

exp	:	exp '+' exp	{$$ = $1 + $3;}
	|	exp '-' exp	{$$ = $1 - $3;}
	|	exp '*'	exp	{$$ = $1 * $3;}
	|	exp '/' exp	{$$ = $1 / $3;}
	|	'(' exp ')'	{$$ = $2;}
	|	DIGIT		{$$ = $1;}
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
