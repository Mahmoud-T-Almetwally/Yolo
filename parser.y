%{

#include <stdio.h>

int yylex(void);
void yyerror(const char *s);

extern int yylineno;
extern char *yytext; 

%}

%union {
    int ival;         // Example: For integers
    char *sval;       // For strings (identifiers, string literals)
    float fval;     // For floating-point numbers
}

%define parse.error verbose

%token IDENT
%token VAR
%token FUNC
%token INT
%token NUMBER
%token STR
%token CHAR
%token STRING
%token INTEGER
%token FLOAT
%token LPAREN
%token RPAREN
%token LCURLY
%token RCURLY
%token LBRACKET
%token RBRACKET
%token DOT
%token SEMICOLON
%token COLON
%token COMMA
%token PLUS
%token MINUS
%token MULTIPLY
%token DIVIDE
%token ASSIGN
%token IF
%token ELSE
%token WHILE
%token FOR
%token RETURN
%left OR
%left AND
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UNARY
%start program

%%

program: statements ;  

function: FUNC IDENT LPAREN params RPAREN LCURLY statements RCURLY
    | FUNC IDENT LPAREN RPAREN LCURLY statements RCURLY
    | FUNC IDENT LPAREN RPAREN LCURLY RCURLY
    ;
params: 
    param
    | params COMMA param
    ;

param: IDENT COLON type
    | VAR IDENT COLON type
    ;


type: INT | FLOAT | CHAR | STRING;

statements: statements statement
    | %empty
    ;
statement: expression SEMICOLON
    | IF LPAREN expression RPAREN LCURLY statements RCURLY
    | IF LPAREN expression RPAREN LCURLY statements RCURLY ELSE LCURLY statements RCURLY
    | WHILE LPAREN expression RPAREN LCURLY statements RCURLY
    | FOR LPAREN expression SEMICOLON expression SEMICOLON expression RPAREN LCURLY statements RCURLY
    | function
    | %empty
    ;

return_statement: RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

expression: IDENT COLON type ASSIGN expression
    | VAR IDENT COLON type ASSIGN expression
    | IDENT DOT IDENT
    | IDENT LBRACKET expression RBRACKET
    | expression PLUS expression
    | expression MINUS expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | expression EQ expression
    | expression NEQ expression
    | expression LT expression
    | expression LTE expression
    | expression GT expression
    | expression GTE expression
    | expression AND expression
    | expression OR expression
    | LPAREN expression RPAREN
    | IDENT
    | INTEGER
    | NUMBER
    | STR
    | %empty
    ;

%%

int main(void){
    yyparse();
}

void yyerror(const char *s){
    fprintf(stderr, "Error: %s\n", s);
    printf("\n");
}