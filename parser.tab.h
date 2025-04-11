/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 21 "parser.y"

    #include "symtab.h"
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

#line 57 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FUNC = 258,                    /* FUNC  */
    IDENT = 259,                   /* IDENT  */
    INTEGER = 260,                 /* INTEGER  */
    DOUBLE = 261,                  /* DOUBLE  */
    CHAR = 262,                    /* CHAR  */
    STRING = 263,                  /* STRING  */
    BOOL = 264,                    /* BOOL  */
    T_INTEGER = 265,               /* T_INTEGER  */
    T_FLOAT = 266,                 /* T_FLOAT  */
    T_CHAR = 267,                  /* T_CHAR  */
    T_STRING = 268,                /* T_STRING  */
    T_DOUBLE = 269,                /* T_DOUBLE  */
    T_BOOL = 270,                  /* T_BOOL  */
    T_VOID = 271,                  /* T_VOID  */
    LPAREN = 272,                  /* LPAREN  */
    RPAREN = 273,                  /* RPAREN  */
    LCURLY = 274,                  /* LCURLY  */
    RCURLY = 275,                  /* RCURLY  */
    LBRACKET = 276,                /* LBRACKET  */
    RBRACKET = 277,                /* RBRACKET  */
    DOT = 278,                     /* DOT  */
    SEMICOLON = 279,               /* SEMICOLON  */
    COLON = 280,                   /* COLON  */
    COMMA = 281,                   /* COMMA  */
    VAR = 282,                     /* VAR  */
    IF = 283,                      /* IF  */
    ELSE = 284,                    /* ELSE  */
    WHILE = 285,                   /* WHILE  */
    FOR = 286,                     /* FOR  */
    PRINT = 287,                   /* PRINT  */
    RETURN = 288,                  /* RETURN  */
    ASSIGN = 289,                  /* ASSIGN  */
    OR = 290,                      /* OR  */
    AND = 291,                     /* AND  */
    EQ = 292,                      /* EQ  */
    NEQ = 293,                     /* NEQ  */
    LT = 294,                      /* LT  */
    GT = 295,                      /* GT  */
    LTE = 296,                     /* LTE  */
    GTE = 297,                     /* GTE  */
    PLUS = 298,                    /* PLUS  */
    MINUS = 299,                   /* MINUS  */
    MULTIPLY = 300,                /* MULTIPLY  */
    DIVIDE = 301,                  /* DIVIDE  */
    INC = 302,                     /* INC  */
    DEC = 303,                     /* DEC  */
    NOT = 304,                     /* NOT  */
    POWER = 305,                   /* POWER  */
    UNARY_MINUS = 306,             /* UNARY_MINUS  */
    NEG = 307                      /* NEG  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 37 "parser.y"

    int int_val;
    double double_val;
    char* string_val;
    char char_val;
    bool bool_val;
    symrec* sym_ptr;
    data_type type_val;

#line 136 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
