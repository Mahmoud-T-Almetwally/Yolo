%{
// MUST BE FIRST: Include headers needed in generated parser.tab.h
#include "symtab.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} op_type;

%}

%code requires {
    #include "symtab.h"
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
}

%code {
    // Forward declaration for helper function
    data_type token_type_to_data_type(int token_type);
    void handle_binop(symrec* left, symrec* right, symrec** result, int op);
    extern symrec *sym_table;
}

%define parse.error verbose
%union {
    int int_val;
    double double_val;
    char* string_val;
    char char_val;
    bool bool_val;
    symrec* sym_ptr;
    data_type type_val;
}

// Token types (fixed)
%token FUNC
%token <string_val> IDENT
%token <int_val> INTEGER
%token <double_val> DOUBLE
%token <char_val> CHAR
%token <string_val> STRING
%token <bool_val> BOOL

// Type tokens
%token T_INTEGER T_FLOAT T_CHAR T_STRING T_DOUBLE T_BOOL T_VOID

// Punctuation
%token LPAREN RPAREN LCURLY RCURLY LBRACKET RBRACKET
%token DOT SEMICOLON COLON COMMA

// Keywords
%token VAR IF ELSE WHILE FOR PRINT RETURN

// Operators
%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MULTIPLY DIVIDE
%left INC DEC
%left NOT
%right POWER
%right UNARY_MINUS

%precedence NEG

// Non-terminal types
%type <type_val> type
%type <sym_ptr> expression
%type <void> statements statement param params function

%start program

%%

program: 
    %empty
    | statements  
    ;

function: 
    FUNC IDENT LPAREN params RPAREN LCURLY statements RCURLY {
        symrec* func = putsym($2, SYM_FUNC);
        set_func_return_type(func, TYPE_VOID);
    }
    | FUNC IDENT LPAREN params RPAREN COLON type LCURLY statements RCURLY {
        symrec* func = putsym($2, SYM_FUNC);
        set_func_return_type(func, $7);
    }
    ;

params: 
    %empty
    | param
    | params COMMA param
    ;

param: 
    IDENT COLON type {
        symrec* p = putsym($1, SYM_VAR);
        set_var_type(p, $3);
        p->is_constant = true;
    }
    | VAR IDENT COLON type {
        symrec* p = putsym($2, SYM_VAR);
        set_var_type(p, $4);
    }
    ;

type: 
    T_INTEGER { $$ = TYPE_INT; }
    | T_DOUBLE { $$ = TYPE_DOUBLE; }
    | T_CHAR { $$ = TYPE_CHAR; }
    | T_STRING { $$ = TYPE_STRING; }
    | T_BOOL { $$ = TYPE_BOOL; }
    | T_VOID { $$ = TYPE_VOID; }
    ;

expression:
    IDENT {
        if (!($$ = getsym($1))) yyerror("Undefined identifier");
    }
    | IDENT COLON type ASSIGN expression {
        if ($5->type != $3) yyerror("Type mismatch");
        $$ = putsym($1, SYM_VAR);
        set_var_type($$, $3);
        $$->is_constant = true;
        memcpy(&$$->value.var, &$5->value.var, sizeof($5->value.var));
    }
    | VAR IDENT COLON type ASSIGN expression {
        if ($6->type != $4) yyerror("Type mismatch");
        $$ = putsym($2, SYM_VAR);
        set_var_type($$, $4);
        memcpy(&$$->value.var, &$6->value.var, sizeof($6->value.var));
    }
    | IDENT ASSIGN expression {
        if (!($$ = getsym($1))) yyerror("Undefined identifier");
        if ($3->type != $$->type) yyerror("Type mismatch");
        memcpy(&$$->value.var, &$3->value.var, sizeof($3->value.var));
    }
    | INTEGER {
        $$ = putsym("_literal", SYM_VAR);
        set_var_type($$, TYPE_INT);
        set_var_value_int($$, $1);
    }
    | DOUBLE {
        $$ = putsym("_literal", SYM_VAR);
        set_var_type($$, TYPE_DOUBLE);
        set_var_value_double($$, $1);
    }
    | CHAR {
        $$ = putsym("_literal", SYM_VAR);
        set_var_type($$, TYPE_CHAR);
        set_var_value_char($$, $1);
    }
    | STRING {
        $$ = putsym("_literal", SYM_VAR);
        set_var_type($$, TYPE_STRING);
        set_var_value_string($$, $1);
    }
    | BOOL {
        $$ = putsym("_literal", SYM_VAR);
        set_var_type($$, TYPE_BOOL);
        set_var_value_bool($$, $1);
    }
    | LPAREN expression RPAREN { $$ = $2; }
    | expression PLUS expression  { handle_binop($1, $3, &$$, OP_ADD); }
    | expression MINUS expression { handle_binop($1, $3, &$$, OP_SUB); }
    | expression MULTIPLY expression { handle_binop($1, $3, &$$, OP_MUL); }
    | expression DIVIDE expression { handle_binop($1, $3, &$$, OP_DIV); }
    | UNARY_MINUS expression %prec NEG {
        $$ = putsym("_temp", SYM_VAR);
        set_var_type($$, $2->type);
        switch($2->type) {
            case TYPE_INT: set_var_value_int($$, -$2->value.var.int_val); break;
            case TYPE_DOUBLE: set_var_value_double($$, -$2->value.var.double_val); break;
            default: yyerror("Invalid type for negation");
        }
    }
    ;

statements: 
    %empty
    | statements statement
    ;

statement: 
    expression SEMICOLON
    | function
    | print_statement
    | LCURLY statements RCURLY
    ;

print_statement: 
    PRINT LPAREN expression RPAREN SEMICOLON {
        switch($3->type) {
            case TYPE_INT: printf("%d\n", $3->value.var.int_val); break;
            case TYPE_DOUBLE: printf("%f\n", $3->value.var.double_val); break;
            case TYPE_CHAR: printf("%c\n", $3->value.var.char_val); break;
            case TYPE_STRING: printf("%s\n", $3->value.var.string_val); break;
            case TYPE_BOOL: printf("%s\n", $3->value.var.bool_val ? "true" : "false"); break;
            default: yyerror("Invalid type for print");
        }
    }
    ;

%%

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
    if (left->type != right->type) {
        yyerror("Type mismatch in binary operation");
        return;
    }
    
    *result = putsym("_temp", SYM_VAR);
    set_var_type(*result, left->type);
    
    switch(left->type) {
        case TYPE_INT:
            switch(op) {
                case OP_ADD: set_var_value_int(*result, left->value.var.int_val + right->value.var.int_val); break;
                case OP_SUB: set_var_value_int(*result, left->value.var.int_val - right->value.var.int_val); break;
                case OP_MUL: set_var_value_int(*result, left->value.var.int_val * right->value.var.int_val); break;
                case OP_DIV: 
                    if (right->value.var.int_val == 0) yyerror("Division by zero");
                    else set_var_value_int(*result, left->value.var.int_val / right->value.var.int_val);
                    break;
            }
            break;
        case TYPE_DOUBLE:
            switch(op) {
                case OP_ADD: set_var_value_double(*result, left->value.var.double_val + right->value.var.double_val); break;
                case OP_SUB: set_var_value_double(*result, left->value.var.double_val - right->value.var.double_val); break;
                case OP_MUL: set_var_value_double(*result, left->value.var.double_val * right->value.var.double_val); break;
                case OP_DIV: 
                    if (right->value.var.double_val == 0.0) yyerror("Division by zero");
                    else set_var_value_double(*result, left->value.var.double_val / right->value.var.double_val);
                    break;
            }
            break;
        case TYPE_CHAR:
            switch(op) {
                case OP_ADD: set_var_value_char(*result, left->value.var.char_val + right->value.var.char_val); break;
                case OP_SUB: set_var_value_char(*result, left->value.var.char_val - right->value.var.char_val); break;
                case OP_MUL: set_var_value_char(*result, left->value.var.char_val * right->value.var.char_val); break;
                case OP_DIV: 
                    if (right->value.var.char_val == 0) yyerror("Division by zero");
                    else set_var_value_char(*result, left->value.var.char_val / right->value.var.char_val);
                    break;
            }
            break;
        case TYPE_STRING:
            // String operations can be handled here
            // For now, we will just print an error
            yyerror("String operations not implemented");
            break;
        case TYPE_BOOL:
            // Boolean operations can be handled here
            // For now, we will just print an error
            yyerror("Boolean operations not implemented");
            break;
        case TYPE_VOID:
            // Void operations are not valid
            yyerror("Void operations not allowed");
            break;
        case TYPE_UNDEFINED:
            // Undefined operations are not valid
            yyerror("Undefined operations not allowed");
            break;
        default:
            yyerror("Invalid type for operation");
    }
}

int main(void) {
    sym_table = NULL;
    yyparse();
    print_symbol_table();
    printf("\n");
    free_symbol_table();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}