%{

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

%}

%code requires {
    #include "symtab.h"
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
}

%code {
    
    data_type token_type_to_data_type(int token_type);
    void handle_binop(symrec* left, symrec* right, symrec** result, int op);
    extern symrec *sym_table;
    extern int yylineno; 
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

%token FUNC
%token <string_val> IDENT
%token <int_val> INTEGER
%token <double_val> DOUBLE
%token <char_val> CHAR    
%token <string_val> STRING 
%token <bool_val> BOOL

%token T_INTEGER T_FLOAT T_CHAR T_STRING T_DOUBLE T_BOOL T_VOID

%token LPAREN RPAREN LCURLY RCURLY LBRACKET RBRACKET
%token DOT SEMICOLON COLON COMMA

%token VAR IF ELSE WHILE FOR PRINT RETURN

%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MULTIPLY DIVIDE POWER 
%left NOT                  
%right UNARY_MINUS          
%precedence INC DEC

%type <type_val> type
%type <sym_ptr> expression literal
%type <void> program statements statement param params param_list function print_statement declaration_statement 

%start program

%%

program:
      %empty {} 
    | statements {} 
    ;

function:
    FUNC IDENT LPAREN params RPAREN LCURLY statements RCURLY {
        symrec* func = getsym($2);
        if (func) {
             char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Function redefinition '%s'", $2); yyerror(err_msg);
             free($2);
        } else {
            func = putsym($2, SYM_FUNC);
            set_func_return_type(func, TYPE_VOID);
            
            free($2); 
        }
    }
    | FUNC IDENT LPAREN params RPAREN COLON type LCURLY statements RCURLY {
         symrec* func = getsym($2);
         if (func) {
             char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Function redefinition '%s'", $2); yyerror(err_msg);
             free($2);
         } else {
            func = putsym($2, SYM_FUNC);
            set_func_return_type(func, $7);
            free($2);
         }
    }
    ;

params:
      %empty {} 
    | param_list {} 
    ;


param_list:
      param {} 
    | param_list COMMA param {} 
    ;


param:
    IDENT COLON type {
        symrec* p = putsym($1, SYM_VAR); 
        if(p) { 
            set_var_type(p, $3);
            p->is_constant = true; 
        }
        free($1); 
    }
    | VAR IDENT COLON type { 
        symrec* p = putsym($2, SYM_VAR);
        if(p) {
            set_var_type(p, $4);
            p->is_constant = false; 
        }
        free($2); 
    }
    ;

type:
      T_INTEGER { $$ = TYPE_INT; }
    | T_DOUBLE  { $$ = TYPE_DOUBLE; }
    | T_CHAR    { $$ = TYPE_CHAR; }
    | T_STRING  { $$ = TYPE_STRING; }
    | T_BOOL    { $$ = TYPE_BOOL; }
    | T_VOID    { $$ = TYPE_VOID; }
    
    ;


literal:
    INTEGER {
        $$ = putsym("_literal_int", SYM_VAR);
        if($$) { 
             set_var_type($$, TYPE_INT);
             set_var_value_int($$, $1);
             $$->is_constant = true; 
        }
    }
    | DOUBLE {
        $$ = putsym("_literal_double", SYM_VAR);
        if($$) {
            set_var_type($$, TYPE_DOUBLE);
            set_var_value_double($$, $1);
            $$->is_constant = true;
        }
    }
    | CHAR { 
        $$ = putsym("_literal_char", SYM_VAR);
        if($$) {
            set_var_type($$, TYPE_CHAR);
            set_var_value_char($$, $1);
            $$->is_constant = true;
        }
    }
    | STRING { 
        $$ = putsym("_literal_string", SYM_VAR);
        if ($$) {
            set_var_type($$, TYPE_STRING);
            
            set_var_value_string($$, $1);
            $$->is_constant = true;
        }
        free($1); 
    }
    | BOOL { 
        $$ = putsym("_literal_bool", SYM_VAR);
        if ($$) {
            set_var_type($$, TYPE_BOOL);
            set_var_value_bool($$, $1);
            $$->is_constant = true;
        }
    }
    ;


expression:
      literal 
    
    | IDENT ASSIGN expression {
        $$ = getsym($1);
        if (!$$) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Assignment to undeclared identifier '%s'", $1);
            yyerror(err_msg);
             $$ = putsym($1, SYM_VAR); 
             if($$) set_var_type($$, TYPE_UNDEFINED); 
             else $$ = NULL; 
        } else if ($$->kind != SYM_VAR) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Assignment target '%s' is not a variable", $1);
            yyerror(err_msg);
        } else if ($$->is_constant) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Assignment to constant variable '%s'", $1);
            yyerror(err_msg);
        } else if (!$3 || $3->type == TYPE_UNDEFINED) { 
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Cannot assign undefined value to '%s'", $1);
            yyerror(err_msg);
        } else if ($3->type != $$->type) {
            
            char err_msg[256];
            
            snprintf(err_msg, sizeof(err_msg), "Type mismatch in assignment to '%s' (expected %d, got %d)", $1, $$->type, $3->type);
            yyerror(err_msg);
        } else {
            switch ($$->type) {
                case TYPE_INT: set_var_value_int($$, $3->value.var.int_val); break;
                case TYPE_DOUBLE: set_var_value_double($$, $3->value.var.double_val); break;
                case TYPE_CHAR: set_var_value_char($$, $3->value.var.char_val); break;
                case TYPE_STRING:
                    
                    if ($$->value.var.string_val) free($$->value.var.string_val);
                     
                    set_var_value_string($$, $3->value.var.string_val ? $3->value.var.string_val : "");
                    break;
                case TYPE_BOOL: set_var_value_bool($$, $3->value.var.bool_val); break;
                default: yyerror("Unhandled type in assignment"); break;
             }
        }
        free($1); 
        
    }
    | IDENT { 
        $$ = getsym($1);
        if (!$$) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Undefined identifier '%s'", $1);
            yyerror(err_msg);
             $$ = putsym($1, SYM_VAR); 
             if($$) set_var_type($$, TYPE_UNDEFINED);
             else $$ = NULL; 
        } else if ($$->kind != SYM_VAR) { 
             char err_msg[256];
             snprintf(err_msg, sizeof(err_msg), "'%s' is not a variable", $1);
             yyerror(err_msg);
             
             
             if($$) set_var_type($$, TYPE_UNDEFINED); 
        }
        free($1); 
    }
    
    | LPAREN expression RPAREN { $$ = $2; }
    
    | expression PLUS expression  { handle_binop($1, $3, &$$, OP_ADD); }
    | expression MINUS expression { handle_binop($1, $3, &$$, OP_SUB); }
    | expression MULTIPLY expression { handle_binop($1, $3, &$$, OP_MUL); }
    | expression DIVIDE expression { handle_binop($1, $3, &$$, OP_DIV); }
    | MINUS expression %prec UNARY_MINUS { 
        if (!$2 || $2->type == TYPE_UNDEFINED) {
            yyerror("Cannot negate undefined value");
            $$ = putsym("_temp_error", SYM_VAR); 
            if($$) set_var_type($$, TYPE_UNDEFINED); else $$ = NULL;
        } else if ($2->type == TYPE_INT) {
            $$ = putsym("_temp_neg_int", SYM_VAR);
            if($$) {
                 set_var_type($$, TYPE_INT);
                 set_var_value_int($$, -$2->value.var.int_val);
                 $$->is_constant = $2->is_constant; 
            }
        } else if ($2->type == TYPE_DOUBLE) {
            $$ = putsym("_temp_neg_double", SYM_VAR);
             if($$) {
                 set_var_type($$, TYPE_DOUBLE);
                 set_var_value_double($$, -$2->value.var.double_val);
                 $$->is_constant = $2->is_constant;
             }
        } else {
            char err_msg[100]; snprintf(err_msg, sizeof(err_msg), "Invalid type (%d) for negation", $2->type); yyerror(err_msg);
            $$ = putsym("_temp_error", SYM_VAR); 
            if($$) set_var_type($$, TYPE_UNDEFINED); else $$ = NULL;
        }
        
    }
    | NOT expression { 
         yyerror("NOT operator not yet implemented");
         $$ = putsym("_temp_error", SYM_VAR);
         if($$) set_var_type($$, TYPE_UNDEFINED); else $$ = NULL;
    }
    ;

statements:
      %empty {} 
    | statements statement {} 
    ;

statement:
      expression SEMICOLON {
          if ($1 && (strncmp($1->name, "_literal", 8) == 0 || strncmp($1->name, "_temp", 5) == 0)) { 
              
          }
      }
    | function 
    | declaration_statement SEMICOLON 
    | print_statement
    | LCURLY statements RCURLY {} 
    
    ;


declaration_statement:
    
      IDENT COLON type ASSIGN expression {
        symrec* existing = getsym($1);
        if (existing) {
            
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Redeclaration of '%s'", $1);
            yyerror(err_msg);
        } else if (!$5 || $5->type == TYPE_UNDEFINED) {
             char err_msg[256];
             snprintf(err_msg, sizeof(err_msg), "Cannot initialize constant '%s' with undefined value", $1);
             yyerror(err_msg);
             
             symrec* sym = putsym($1, SYM_VAR);
             if(sym) { set_var_type(sym, $3); sym->is_constant = true; }
        } else if ($5->type != $3) {
             
            char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Type mismatch in constant '%s' initialization (expected %d, got %d)", $1, $3, $5->type); yyerror(err_msg);
             symrec* sym = putsym($1, SYM_VAR);
             if(sym) { set_var_type(sym, $3); sym->is_constant = true; }
        } else {
            symrec* sym = putsym($1, SYM_VAR);
            if(sym) {
                set_var_type(sym, $3);
                sym->is_constant = true; 
                switch (sym->type) { 
                    case TYPE_INT: set_var_value_int(sym, $5->value.var.int_val); break;
                    case TYPE_DOUBLE: set_var_value_double(sym, $5->value.var.double_val); break;
                    case TYPE_CHAR: set_var_value_char(sym, $5->value.var.char_val); break;
                    case TYPE_STRING: set_var_value_string(sym, $5->value.var.string_val ? $5->value.var.string_val : ""); break;
                    case TYPE_BOOL: set_var_value_bool(sym, $5->value.var.bool_val); break;
                    default: yyerror("Unhandled type in const initialization"); break;
                 }
            }
        }
        free($1); 
        
    }
    
    | VAR IDENT COLON type ASSIGN expression {
        symrec* existing = getsym($2);
         if (existing) {
            char err_msg[256];
            snprintf(err_msg, sizeof(err_msg), "Redeclaration of '%s'", $2);
            yyerror(err_msg);
         } else if (!$6 || $6->type == TYPE_UNDEFINED) {
             char err_msg[256];
             snprintf(err_msg, sizeof(err_msg), "Cannot initialize variable '%s' with undefined value", $2);
             yyerror(err_msg);
             symrec* sym = putsym($2, SYM_VAR);
             if(sym) { set_var_type(sym, $4); sym->is_constant = false; } 
         } else if ($6->type != $4) {
            char err_msg[256]; snprintf(err_msg, sizeof(err_msg), "Type mismatch in variable '%s' initialization (expected %d, got %d)", $2, $4, $6->type); yyerror(err_msg);
             symrec* sym = putsym($2, SYM_VAR);
             if(sym) { set_var_type(sym, $4); sym->is_constant = false; }
         } else {
            symrec* sym = putsym($2, SYM_VAR);
            if(sym) {
                set_var_type(sym, $4);
                sym->is_constant = false; 
                switch (sym->type) { 
                    case TYPE_INT: set_var_value_int(sym, $6->value.var.int_val); break;
                    case TYPE_DOUBLE: set_var_value_double(sym, $6->value.var.double_val); break;
                    case TYPE_CHAR: set_var_value_char(sym, $6->value.var.char_val); break;
                    case TYPE_STRING: set_var_value_string(sym, $6->value.var.string_val ? $6->value.var.string_val : ""); break;
                    case TYPE_BOOL: set_var_value_bool(sym, $6->value.var.bool_val); break;
                    default: yyerror("Unhandled type in var initialization"); break;
                 }
            }
        }
        free($2); 
        
    }
    ;

print_statement:
    PRINT LPAREN expression RPAREN SEMICOLON {
        if (!$3 || $3->type == TYPE_UNDEFINED) {
             yyerror("Cannot print undefined value");
        } else {
            switch($3->type) {
                case TYPE_INT: printf("%d\n", $3->value.var.int_val); break;
                case TYPE_DOUBLE: printf("%f\n", $3->value.var.double_val); break;
                case TYPE_CHAR: printf("%c\n", $3->value.var.char_val); break;
                
                case TYPE_STRING: printf("%s\n", $3->value.var.string_val ? $3->value.var.string_val : "(null)"); break;
                case TYPE_BOOL: printf("%s\n", $3->value.var.bool_val ? "true" : "false"); break;
                case TYPE_VOID: yyerror("Cannot print void value"); break; 
                default: yyerror("Invalid type for print"); 
            }
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