%{
    #include "parser.tab.h" 
    #include "symtab.h"
    #include "ast.h"
    #include "utils.h"
    #include "globals.h"
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylineno;
    symrec* current_func_sym_building = NULL;

    void yyerror(YYLTYPE *llocp, void *scanner, const char *s);
    int yylex(YYSTYPE *yylval, YYLTYPE *yyloc, void *scanner);
%}

%code requires {
    #include "types.h" 
    #include "symtab.h" 
    #include "ast.h" 
    #include <stdbool.h> 
}

%code {
    extern int yylineno;
}

%parse-param { void *scanner }
%lex-param { void *scanner }    
%define parse.error verbose 
%define api.pure full
%locations

%destructor {
    if ($$ != ast_root) {
        if(debug) {
            printf("DEBUG: Destructor freeing node %p (type %d)\n", $$, $$ ? $$->node_type : -1);
        }
        free_ast($$);
    } else {
        if(debug) {
            printf("DEBUG: Destructor skipping free for ast_root %p\n", $$); 
        } 
    }
} <node_ptr>

%union {
    int int_val;
    double double_val;
    char* string_val;
    char char_val;
    bool bool_val;
    symrec* sym_ptr; 
    data_type type_val; 
    AstNode* node_ptr; 
    OperatorType op_val; 
}

%token <string_val> IDENT       
%token <int_val> INTEGER   
%token <double_val> DOUBLE   
%token <char_val> CHAR   
%token <string_val> STRING     
%token <bool_val> BOOL        

%token FUNC VAR IF ELSE WHILE FOR PRINT RETURN T_FLOAT 

%token <type_val> T_INTEGER T_CHAR T_STRING T_DOUBLE T_BOOL T_VOID

%token LPAREN RPAREN LCURLY RCURLY LBRACKET RBRACKET
%token DOT SEMICOLON COLON COMMA

%token <op_val> PLUS MINUS MULTIPLY DIVIDE POWER 
%token <op_val> EQ NEQ LT GT LTE GTE          
%token <op_val> AND OR NOT                   
%token <op_val> ASSIGN                  


%token INC DEC


%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER 
%left NOT    

%precedence UNARY_MINUS




%type <type_val> primitive_type return_type
%type <node_ptr> statements statement block func_definition
%type <node_ptr> declaration_statement assignment_statement if_statement print_statement return_statement
%type <node_ptr> expression literal
%type <node_ptr> param_list param 
%type <node_ptr> arg_list         

%type <sym_ptr> func_signature

%start program

%%

program:
      /* empty */ {
                    if (debug) {
                        printf("Parser Action: program: <empty> matched.\n");
                    }

                    ast_root = create_program_node(NULL);
                    init_scope_management();

                    if (debug) {
                        printf("Parser Action: program: <empty> created program node %p, assigned to ast_root.\n", ast_root);
                    }
                  }
    | statements  {
                    if (debug) {
                        printf("Parser Action: program: statements matched.\n");
                    }

                    AstNode* statement_list = $1;

                    if (debug) {
                        printf("Parser Action: program: statements matched, statement_list = %p\n", statement_list);
                    };

                    AstNode* new_program_node = create_program_node(statement_list);
                    if (debug) {
                        printf("Parser Action: program: create_program_node(%p) returned %p\n", statement_list, new_program_node);
                    }

                    if (new_program_node) {
                        if (debug) {
                            printf("Parser Action: new_program_node->node_type = %d (%s) - Expected: %d (%s)\n",
                                new_program_node->node_type, node_type_to_string_parser(new_program_node->node_type),
                                NODE_PROGRAM, node_type_to_string_parser(NODE_PROGRAM));
                        }
                         
                        ast_root = new_program_node;
                        if (debug) {
                            printf("Parser Action: Assigned %p to global ast_root.\n", ast_root);
                        }

                    } else {
                        if (debug) {
                            printf("Parser Action: create_program_node returned NULL!\n");
                        }
                        ast_root = NULL;
                        YYERROR;
                    }
                    
                    
                 }
    ;

statements: 
      /* empty */      { $$ = NULL; }
    | statement statements { 
                      AstNode* first = $1;
                      AstNode* rest = $2;
                      if (first) {
                          AstNode* current = first;
                          while (current->next != NULL) current = current->next;
                          current->next = rest;
                          $$ = first;
                      } else {
                          $$ = rest;
                      }
                   }
    ;

block:
      LCURLY
          { enter_scope(NULL); } 
      statements
      RCURLY
          { $$ = $3; exit_scope(); } 
    ;

statement:
      declaration_statement SEMICOLON { $$ = $1; }
    | assignment_statement SEMICOLON  { $$ = $1; }
    | expression SEMICOLON            { $$ = $1; } 
    | if_statement                    { $$ = $1; } 
    | print_statement SEMICOLON       { $$ = $1; }
    | return_statement SEMICOLON      { $$ = $1; }
    | func_definition                 { $$ = $1; } 
    | block                           { $$ = $1; } 
    | SEMICOLON                       { $$ = NULL; } 
    ;

func_definition:
    
    
    func_signature block
    {
        symrec* func_sym = $1; 
        AstNode* body_node = $2; 

        if (func_sym) {
            
            set_func_body(func_sym, body_node);
            
            $$ = create_func_def_node(func_sym, body_node);
            if ($$) $$->lineno = @1.first_line; 
        } else {
            
            PARSER_ERROR(@1.first_line, "Failed to create function symbol, discarding function body.");
            free_ast(body_node); 
            $$ = NULL;
        }
        
        
        
        current_func_sym_building = NULL; 
    }
    ;

func_signature:
    FUNC IDENT 
    {
        if (getsym_in_scope($2, current_scope)) {
             PARSER_ERROR(@2.first_line, "Function '%s' redefined in the current scope.", $2);
             free($2); 
             current_func_sym_building = NULL;
             YYERROR; 
        }
        current_func_sym_building = putsym($2, SYM_FUNC, TYPE_VOID);
        if (!current_func_sym_building) {
             PARSER_ERROR(@2.first_line, "Failed to create symbol table entry for function '%s'.", $2);
             free($2);
             YYERROR;
        }
        enter_scope($2); 
        free($2); 
    }
    LPAREN param_list RPAREN return_type 
    {
        symrec* func_sym = current_func_sym_building;
        AstNode* param_ast_list = $5;
        data_type ret_type = $7;

        if (func_sym) {
            set_symbol_type(func_sym, ret_type);

            AstNode* current_param_node = param_ast_list;
            while (current_param_node != NULL) {
                 if (current_param_node->node_type == NODE_VAR_DECL &&
                     current_param_node->data.declaration.symbol &&
                     current_param_node->data.declaration.symbol->kind == SYM_PARAM)
                 {
                      add_func_param(func_sym, current_param_node->data.declaration.symbol);
                 } else {
                      PARSER_ERROR(current_param_node ? current_param_node->lineno : @5.first_line,
                                   "Internal Error: Expected parameter declaration node, got something else.");
                 }
                 current_param_node = current_param_node->next;
            }
            
        } else {
            free_ast(param_ast_list);
        }

        $$ = func_sym;
    }
    ;

return_type:
      /* empty */         { $$ = TYPE_VOID; } 
    | COLON primitive_type { $$ = $2; }      
    ;


param_list:
      /* empty */ { $$ = NULL; }           
    | param       { $$ = $1; $1->next = NULL; } 
    | param COMMA param_list {             
              $1->next = $3; 
              $$ = $1;       
      }
    ;


param:
    
    IDENT COLON primitive_type {
        if (!current_func_sym_building) { 
             PARSER_ERROR(@1.first_line, "Parameter '%s' declared outside function signature.", $1);
             free($1);
             $$ = NULL; YYERROR;
        }
        
        symrec* param_sym = putsym($1, SYM_PARAM, $3);
        if (!param_sym) {
            PARSER_ERROR(@1.first_line, "Failed to declare parameter '%s' (possibly redefined).", $1);
            free($1);
            $$ = NULL; YYERROR;
        }
        free($1); 
        
        $$ = create_decl_node(NODE_VAR_DECL, param_sym, NULL);
         if ($$) $$->lineno = @1.first_line;
         
         mark_symbol_initialized(param_sym);
    }
    | VAR IDENT COLON primitive_type { 
         if (!current_func_sym_building) {
             PARSER_ERROR(@2.first_line, "Parameter '%s' declared outside function signature.", $2);
             free($2);
             $$ = NULL; YYERROR;
         }
        symrec* param_sym = putsym($2, SYM_PARAM, $4);
         if (!param_sym) {
            PARSER_ERROR(@2.first_line, "Failed to declare parameter '%s' (possibly redefined).", $2);
            free($2);
            $$ = NULL; YYERROR;
        }
        free($2); 
        $$ = create_decl_node(NODE_VAR_DECL, param_sym, NULL);
         if ($$) $$->lineno = @2.first_line;
         mark_symbol_initialized(param_sym);
    }
    ;

primitive_type:
      T_INTEGER { $$ = TYPE_INT; }
    | T_DOUBLE  { $$ = TYPE_DOUBLE; }
    | T_FLOAT   { $$ = TYPE_DOUBLE; } 
    | T_CHAR    { $$ = TYPE_CHAR; }
    | T_STRING  { $$ = TYPE_STRING; }
    | T_BOOL    { $$ = TYPE_BOOL; }
    | T_VOID    { $$ = TYPE_VOID; } 
    ;

declaration_statement:
    
    IDENT COLON primitive_type ASSIGN expression {
            symrec* sym = putsym($1, SYM_VAR, $3);
            if (!sym) {
                 PARSER_ERROR(@1.first_line, "Failed to declare constant '%s' (possibly redefined).", $1);
                 free($1); free_ast($5); $$ = NULL; YYERROR;
            }
            set_symbol_const(sym, true); 
            $$ = create_decl_node(NODE_CONST_DECL, sym, $5); 
            if ($$) $$->lineno = @1.first_line;
             
             
             mark_symbol_initialized(sym);
            free($1); 
    }
    
    | VAR IDENT COLON primitive_type ASSIGN expression {
            symrec* sym = putsym($2, SYM_VAR, $4);
             if (!sym) {
                 PARSER_ERROR(@2.first_line, "Failed to declare variable '%s' (possibly redefined).", $2);
                 free($2); free_ast($6); $$ = NULL; YYERROR;
            }
            
            $$ = create_decl_node(NODE_VAR_DECL, sym, $6);
            if ($$) $$->lineno = @1.first_line;
             mark_symbol_initialized(sym);
            free($2); 
    }
    | VAR IDENT COLON primitive_type {
            symrec* sym = putsym($2, SYM_VAR, $4);
             if (!sym) {
                 PARSER_ERROR(@2.first_line, "Failed to declare variable '%s' (possibly redefined).", $2);
                 free($2); $$ = NULL; YYERROR;
            }
            $$ = create_decl_node(NODE_VAR_DECL, sym, NULL); 
            if ($$) $$->lineno = @1.first_line;
            free($2); 
    }
    ;

assignment_statement:
    IDENT ASSIGN expression {
        symrec* sym = getsym($1);
        if (!sym) {
            PARSER_ERROR(@1.first_line, "Assignment to undeclared identifier '%s'.", $1);
            free($1); free_ast($3); $$ = NULL; YYERROR;
        }
        AstNode* ident_node = create_ident_node(sym);
        if (ident_node) ident_node->lineno = @1.first_line;

        $$ = create_assign_node(ident_node, $3);
         if ($$) $$->lineno = @1.first_line; 

        free($1); 
    }
    ;

if_statement:
    IF LPAREN expression RPAREN statement
       { $$ = create_if_node($3, $5, NULL); if($$) $$->lineno = @1.first_line; } 
    | IF LPAREN expression RPAREN statement ELSE statement 
       { $$ = create_if_node($3, $5, $7); if($$) $$->lineno = @1.first_line; }
    ;

print_statement:
    PRINT LPAREN RPAREN { $$ = create_print_node(NULL); if($$) $$->lineno = @1.first_line; }
    | PRINT LPAREN expression RPAREN { $$ = create_print_node($3); if($$) $$->lineno = @1.first_line; }
    ;

return_statement:
    RETURN expression { $$ = create_return_node($2); if($$) $$->lineno = @1.first_line; }
    | RETURN           { $$ = create_return_node(NULL); if($$) $$->lineno = @1.first_line; } 
    ;

literal:
      INTEGER { $$ = create_int_node($1); if($$) $$->lineno = @1.first_line; }
    | DOUBLE  { $$ = create_double_node($1); if($$) $$->lineno = @1.first_line;}
    | CHAR    { $$ = create_char_node($1); if($$) $$->lineno = @1.first_line;}
    | BOOL    { $$ = create_bool_node($1); if($$) $$->lineno = @1.first_line;} 
    | STRING  {
                if (!$1) {
                    compiler_error(@1.first_line, "Internal Error: Failed to process string literal content (likely memory allocation).");
                    $$ = NULL;
                    YYERROR;
                }
                $$ = create_string_node($1);
                free($1);                    
                if($$) $$->lineno = @1.first_line;
                else { YYERROR; }
            }
    ;


arg_list:
    /* empty */             { $$ = NULL; }
    | expression            { $$ = $1; $1->next = NULL; }
    | expression COMMA arg_list {
          $1->next = $3;
          $$ = $1;      
      }
    ;


expression:
      literal { $$ = $1; }
    | IDENT   {
          symrec* sym = getsym($1);
          if (!sym) {
               PARSER_ERROR(@1.first_line, "Undeclared identifier '%s' used in expression.", $1);
               free($1); $$ = NULL; YYERROR;
          }
          $$ = create_ident_node(sym);
          if ($$) $$->lineno = @1.first_line;
          free($1); 
      }
    | expression PLUS expression     { $$ = create_binop_node(OP_PLUS, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression MINUS expression    { $$ = create_binop_node(OP_MINUS, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression MULTIPLY expression { $$ = create_binop_node(OP_MUL, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression DIVIDE expression   { $$ = create_binop_node(OP_DIV, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression POWER expression    { $$ = create_binop_node(OP_POWER, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression EQ expression       { $$ = create_binop_node(OP_EQ, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression NEQ expression      { $$ = create_binop_node(OP_NEQ, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression LT expression       { $$ = create_binop_node(OP_LT, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression GT expression       { $$ = create_binop_node(OP_GT, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression LTE expression      { $$ = create_binop_node(OP_LTE, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression GTE expression      { $$ = create_binop_node(OP_GTE, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression AND expression      { $$ = create_binop_node(OP_AND, $1, $3); if($$) $$->lineno = @1.first_line;}
    | expression OR expression       { $$ = create_binop_node(OP_OR, $1, $3); if($$) $$->lineno = @1.first_line;}
    | MINUS expression %prec UNARY_MINUS { $$ = create_unop_node(OP_NEGATE, $2); if($$) $$->lineno = @1.first_line;}
    | NOT expression                 { $$ = create_unop_node(OP_NOT, $2); if($$) $$->lineno = @1.first_line;}
    | LPAREN expression RPAREN       { $$ = $2; }

    | IDENT LPAREN arg_list RPAREN {

         symrec* func_sym = getsym($1);
         if (!func_sym) {
              PARSER_ERROR(@1.first_line, "Call to undeclared function '%s'.", $1);
              free($1); free_ast($3); $$ = NULL; YYERROR;
         }

         $$ = create_func_call_node(func_sym, $3);
         if ($$) $$->lineno = @1.first_line;
         free($1); 
    }
    ;

%%
void yyerror(YYLTYPE *llocp, void *scanner, const char *s) {
    int lineno = llocp ? llocp->first_line : 0; 
    compiler_error(lineno, "Syntax Error: %s", s);
} 