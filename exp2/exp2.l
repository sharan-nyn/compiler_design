%{
	int count = 0;
%}

keyword			auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while
number			[0-9]+
arthimetic-op	\+|-|\*|\/|%
relational-op	==|!=|<=|>=|>|<
logical-op		&&|\|\||!
assignment-op	=|\+=|-=|\/=|\*=|%=
singlecomment	\/\/.*
multicomment	\/\*(.|\n)*\*\/
string			\".*\"
identifier		[A-z|_]([A-z]|[0-9])*
header			#.*

%%
{singlecomment}	;
{multicomment}	;
{header}		;
{string}		{ printf("%s\t\t Literal\n", yytext); count++;};
{keyword}		{ printf("%s\t\t Keyword\n", yytext);count++; };
{number}		{ printf("%s\t\t Number\n", yytext);count++; };
{assignment-op}	{ printf("%s\t\t Assignment-Operator\n", yytext);count++; };
{relational-op}	{ printf("%s\t\t Relational-Operator\n", yytext);count++; };
{logical-op}	{ printf("%s\t\t Logical-Operator\n", yytext);count++; };
{arthimetic-op}	{ printf("%s\t\t Arthimetic-Operator\n", yytext);count++; };
{identifier}	{ printf("%s\t\t Identifier\n", yytext);count++; };
.				;
\n 				;
%%

void main(){
	yylex();
	printf("Count: %d\n", count);
}

int yywrap(){
	return 1;
}