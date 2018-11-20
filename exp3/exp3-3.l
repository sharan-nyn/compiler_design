%{
#include <stdlib.h>
#include "y.tab.h"
%}

number		[0-9]+

%%
{number}	{yylval = atoi(yytext); return DIGIT;};
[ \t]		;
\n			return 0;
.			return yytext[0];
%%

int yywrap(void){
	return 1;
}
