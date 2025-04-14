%option noyywrap nounput noinput 
%option bison-bridge bison-locations
%option reentrant
%option extra-type="void*"

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "utils.h" 

#define MAX_STR_CONST 1024
#define YY_DECL int yylex(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, void* yyscanner)
char string_buf[MAX_STR_CONST]; 
char *string_buf_ptr;

int yyget_lineno(void* scanner);
void yyset_lineno(int line_number, void* scanner);

char handle_escape(char c, void* scanner) {
    switch (c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        
        default:
            compiler_warning(yyget_lineno(scanner), "Unknown escape sequence '\\%c'", c);
            return c; 
    }
}

%}


%x STRING_STATE

%%

<INITIAL>{
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

    [0-9]+            { yylval_param->int_val = atoi(yytext); return INTEGER; }
    [0-9]+"."[0-9]+   { yylval_param->double_val = atof(yytext); return DOUBLE; }

    
    
    \'([^\\\'\n]|\\.)\' {
        if (yytext[1] == '\\') {
            
            if (yyleng == 4) { 
                yylval_param->char_val = handle_escape(yytext[2], yyscanner);
            } else {
                 
                compiler_error(yyget_lineno(yyscanner),"Invalid escape sequence in character literal");
                yylval_param->char_val = '?'; 
            }
        } else {
            
            if (yyleng == 3) { 
                yylval_param->char_val = yytext[1];
            } else {
                compiler_error(yyget_lineno(yyscanner), "Multi-character literal '%s'", yytext);
                yylval_param->char_val = '?';
            }
        }
        
        return CHAR;
    }
    
    \'[^\'\n]*\n { compiler_error(yyget_lineno(yyscanner)-1, "Unterminated character literal"); /* Error occurred on previous line */ BEGIN(INITIAL); }
    \' { compiler_error(yyget_lineno(yyscanner), "Empty or invalid character literal"); }

    
    \" {
        string_buf_ptr = string_buf; 
        
        if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
            *string_buf_ptr = '\0'; 
             BEGIN(STRING_STATE);         
        } else {
            
            compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow at start");
             
        }
    }

    [a-zA-Z_][a-zA-Z0-9_]*  { yylval_param->string_val = strdup(yytext); return IDENT; }

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
    "!"               { return NOT; }   

    [ \t]+            { /* Ignore whitespace except newline */ }
    \n                {  }
    .                 { compiler_error(yyget_lineno(yyscanner), "Unknown character '%c'", *yytext); } 
}


<STRING_STATE>{
    
    \" {
        BEGIN(INITIAL); 
        *string_buf_ptr = '\0'; 
        
        yylval_param->string_val = strdup(string_buf);
        return STRING;
    }

    
    \\.? { 
        if (yyleng < 2) {

             compiler_error(yyget_lineno(yyscanner), "Incomplete escape sequence in string literal");
             
             if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                *string_buf_ptr++ = '\\';
                *string_buf_ptr = '\0'; 
             } else {

                compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow before escape sequence");

             }
        } else {
            char escaped_char = handle_escape(yytext[1], yyscanner);
            if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                *string_buf_ptr++ = escaped_char;
                *string_buf_ptr = '\0'; 
            } else {
                 
                compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow during escape sequence processing");
                 
            }
        }
    }

    
    \n {
        compiler_error(yyget_lineno(yyscanner)-1, "Newline in string constant not allowed");
        BEGIN(INITIAL);   
    }
    
    [^\\\"\n]+ {
        char *yptr = yytext;
        while (*yptr) {
            if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                *string_buf_ptr++ = *yptr++;
            } else {
                compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow");
                    
                *string_buf_ptr = '\0'; 
                
                
                break; 
            }  
        }
        *string_buf_ptr = '\0'; 
    }
    
    <<EOF>> {
        compiler_error(yyget_lineno(yyscanner), "Unterminated string literal at end of file");
        BEGIN(INITIAL); 
        return YY_NULL; 
    }
}

%%
