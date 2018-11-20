%{
	#include "y.tab.h"
%}

number		[0-9]
char		[A-z_]

%%

{number}	return DIGIT;
{char}		return LETTER;
[ \t]		;
\n		return 0;
.		return yytext[0];

%%

int yywrap(void){
	return 1;
}
