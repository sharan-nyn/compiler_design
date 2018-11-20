%{
	#include "y.tab.h"
%}
number		[0-9]+
identifier	[A-z|_]([A-z]|[0-9])*

%%
{number}		return(DIGIT);
{identifier}	return(ID);
[ \t] 			;
\n 				return 0;
.				return yytext[0];
%%

int yywrap(void){
	return 1;
}
