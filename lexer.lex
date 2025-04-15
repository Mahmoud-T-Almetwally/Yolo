%option noyywrap nounput noinput
%option bison-bridge bison-locations
%option reentrant
%option extra-type="void*"
%option header-file="lex.yy.h"

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

int current_column = 1;

#define YY_USER_ACTION \
    yylloc_param->first_line = yyget_lineno(yyscanner); \
    yylloc_param->first_column = current_column; \
    yylloc_param->last_line = yyget_lineno(yyscanner); \
    yylloc_param->last_column = current_column + yyget_leng(yyscanner) - 1; \
    current_column += yyget_leng(yyscanner);

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
    "true"            { yylval_param->bool_val = true; return BOOL; }
    "false"           { yylval_param->bool_val = false; return BOOL; }

    "int"             { return T_INTEGER; }
    "float"           { /* Treat as double */ return T_FLOAT; }
    "char"            { return T_CHAR; }
    "string"          { return T_STRING; }
    "double"          { return T_DOUBLE; }
    "bool"            { return T_BOOL; }
    "void"            { return T_VOID; }
    [0-9]+            { yylval_param->int_val = atoi(yytext); return INTEGER; }
    [0-9]+\.[0-9]+    { yylval_param->double_val = atof(yytext); return DOUBLE; }

    \'([^\\\'\n]|\\.)\' {
        if (yytext[1] == '\\') { 
            if (yyleng == 4) {
                yylval_param->char_val = handle_escape(yytext[2], yyscanner);
            } else {
                compiler_error(yyget_lineno(yyscanner),"Invalid escape sequence in character literal: %s", yytext);
                yylval_param->char_val = '?'; 
            }
        } else { 
            if (yyleng == 3) { 
                yylval_param->char_val = yytext[1];
            } else { // Multi-character like 'abc' - error
                compiler_error(yyget_lineno(yyscanner), "Multi-character character literal '%s'", yytext);
                yylval_param->char_val = '?'; 
            }
        }
        return CHAR;
    }
    \'[^\'\n]*\n {
        compiler_error(yyget_lineno(yyscanner)-1, "Unterminated character literal");
        current_column = 1; 
        
     }
    \' {
        compiler_error(yyget_lineno(yyscanner), "Empty or invalid character literal starting here");
     }

    \" {
        string_buf_ptr = string_buf;
        if (string_buf_ptr >= string_buf + MAX_STR_CONST) {
            compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow at start");
        } else {
            *string_buf_ptr = '\0'; 
            BEGIN(STRING_STATE);
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

    [ \t]+            { current_column += yyleng;  }
    \n                { current_column = 1; }

    .                 { 
                        compiler_error(yyget_lineno(yyscanner), "Unknown character '%c' (ASCII: %d) at column %d", *yytext, *yytext, yylloc_param->first_column);
                      }
}

<STRING_STATE>{
    \" {
        BEGIN(INITIAL); 
        *string_buf_ptr = '\0'; 
        if (string_buf_ptr >= string_buf + MAX_STR_CONST) {
             compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow near end");
             yylval_param->string_val = strdup(""); 
        } else {
             yylval_param->string_val = strdup(string_buf);
             if (!yylval_param->string_val) {
                 compiler_error(yyget_lineno(yyscanner), "Memory allocation failed for string literal");
             }
        }
        return STRING;
    }

    \\.? {
        char escaped_char;
        if (yyleng < 2 || yytext[1] == '\n') { 
             if (yytext[1] == '\n') { // Backslash followed by newline is invalid escape
                  compiler_error(yyget_lineno(yyscanner)-1, "Illegal escape sequence (backslash followed by newline) in string literal");
             } else { 
                  compiler_error(yyget_lineno(yyscanner), "Incomplete escape sequence at end of string literal");
                  escaped_char = '\\';
             }
              if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                 *string_buf_ptr++ = '\\'; 
              } else {  }

        } else { 
             escaped_char = handle_escape(yytext[1], yyscanner);
             if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                 *string_buf_ptr++ = escaped_char;
             } else {
                 compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow during escape sequence processing");
             }
        }
        *string_buf_ptr = '\0';
    }

    \n {
        compiler_error(yyget_lineno(yyscanner)-1, "Newline in string constant not allowed (unterminated string?)");
        BEGIN(INITIAL);   
        current_column = 1; 
    }

    [^\\\"\n]+ {
        char *yptr = yytext;
        while (*yptr) {
            if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                *string_buf_ptr++ = *yptr++;
            } else {
                compiler_error(yyget_lineno(yyscanner), "String constant buffer overflow while processing literal content");
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