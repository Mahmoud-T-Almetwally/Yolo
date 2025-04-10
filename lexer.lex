%{

#include<stdio.h>
#include <string.h>
#include "parser.tab.h" 


#define MAX_STR_CONST 1024 
char string_buf[MAX_STR_CONST];
char *string_buf_ptr;


void yyerror(const char *s); 

%}

%%

"func" {return FUNC; }
"for" {return FOR; }
"while" {return WHILE; }
"if" {return IF; }
"else" {return ELSE; }
"return" {return RETURN; }
"var" {return VAR; }

"int" {return INT; }
"float" {return FLOAT; }
"char" {return CHAR; }
"string" {return STRING;}

[0-9]+ {return INTEGER;}
[0-9]+"."[0-9]+ {return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]* { return IDENT; }


";" {return SEMICOLON; }
":" {return COLON; }
"," {return COMMA; }
"." {return DOT; }
"(" {return LPAREN; }
")" {return RPAREN; }
"{" {return LCURLY; }
"}" {return RCURLY; }
"[" {return LBRACKET; }
"]" {return RBRACKET; }

"+" {return PLUS; }
"-" {return MINUS; }
"*" {return MULTIPLY; }
"/" {return DIVIDE; }
"=" {return ASSIGN; }

"&&" {return AND; }
"||" {return OR; }

"==" {return EQ; }
"!=" {return NEQ; }
"<"  {return LT; }
"<=" {return LTE; }
">"  {return GT; }
">=" {return GTE; }

[\t\n] {}

%%
