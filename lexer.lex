%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

#define MAX_STR_CONST 1024 
char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

void yyerror(const char *s); 
%}

%%
"func"            { return FUNC; }
"print"           { return PRINT; }
"for"             { return FOR; }
"while"           { return WHILE; }
"if"              { return IF; }
"else"            { return ELSE; }
"return"          { return RETURN; }
"var"             { return VAR; }

"int"             { return T_INTEGER; }
"float"           { return T_FLOAT; }
"char"            { return T_CHAR; }
"string"          { return T_STRING; }
"double"          { return T_DOUBLE; }
"bool"            { return T_BOOL; }
"void"            { return T_VOID; }

[0-9]+            { yylval.int_val = atoi(yytext); return INTEGER; }
[0-9]+"."[0-9]+   { yylval.double_val = atof(yytext); return DOUBLE; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.string_val = strdup(yytext); return IDENT; }

";"               { return SEMICOLON; }
":"               { return COLON; }
","               { return COMMA; }
"."               { return DOT; }
"("               { return LPAREN; }
")"               { return RPAREN; }
"{"               { return LCURLY; }
"}"               { return RCURLY; }
"["               { return LBRACKET; }
"]"               { return RBRACKET; }

"+"               { return PLUS; }
"-"               { return MINUS; }
"*"               { return MULTIPLY; }
"**"              { return POWER; }
"/"               { return DIVIDE; }
"="               { return ASSIGN; }

"&&"              { return AND; }
"||"              { return OR; }

"=="              { return EQ; }
"!="              { return NEQ; }
"<"               { return LT; }
"<="              { return LTE; }
">"               { return GT; }
">="              { return GTE; }

"++"              { return INC; }
"--"              { return DEC; }

[ \t\n]            { }
%%
