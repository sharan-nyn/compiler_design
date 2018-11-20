# Generate YACC specification for a few syntactic categories.
##	1. Program to recognize a valid arithmetic expression that uses operator +, – , * and /.
##	2. Program to recognize a valid variable which starts with a letter followed by any number of letters or digits.
##	3. Implementation of Calculator using LEX and YACC.

Credits for the code: @kishorv06
1. lex exp3-1.l
2. yacc -d exp3-1.y
3. gcc lex.yy.c y.tab.c
4. ./a.out