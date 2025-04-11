/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"


#include "symtab.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);
extern int yylineno;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
    
} op_type;


#line 93 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_FUNC = 3,                       /* FUNC  */
  YYSYMBOL_IDENT = 4,                      /* IDENT  */
  YYSYMBOL_INTEGER = 5,                    /* INTEGER  */
  YYSYMBOL_DOUBLE = 6,                     /* DOUBLE  */
  YYSYMBOL_CHAR = 7,                       /* CHAR  */
  YYSYMBOL_STRING = 8,                     /* STRING  */
  YYSYMBOL_BOOL = 9,                       /* BOOL  */
  YYSYMBOL_T_INTEGER = 10,                 /* T_INTEGER  */
  YYSYMBOL_T_FLOAT = 11,                   /* T_FLOAT  */
  YYSYMBOL_T_CHAR = 12,                    /* T_CHAR  */
  YYSYMBOL_T_STRING = 13,                  /* T_STRING  */
  YYSYMBOL_T_DOUBLE = 14,                  /* T_DOUBLE  */
  YYSYMBOL_T_BOOL = 15,                    /* T_BOOL  */
  YYSYMBOL_T_VOID = 16,                    /* T_VOID  */
  YYSYMBOL_LPAREN = 17,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 18,                    /* RPAREN  */
  YYSYMBOL_LCURLY = 19,                    /* LCURLY  */
  YYSYMBOL_RCURLY = 20,                    /* RCURLY  */
  YYSYMBOL_LBRACKET = 21,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 22,                  /* RBRACKET  */
  YYSYMBOL_DOT = 23,                       /* DOT  */
  YYSYMBOL_SEMICOLON = 24,                 /* SEMICOLON  */
  YYSYMBOL_COLON = 25,                     /* COLON  */
  YYSYMBOL_COMMA = 26,                     /* COMMA  */
  YYSYMBOL_VAR = 27,                       /* VAR  */
  YYSYMBOL_IF = 28,                        /* IF  */
  YYSYMBOL_ELSE = 29,                      /* ELSE  */
  YYSYMBOL_WHILE = 30,                     /* WHILE  */
  YYSYMBOL_FOR = 31,                       /* FOR  */
  YYSYMBOL_PRINT = 32,                     /* PRINT  */
  YYSYMBOL_RETURN = 33,                    /* RETURN  */
  YYSYMBOL_ASSIGN = 34,                    /* ASSIGN  */
  YYSYMBOL_OR = 35,                        /* OR  */
  YYSYMBOL_AND = 36,                       /* AND  */
  YYSYMBOL_EQ = 37,                        /* EQ  */
  YYSYMBOL_NEQ = 38,                       /* NEQ  */
  YYSYMBOL_LT = 39,                        /* LT  */
  YYSYMBOL_GT = 40,                        /* GT  */
  YYSYMBOL_LTE = 41,                       /* LTE  */
  YYSYMBOL_GTE = 42,                       /* GTE  */
  YYSYMBOL_PLUS = 43,                      /* PLUS  */
  YYSYMBOL_MINUS = 44,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 45,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 46,                    /* DIVIDE  */
  YYSYMBOL_POWER = 47,                     /* POWER  */
  YYSYMBOL_NOT = 48,                       /* NOT  */
  YYSYMBOL_UNARY_MINUS = 49,               /* UNARY_MINUS  */
  YYSYMBOL_INC = 50,                       /* INC  */
  YYSYMBOL_DEC = 51,                       /* DEC  */
  YYSYMBOL_YYACCEPT = 52,                  /* $accept  */
  YYSYMBOL_program = 53,                   /* program  */
  YYSYMBOL_function = 54,                  /* function  */
  YYSYMBOL_params = 55,                    /* params  */
  YYSYMBOL_param_list = 56,                /* param_list  */
  YYSYMBOL_param = 57,                     /* param  */
  YYSYMBOL_type = 58,                      /* type  */
  YYSYMBOL_literal = 59,                   /* literal  */
  YYSYMBOL_expression = 60,                /* expression  */
  YYSYMBOL_statements = 61,                /* statements  */
  YYSYMBOL_statement = 62,                 /* statement  */
  YYSYMBOL_declaration_statement = 63,     /* declaration_statement  */
  YYSYMBOL_print_statement = 64            /* print_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 31 "parser.y"

    
    data_type token_type_to_data_type(int token_type);
    void handle_binop(symrec* left, symrec* right, symrec** result, int op);
    extern symrec *sym_table;
    extern int yylineno; 

#line 200 "parser.tab.c"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   167

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  42
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  84

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   306


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    85,    85,    86,    90,   102,   116,   117,   122,   123,
     128,   136,   147,   148,   149,   150,   151,   152,   158,   166,
     174,   182,   192,   204,   206,   251,   271,   273,   274,   275,
     276,   277,   303,   311,   312,   316,   321,   322,   323,   324,
     331,   369,   406
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "FUNC", "IDENT",
  "INTEGER", "DOUBLE", "CHAR", "STRING", "BOOL", "T_INTEGER", "T_FLOAT",
  "T_CHAR", "T_STRING", "T_DOUBLE", "T_BOOL", "T_VOID", "LPAREN", "RPAREN",
  "LCURLY", "RCURLY", "LBRACKET", "RBRACKET", "DOT", "SEMICOLON", "COLON",
  "COMMA", "VAR", "IF", "ELSE", "WHILE", "FOR", "PRINT", "RETURN",
  "ASSIGN", "OR", "AND", "EQ", "NEQ", "LT", "GT", "LTE", "GTE", "PLUS",
  "MINUS", "MULTIPLY", "DIVIDE", "POWER", "NOT", "UNARY_MINUS", "INC",
  "DEC", "$accept", "program", "function", "params", "param_list", "param",
  "type", "literal", "expression", "statements", "statement",
  "declaration_statement", "print_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-44)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-3)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      18,    19,   119,   -44,     0,   -18,   -44,   -44,   -44,   -44,
     -44,     4,   -44,    24,     5,     4,     4,   -44,   -44,    96,
     -44,    12,   -44,    21,   140,     4,    10,    -4,    26,    22,
       4,   -44,   -44,   -44,     4,     4,     4,     4,   -44,    33,
     -44,   -44,   -44,   -44,   -44,   -44,    15,    86,   -44,   -44,
     140,    37,   -43,   -43,   -44,   -44,    25,    50,    39,    30,
     -44,     4,    41,    47,   140,    48,    -2,    33,    86,     4,
     -44,   -44,   140,   -44,   140,   -44,    86,   -44,    59,    53,
     -44,   -44,    89,   -44
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      33,     0,     3,     1,     0,    25,    18,    19,    20,    21,
      22,     0,    33,     0,     0,     0,     0,    36,    23,     0,
      34,     0,    38,     0,     0,     0,    25,     0,     0,     0,
       0,    31,    32,    35,     0,     0,     0,     0,    37,     6,
      12,    14,    15,    13,    16,    17,     0,    24,    26,    39,
       0,     0,    27,    28,    29,    30,     0,     0,     0,     7,
       8,     0,     0,     0,     0,     0,     0,     0,    40,     0,
      42,    10,     0,    33,     0,     9,    41,    11,     0,     0,
       4,    33,     0,     5
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -44,   -44,   -44,   -44,   -44,    17,    38,   -44,   -10,   -12,
     -44,   -44,   -44
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,    17,    58,    59,    60,    46,    18,    19,     2,
      20,    21,    22
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      28,    27,    36,    37,    23,    31,    32,    24,    26,     6,
       7,     8,     9,    10,    48,    47,    25,    73,    -2,     3,
      51,    11,    30,    74,    52,    53,    54,    55,    29,     4,
       5,     6,     7,     8,     9,    10,    38,    56,    39,    34,
      35,    36,    37,    11,    25,    12,    49,    50,    15,    61,
      64,    68,    16,    13,    65,    63,    67,    66,    14,    76,
      57,    78,     4,     5,     6,     7,     8,     9,    10,    82,
      15,    70,    81,    72,    16,    69,    11,     0,    12,    80,
      34,    35,    36,    37,    75,     0,    13,     0,    62,     0,
       0,    14,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    71,    15,     0,     0,    11,    16,    12,    83,
      77,     0,    79,     0,     0,     0,    13,     0,     0,     0,
      33,    14,     4,     5,     6,     7,     8,     9,    10,    34,
      35,    36,    37,    15,     0,     0,    11,    16,    12,    34,
      35,    36,    37,     0,     0,     0,    13,     0,     0,     0,
      40,    14,    41,    42,    43,    44,    45,     0,     0,     0,
       0,     0,     0,    15,     0,     0,     0,    16
};

static const yytype_int8 yycheck[] =
{
      12,    11,    45,    46,     4,    15,    16,    25,     4,     5,
       6,     7,     8,     9,    18,    25,    34,    19,     0,     0,
      30,    17,    17,    25,    34,    35,    36,    37,     4,     3,
       4,     5,     6,     7,     8,     9,    24,     4,    17,    43,
      44,    45,    46,    17,    34,    19,    20,    25,    44,    34,
      25,    61,    48,    27,     4,    18,    26,    18,    32,    69,
      27,    73,     3,     4,     5,     6,     7,     8,     9,    81,
      44,    24,    19,    25,    48,    34,    17,    -1,    19,    20,
      43,    44,    45,    46,    67,    -1,    27,    -1,    50,    -1,
      -1,    32,     3,     4,     5,     6,     7,     8,     9,    -1,
      -1,    -1,    64,    44,    -1,    -1,    17,    48,    19,    20,
      72,    -1,    74,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      24,    32,     3,     4,     5,     6,     7,     8,     9,    43,
      44,    45,    46,    44,    -1,    -1,    17,    48,    19,    43,
      44,    45,    46,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      10,    32,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    48
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    53,    61,     0,     3,     4,     5,     6,     7,     8,
       9,    17,    19,    27,    32,    44,    48,    54,    59,    60,
      62,    63,    64,     4,    25,    34,     4,    60,    61,     4,
      17,    60,    60,    24,    43,    44,    45,    46,    24,    17,
      10,    12,    13,    14,    15,    16,    58,    60,    18,    20,
      25,    60,    60,    60,    60,    60,     4,    27,    55,    56,
      57,    34,    58,    18,    25,     4,    18,    26,    60,    34,
      24,    58,    25,    19,    25,    57,    60,    58,    61,    58,
      20,    19,    61,    20
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    58,    58,    58,    58,    59,    59,
      59,    59,    59,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    61,    61,    62,    62,    62,    62,    62,
      63,    63,    64
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     8,    10,     0,     1,     1,     3,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     3,     3,     3,
       3,     2,     2,     0,     2,     2,     1,     2,     1,     3,
       5,     6,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: %empty  */
#line 85 "parser.y"
             {}
#line 1493 "parser.tab.c"
    break;

  case 3: /* program: statements  */
#line 86 "parser.y"
                 {}
#line 1499 "parser.tab.c"
    break;

  case 4: /* function: FUNC IDENT LPAREN params RPAREN LCURLY statements RCURLY  */
#line 90 "parser.y"
                                                             {
        symrec* func = getsym((yyvsp[-6].string_val));
        if (func) {
             char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Function redefinition '%s'", (yyvsp[-6].string_val)); yyerror(err_msg);
             free((yyvsp[-6].string_val));
        } else {
            func = putsym((yyvsp[-6].string_val), SYM_FUNC);
            set_func_return_type(func, TYPE_VOID);
            
            free((yyvsp[-6].string_val)); 
        }
    }
#line 1516 "parser.tab.c"
    break;

  case 5: /* function: FUNC IDENT LPAREN params RPAREN COLON type LCURLY statements RCURLY  */
#line 102 "parser.y"
                                                                          {
         symrec* func = getsym((yyvsp[-8].string_val));
         if (func) {
             char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Function redefinition '%s'", (yyvsp[-8].string_val)); yyerror(err_msg);
             free((yyvsp[-8].string_val));
         } else {
            func = putsym((yyvsp[-8].string_val), SYM_FUNC);
            set_func_return_type(func, (yyvsp[-3].type_val));
            free((yyvsp[-8].string_val));
         }
    }
#line 1532 "parser.tab.c"
    break;

  case 6: /* params: %empty  */
#line 116 "parser.y"
             {}
#line 1538 "parser.tab.c"
    break;

  case 7: /* params: param_list  */
#line 117 "parser.y"
                 {}
#line 1544 "parser.tab.c"
    break;

  case 8: /* param_list: param  */
#line 122 "parser.y"
            {}
#line 1550 "parser.tab.c"
    break;

  case 9: /* param_list: param_list COMMA param  */
#line 123 "parser.y"
                             {}
#line 1556 "parser.tab.c"
    break;

  case 10: /* param: IDENT COLON type  */
#line 128 "parser.y"
                     {
        symrec* p = putsym((yyvsp[-2].string_val), SYM_VAR); 
        if(p) { 
            set_var_type(p, (yyvsp[0].type_val));
            p->is_constant = true; 
        }
        free((yyvsp[-2].string_val)); 
    }
#line 1569 "parser.tab.c"
    break;

  case 11: /* param: VAR IDENT COLON type  */
#line 136 "parser.y"
                           { 
        symrec* p = putsym((yyvsp[-2].string_val), SYM_VAR);
        if(p) {
            set_var_type(p, (yyvsp[0].type_val));
            p->is_constant = false; 
        }
        free((yyvsp[-2].string_val)); 
    }
#line 1582 "parser.tab.c"
    break;

  case 12: /* type: T_INTEGER  */
#line 147 "parser.y"
                { (yyval.type_val) = TYPE_INT; }
#line 1588 "parser.tab.c"
    break;

  case 13: /* type: T_DOUBLE  */
#line 148 "parser.y"
                { (yyval.type_val) = TYPE_DOUBLE; }
#line 1594 "parser.tab.c"
    break;

  case 14: /* type: T_CHAR  */
#line 149 "parser.y"
                { (yyval.type_val) = TYPE_CHAR; }
#line 1600 "parser.tab.c"
    break;

  case 15: /* type: T_STRING  */
#line 150 "parser.y"
                { (yyval.type_val) = TYPE_STRING; }
#line 1606 "parser.tab.c"
    break;

  case 16: /* type: T_BOOL  */
#line 151 "parser.y"
                { (yyval.type_val) = TYPE_BOOL; }
#line 1612 "parser.tab.c"
    break;

  case 17: /* type: T_VOID  */
#line 152 "parser.y"
                { (yyval.type_val) = TYPE_VOID; }
#line 1618 "parser.tab.c"
    break;

  case 18: /* literal: INTEGER  */
#line 158 "parser.y"
            {
        (yyval.sym_ptr) = putsym("_literal_int", SYM_VAR);
        if((yyval.sym_ptr)) { 
             set_var_type((yyval.sym_ptr), TYPE_INT);
             set_var_value_int((yyval.sym_ptr), (yyvsp[0].int_val));
             (yyval.sym_ptr)->is_constant = true; 
        }
    }
#line 1631 "parser.tab.c"
    break;

  case 19: /* literal: DOUBLE  */
#line 166 "parser.y"
             {
        (yyval.sym_ptr) = putsym("_literal_double", SYM_VAR);
        if((yyval.sym_ptr)) {
            set_var_type((yyval.sym_ptr), TYPE_DOUBLE);
            set_var_value_double((yyval.sym_ptr), (yyvsp[0].double_val));
            (yyval.sym_ptr)->is_constant = true;
        }
    }
#line 1644 "parser.tab.c"
    break;

  case 20: /* literal: CHAR  */
#line 174 "parser.y"
           { 
        (yyval.sym_ptr) = putsym("_literal_char", SYM_VAR);
        if((yyval.sym_ptr)) {
            set_var_type((yyval.sym_ptr), TYPE_CHAR);
            set_var_value_char((yyval.sym_ptr), (yyvsp[0].char_val));
            (yyval.sym_ptr)->is_constant = true;
        }
    }
#line 1657 "parser.tab.c"
    break;

  case 21: /* literal: STRING  */
#line 182 "parser.y"
             { 
        (yyval.sym_ptr) = putsym("_literal_string", SYM_VAR);
        if ((yyval.sym_ptr)) {
            set_var_type((yyval.sym_ptr), TYPE_STRING);
            
            set_var_value_string((yyval.sym_ptr), (yyvsp[0].string_val));
            (yyval.sym_ptr)->is_constant = true;
        }
        free((yyvsp[0].string_val)); 
    }
#line 1672 "parser.tab.c"
    break;

  case 22: /* literal: BOOL  */
#line 192 "parser.y"
           { 
        (yyval.sym_ptr) = putsym("_literal_bool", SYM_VAR);
        if ((yyval.sym_ptr)) {
            set_var_type((yyval.sym_ptr), TYPE_BOOL);
            set_var_value_bool((yyval.sym_ptr), (yyvsp[0].bool_val));
            (yyval.sym_ptr)->is_constant = true;
        }
    }
#line 1685 "parser.tab.c"
    break;

  case 24: /* expression: IDENT ASSIGN expression  */
#line 206 "parser.y"
                              {
        (yyval.sym_ptr) = getsym((yyvsp[-2].string_val));
        if (!(yyval.sym_ptr)) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Assignment to undeclared identifier '%s'", (yyvsp[-2].string_val));
            yyerror(err_msg);
             (yyval.sym_ptr) = putsym((yyvsp[-2].string_val), SYM_VAR); 
             if((yyval.sym_ptr)) set_var_type((yyval.sym_ptr), TYPE_UNDEFINED); 
             else (yyval.sym_ptr) = NULL; 
        } else if ((yyval.sym_ptr)->kind != SYM_VAR) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Assignment target '%s' is not a variable", (yyvsp[-2].string_val));
            yyerror(err_msg);
        } else if ((yyval.sym_ptr)->is_constant) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Assignment to constant variable '%s'", (yyvsp[-2].string_val));
            yyerror(err_msg);
        } else if (!(yyvsp[0].sym_ptr) || (yyvsp[0].sym_ptr)->type == TYPE_UNDEFINED) { 
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Cannot assign undefined value to '%s'", (yyvsp[-2].string_val));
            yyerror(err_msg);
        } else if ((yyvsp[0].sym_ptr)->type != (yyval.sym_ptr)->type) {
            
            char err_msg[256];
            
            snprintf(err_msg, sizeof(err_msg), "Type mismatch in assignment to '%s' (expected %d, got %d)", (yyvsp[-2].string_val), (yyval.sym_ptr)->type, (yyvsp[0].sym_ptr)->type);
            yyerror(err_msg);
        } else {
            switch ((yyval.sym_ptr)->type) {
                case TYPE_INT: set_var_value_int((yyval.sym_ptr), (yyvsp[0].sym_ptr)->value.var.int_val); break;
                case TYPE_DOUBLE: set_var_value_double((yyval.sym_ptr), (yyvsp[0].sym_ptr)->value.var.double_val); break;
                case TYPE_CHAR: set_var_value_char((yyval.sym_ptr), (yyvsp[0].sym_ptr)->value.var.char_val); break;
                case TYPE_STRING:
                    
                    if ((yyval.sym_ptr)->value.var.string_val) free((yyval.sym_ptr)->value.var.string_val);
                     
                    set_var_value_string((yyval.sym_ptr), (yyvsp[0].sym_ptr)->value.var.string_val ? (yyvsp[0].sym_ptr)->value.var.string_val : "");
                    break;
                case TYPE_BOOL: set_var_value_bool((yyval.sym_ptr), (yyvsp[0].sym_ptr)->value.var.bool_val); break;
                default: yyerror("Unhandled type in assignment"); break;
             }
        }
        free((yyvsp[-2].string_val)); 
        
    }
#line 1735 "parser.tab.c"
    break;

  case 25: /* expression: IDENT  */
#line 251 "parser.y"
            { 
        (yyval.sym_ptr) = getsym((yyvsp[0].string_val));
        if (!(yyval.sym_ptr)) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Undefined identifier '%s'", (yyvsp[0].string_val));
            yyerror(err_msg);
             (yyval.sym_ptr) = putsym((yyvsp[0].string_val), SYM_VAR); 
             if((yyval.sym_ptr)) set_var_type((yyval.sym_ptr), TYPE_UNDEFINED);
             else (yyval.sym_ptr) = NULL; 
        } else if ((yyval.sym_ptr)->kind != SYM_VAR) { 
             char err_msg[256];
             snprintf(err_msg, sizeof(err_msg), "'%s' is not a variable", (yyvsp[0].string_val));
             yyerror(err_msg);
             
             
             if((yyval.sym_ptr)) set_var_type((yyval.sym_ptr), TYPE_UNDEFINED); 
        }
        free((yyvsp[0].string_val)); 
    }
#line 1759 "parser.tab.c"
    break;

  case 26: /* expression: LPAREN expression RPAREN  */
#line 271 "parser.y"
                               { (yyval.sym_ptr) = (yyvsp[-1].sym_ptr); }
#line 1765 "parser.tab.c"
    break;

  case 27: /* expression: expression PLUS expression  */
#line 273 "parser.y"
                                  { handle_binop((yyvsp[-2].sym_ptr), (yyvsp[0].sym_ptr), &(yyval.sym_ptr), OP_ADD); }
#line 1771 "parser.tab.c"
    break;

  case 28: /* expression: expression MINUS expression  */
#line 274 "parser.y"
                                  { handle_binop((yyvsp[-2].sym_ptr), (yyvsp[0].sym_ptr), &(yyval.sym_ptr), OP_SUB); }
#line 1777 "parser.tab.c"
    break;

  case 29: /* expression: expression MULTIPLY expression  */
#line 275 "parser.y"
                                     { handle_binop((yyvsp[-2].sym_ptr), (yyvsp[0].sym_ptr), &(yyval.sym_ptr), OP_MUL); }
#line 1783 "parser.tab.c"
    break;

  case 30: /* expression: expression DIVIDE expression  */
#line 276 "parser.y"
                                   { handle_binop((yyvsp[-2].sym_ptr), (yyvsp[0].sym_ptr), &(yyval.sym_ptr), OP_DIV); }
#line 1789 "parser.tab.c"
    break;

  case 31: /* expression: MINUS expression  */
#line 277 "parser.y"
                                         { 
        if (!(yyvsp[0].sym_ptr) || (yyvsp[0].sym_ptr)->type == TYPE_UNDEFINED) {
            yyerror("Cannot negate undefined value");
            (yyval.sym_ptr) = putsym("_temp_error", SYM_VAR); 
            if((yyval.sym_ptr)) set_var_type((yyval.sym_ptr), TYPE_UNDEFINED); else (yyval.sym_ptr) = NULL;
        } else if ((yyvsp[0].sym_ptr)->type == TYPE_INT) {
            (yyval.sym_ptr) = putsym("_temp_neg_int", SYM_VAR);
            if((yyval.sym_ptr)) {
                 set_var_type((yyval.sym_ptr), TYPE_INT);
                 set_var_value_int((yyval.sym_ptr), -(yyvsp[0].sym_ptr)->value.var.int_val);
                 (yyval.sym_ptr)->is_constant = (yyvsp[0].sym_ptr)->is_constant; 
            }
        } else if ((yyvsp[0].sym_ptr)->type == TYPE_DOUBLE) {
            (yyval.sym_ptr) = putsym("_temp_neg_double", SYM_VAR);
             if((yyval.sym_ptr)) {
                 set_var_type((yyval.sym_ptr), TYPE_DOUBLE);
                 set_var_value_double((yyval.sym_ptr), -(yyvsp[0].sym_ptr)->value.var.double_val);
                 (yyval.sym_ptr)->is_constant = (yyvsp[0].sym_ptr)->is_constant;
             }
        } else {
            char err_msg[100]; snprintf(err_msg, sizeof(err_msg), "Invalid type (%d) for negation", (yyvsp[0].sym_ptr)->type); yyerror(err_msg);
            (yyval.sym_ptr) = putsym("_temp_error", SYM_VAR); 
            if((yyval.sym_ptr)) set_var_type((yyval.sym_ptr), TYPE_UNDEFINED); else (yyval.sym_ptr) = NULL;
        }
        
    }
#line 1820 "parser.tab.c"
    break;

  case 32: /* expression: NOT expression  */
#line 303 "parser.y"
                     { 
         yyerror("NOT operator not yet implemented");
         (yyval.sym_ptr) = putsym("_temp_error", SYM_VAR);
         if((yyval.sym_ptr)) set_var_type((yyval.sym_ptr), TYPE_UNDEFINED); else (yyval.sym_ptr) = NULL;
    }
#line 1830 "parser.tab.c"
    break;

  case 33: /* statements: %empty  */
#line 311 "parser.y"
             {}
#line 1836 "parser.tab.c"
    break;

  case 34: /* statements: statements statement  */
#line 312 "parser.y"
                           {}
#line 1842 "parser.tab.c"
    break;

  case 35: /* statement: expression SEMICOLON  */
#line 316 "parser.y"
                           {
          if ((yyvsp[-1].sym_ptr) && (strncmp((yyvsp[-1].sym_ptr)->name, "_literal", 8) == 0 || strncmp((yyvsp[-1].sym_ptr)->name, "_temp", 5) == 0)) { 
              
          }
      }
#line 1852 "parser.tab.c"
    break;

  case 39: /* statement: LCURLY statements RCURLY  */
#line 324 "parser.y"
                               {}
#line 1858 "parser.tab.c"
    break;

  case 40: /* declaration_statement: IDENT COLON type ASSIGN expression  */
#line 331 "parser.y"
                                         {
        symrec* existing = getsym((yyvsp[-4].string_val));
        if (existing) {
            
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Redeclaration of '%s'", (yyvsp[-4].string_val));
            yyerror(err_msg);
        } else if (!(yyvsp[0].sym_ptr) || (yyvsp[0].sym_ptr)->type == TYPE_UNDEFINED) {
             char err_msg[256];
             snprintf(err_msg, sizeof(err_msg), "Cannot initialize constant '%s' with undefined value", (yyvsp[-4].string_val));
             yyerror(err_msg);
             
             symrec* sym = putsym((yyvsp[-4].string_val), SYM_VAR);
             if(sym) { set_var_type(sym, (yyvsp[-2].type_val)); sym->is_constant = true; }
        } else if ((yyvsp[0].sym_ptr)->type != (yyvsp[-2].type_val)) {
             
            char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Type mismatch in constant '%s' initialization (expected %d, got %d)", (yyvsp[-4].string_val), (yyvsp[-2].type_val), (yyvsp[0].sym_ptr)->type); yyerror(err_msg);
             symrec* sym = putsym((yyvsp[-4].string_val), SYM_VAR);
             if(sym) { set_var_type(sym, (yyvsp[-2].type_val)); sym->is_constant = true; }
        } else {
            symrec* sym = putsym((yyvsp[-4].string_val), SYM_VAR);
            if(sym) {
                set_var_type(sym, (yyvsp[-2].type_val));
                sym->is_constant = true; 
                switch (sym->type) { 
                    case TYPE_INT: set_var_value_int(sym, (yyvsp[0].sym_ptr)->value.var.int_val); break;
                    case TYPE_DOUBLE: set_var_value_double(sym, (yyvsp[0].sym_ptr)->value.var.double_val); break;
                    case TYPE_CHAR: set_var_value_char(sym, (yyvsp[0].sym_ptr)->value.var.char_val); break;
                    case TYPE_STRING: set_var_value_string(sym, (yyvsp[0].sym_ptr)->value.var.string_val ? (yyvsp[0].sym_ptr)->value.var.string_val : ""); break;
                    case TYPE_BOOL: set_var_value_bool(sym, (yyvsp[0].sym_ptr)->value.var.bool_val); break;
                    default: yyerror("Unhandled type in const initialization"); break;
                 }
            }
        }
        free((yyvsp[-4].string_val)); 
        
    }
#line 1900 "parser.tab.c"
    break;

  case 41: /* declaration_statement: VAR IDENT COLON type ASSIGN expression  */
#line 369 "parser.y"
                                             {
        symrec* existing = getsym((yyvsp[-4].string_val));
         if (existing) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Redeclaration of '%s'", (yyvsp[-4].string_val));
            yyerror(err_msg);
         } else if (!(yyvsp[0].sym_ptr) || (yyvsp[0].sym_ptr)->type == TYPE_UNDEFINED) {
             char err_msg[256];
             snprintf(err_msg, sizeof(err_msg), "Cannot initialize variable '%s' with undefined value", (yyvsp[-4].string_val));
             yyerror(err_msg);
             symrec* sym = putsym((yyvsp[-4].string_val), SYM_VAR);
             if(sym) { set_var_type(sym, (yyvsp[-2].type_val)); sym->is_constant = false; } 
         } else if ((yyvsp[0].sym_ptr)->type != (yyvsp[-2].type_val)) {
            char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Type mismatch in variable '%s' initialization (expected %d, got %d)", (yyvsp[-4].string_val), (yyvsp[-2].type_val), (yyvsp[0].sym_ptr)->type); yyerror(err_msg);
             symrec* sym = putsym((yyvsp[-4].string_val), SYM_VAR);
             if(sym) { set_var_type(sym, (yyvsp[-2].type_val)); sym->is_constant = false; }
         } else {
            symrec* sym = putsym((yyvsp[-4].string_val), SYM_VAR);
            if(sym) {
                set_var_type(sym, (yyvsp[-2].type_val));
                sym->is_constant = false; 
                switch (sym->type) { 
                    case TYPE_INT: set_var_value_int(sym, (yyvsp[0].sym_ptr)->value.var.int_val); break;
                    case TYPE_DOUBLE: set_var_value_double(sym, (yyvsp[0].sym_ptr)->value.var.double_val); break;
                    case TYPE_CHAR: set_var_value_char(sym, (yyvsp[0].sym_ptr)->value.var.char_val); break;
                    case TYPE_STRING: set_var_value_string(sym, (yyvsp[0].sym_ptr)->value.var.string_val ? (yyvsp[0].sym_ptr)->value.var.string_val : ""); break;
                    case TYPE_BOOL: set_var_value_bool(sym, (yyvsp[0].sym_ptr)->value.var.bool_val); break;
                    default: yyerror("Unhandled type in var initialization"); break;
                 }
            }
        }
        free((yyvsp[-4].string_val)); 
        
    }
#line 1939 "parser.tab.c"
    break;

  case 42: /* print_statement: PRINT LPAREN expression RPAREN SEMICOLON  */
#line 406 "parser.y"
                                             {
        if (!(yyvsp[-2].sym_ptr) || (yyvsp[-2].sym_ptr)->type == TYPE_UNDEFINED) {
             yyerror("Cannot print undefined value");
        } else {
            switch((yyvsp[-2].sym_ptr)->type) {
                case TYPE_INT: printf("%d\n", (yyvsp[-2].sym_ptr)->value.var.int_val); break;
                case TYPE_DOUBLE: printf("%f\n", (yyvsp[-2].sym_ptr)->value.var.double_val); break;
                case TYPE_CHAR: printf("%c\n", (yyvsp[-2].sym_ptr)->value.var.char_val); break;
                
                case TYPE_STRING: printf("%s\n", (yyvsp[-2].sym_ptr)->value.var.string_val ? (yyvsp[-2].sym_ptr)->value.var.string_val : "(null)"); break;
                case TYPE_BOOL: printf("%s\n", (yyvsp[-2].sym_ptr)->value.var.bool_val ? "true" : "false"); break;
                case TYPE_VOID: yyerror("Cannot print void value"); break; 
                default: yyerror("Invalid type for print"); 
            }
        }
        
    }
#line 1961 "parser.tab.c"
    break;


#line 1965 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 425 "parser.y"




data_type token_type_to_data_type(int token_type) {
    switch(token_type) {
        case T_INTEGER: return TYPE_INT;
        case T_DOUBLE: return TYPE_DOUBLE;
        case T_CHAR: return TYPE_CHAR;
        case T_STRING: return TYPE_STRING;
        case T_BOOL: return TYPE_BOOL;
        case T_VOID: return TYPE_VOID;
        
        default: return TYPE_UNDEFINED;
    }
}


void handle_binop(symrec* left, symrec* right, symrec** result, int op) {
    
    if (!left || left->type == TYPE_UNDEFINED || !right || right->type == TYPE_UNDEFINED) {
        yyerror("Operand undefined in binary operation");
        *result = putsym("_temp_error", SYM_VAR);
        if(*result) set_var_type(*result, TYPE_UNDEFINED); else *result = NULL;
        return;
    }
    
    bool string_concat = (op == OP_ADD && (left->type == TYPE_STRING || right->type == TYPE_STRING));

    if (!string_concat && left->type != right->type) {
        
        if ((left->type == TYPE_INT && right->type == TYPE_DOUBLE) ||
            (left->type == TYPE_DOUBLE && right->type == TYPE_INT)) {
             yyerror("Implicit type promotion (int/double) not yet implemented");
             *result = putsym("_temp_error", SYM_VAR);
             if(*result) set_var_type(*result, TYPE_UNDEFINED); else *result = NULL;
             return;
        } else {
             
             char err_msg[100]; snprintf(err_msg, sizeof(err_msg), "Type mismatch in binary operation (%d %d)", left->type, right->type); yyerror(err_msg);
            *result = putsym("_temp_error", SYM_VAR);
             if(*result) set_var_type(*result, TYPE_UNDEFINED); else *result = NULL;
            return;
        }
    }
    
    *result = putsym("_temp_binop", SYM_VAR); 
    if (!*result) { yyerror("Memory allocation failed for temporary result"); return; } 

    data_type result_type = left->type; 
    if (string_concat) result_type = TYPE_STRING; 

    set_var_type(*result, result_type);
    (*result)->is_constant = left->is_constant && right->is_constant; 

    switch(result_type) { 
        case TYPE_INT:
            {
                int lval = left->value.var.int_val; 
                int rval = right->value.var.int_val;
                int res_val;
                switch(op) {
                    case OP_ADD: res_val = lval + rval; break;
                    case OP_SUB: res_val = lval - rval; break;
                    case OP_MUL: res_val = lval * rval; break;
                    case OP_DIV:
                        if (rval == 0) { yyerror("Integer division by zero"); set_var_type(*result, TYPE_UNDEFINED); return; }
                        res_val = lval / rval; break;
                    default: yyerror("Unknown integer operation"); set_var_type(*result, TYPE_UNDEFINED); return;
                }
                set_var_value_int(*result, res_val);
            }
            break;
        case TYPE_DOUBLE:
             {
                
                double lval = (left->type == TYPE_DOUBLE) ? left->value.var.double_val : (double)left->value.var.int_val;
                double rval = (right->type == TYPE_DOUBLE) ? right->value.var.double_val : (double)right->value.var.int_val;
                double res_val;
                switch(op) {
                    case OP_ADD: res_val = lval + rval; break;
                    case OP_SUB: res_val = lval - rval; break;
                    case OP_MUL: res_val = lval * rval; break;
                    case OP_DIV:
                        if (rval == 0.0) { yyerror("Floating point division by zero"); set_var_type(*result, TYPE_UNDEFINED); return; }
                        res_val = lval / rval; break;
                    default: yyerror("Unknown double operation"); set_var_type(*result, TYPE_UNDEFINED); return;
                }
                set_var_value_double(*result, res_val);
            }
            break;
        case TYPE_CHAR:
             yyerror("Arithmetic operations on char type not currently supported");
             set_var_type(*result, TYPE_UNDEFINED); 
            break;
        case TYPE_STRING:
            
            if (op == OP_ADD) {
                
                if (left->type != TYPE_STRING || right->type != TYPE_STRING) {
                    yyerror("String concatenation with non-string types not yet implemented");
                    set_var_type(*result, TYPE_UNDEFINED);
                } else {
                    char* lstr = left->value.var.string_val ? left->value.var.string_val : "";
                    char* rstr = right->value.var.string_val ? right->value.var.string_val : "";
                    size_t total_len = strlen(lstr) + strlen(rstr);
                    char* new_str = (char*)malloc(total_len + 1);
                    if (new_str) {
                        strcpy(new_str, lstr);
                        strcat(new_str, rstr);
                        set_var_value_string(*result, new_str); 
                        free(new_str); 
                    } else {
                        yyerror("Memory allocation failed for string concatenation");
                        set_var_type(*result, TYPE_UNDEFINED);
                    }
                }
            } else {
                 yyerror("Invalid operation for string type");
                 set_var_type(*result, TYPE_UNDEFINED);
            }
            break;
        case TYPE_BOOL:
             yyerror("Arithmetic operations on bool type not supported");
             set_var_type(*result, TYPE_UNDEFINED);
            break;
        default:
            yyerror("Invalid result type for binary operation");
            set_var_type(*result, TYPE_UNDEFINED);
    }
    
}


int main(int argc, char *argv[]) {

     int debug_enabled = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) {
            debug_enabled = 1;
        }
    }

    sym_table = NULL;
    yylineno = 1; 
    if(yyparse() == 0) { 
        printf("Yolo: Parsing Successful.\n");
    } else {
        printf("Yolo: Parsing Failed.\n");
    }
    if(debug_enabled) {
        printf("Yolo: debug mode enabled -- to disable it remove '--debug'\n");
        printf("\n--- Symbol Table ---\n");
        print_symbol_table();
        printf("--------------------\n");
    }
    free_symbol_table();
    return 0;
}


void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error on line %d: %s\n", yylineno, s);
}
