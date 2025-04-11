%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h" 

#define MAX_STR_CONST 1024
char string_buf[MAX_STR_CONST]; 
char *string_buf_ptr;


void yyerror(const char *s);
extern int yylineno; 


char handle_escape(char c) {
    switch (c) {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        
        default:
            
            
            fprintf(stderr, "Warning line %d: Unknown escape sequence '\\%c'\n", yylineno, c);
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

    [0-9]+            { yylval.int_val = atoi(yytext); return INTEGER; }
    [0-9]+"."[0-9]+   { yylval.double_val = atof(yytext); return DOUBLE; }

    
    
    \'([^\\\'\n]|\\.)\' {
        if (yytext[1] == '\\') {
            
            if (yyleng == 4) { 
                 yylval.char_val = handle_escape(yytext[2]);
            } else {
                 
                 yyerror("Invalid escape sequence in character literal");
                 yylval.char_val = '?'; 
            }
        } else {
            
            if (yyleng == 3) { 
                yylval.char_val = yytext[1];
            } else {
                 yyerror("Invalid character literal");
                 yylval.char_val = '?'; 
            }
        }
        
        return CHAR;
    }
    
     \' { yyerror("Invalid or unterminated character literal"); }

    
    \" {
        string_buf_ptr = string_buf; 
        
        if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
            *string_buf_ptr = '\0'; 
             BEGIN(STRING_STATE);         
        } else {
             yyerror("String constant buffer overflow at start");
             
        }
    }

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
    "!"               { return NOT; }   

    [ \t]+            { /* Ignore whitespace except newline */ }
    \n                { yylineno++; /* Track line numbers */ }
    .                 { char err_msg[100]; sprintf(err_msg, "Unknown character '%c'", *yytext); yyerror(err_msg); } 
}


<STRING_STATE>{
    
    \" {
        BEGIN(INITIAL); 
        *string_buf_ptr = '\0'; 
        
        yylval.string_val = strdup(string_buf);
        return STRING;
    }

    
    \\.? { 
        if (yyleng < 2) { 
             yyerror("Incomplete escape sequence in string literal");
             
             if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                *string_buf_ptr++ = '\\';
                *string_buf_ptr = '\0'; 
             } else {
                  
             }
        } else {
            char escaped_char = handle_escape(yytext[1]);
            if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                *string_buf_ptr++ = escaped_char;
                *string_buf_ptr = '\0'; 
            } else {
                 
                 yyerror("String constant buffer overflow during escape sequence processing");
                 
            }
        }
    }

    
    \n {
        yyerror("Newline in string constant not allowed");
        yylineno++;   
    }
    
    [^\\\"\n]+ {
        char *yptr = yytext;
        while (*yptr) {
             if (string_buf_ptr < string_buf + MAX_STR_CONST - 1) {
                 *string_buf_ptr++ = *yptr++;
             } else {
                 yyerror("String constant buffer overflow");
                 
                 *string_buf_ptr = '\0'; 
                 
                 
                 break; 
             }
        }
        *string_buf_ptr = '\0'; 
    }
    
    <<EOF>> {
        yyerror("Unterminated string literal at end of file");
        BEGIN(INITIAL); 
        return YY_NULL; 
    }
}

%%
