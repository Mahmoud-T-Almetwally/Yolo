%{
    #include "parser.tab.h" 
    #include "symtab.h"
    #include "ast.h"
    #include "utils.h"
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylineno;
    AstNode *ast_root = NULL;
    symrec* current_func_sym_building = NULL;

    void yyerror(YYLTYPE *llocp, void *scanner, const char *s);
    int yylex(YYSTYPE *yylval, YYLTYPE *yyloc, void *scanner);
%}

%code requires {
    #include "types.h" // Enums like data_type, OperatorType
    #include "symtab.h" // For symrec struct pointer
    #include "ast.h" // For AstNode struct pointer
    #include <stdbool.h> // For bool_val
}

%code {
    extern SymbolTable *current_scope; 
    extern int yylineno;
}

%parse-param { void *scanner }
%lex-param { void *scanner }    
%define parse.error verbose 
%define api.pure full
%locations

%destructor { free_ast($$); } <node_ptr> 

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

%token FUNC VAR IF ELSE WHILE FOR PRINT RETURN T_FLOAT // T_FLOAT might map to T_DOUBLE

%token <type_val> T_INTEGER T_CHAR T_STRING T_DOUBLE T_BOOL T_VOID

%token LPAREN RPAREN LCURLY RCURLY LBRACKET RBRACKET
%token DOT SEMICOLON COLON COMMA

%token <op_val> PLUS MINUS MULTIPLY DIVIDE POWER 
%token <op_val> EQ NEQ LT GT LTE GTE          
%token <op_val> AND OR NOT                   
%token <op_val> ASSIGN                  

// Tokens for potential future use (currently no rules use them)
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
// %precedence INC DEC // If used, likely higher than unary minus



%type <type_val> primitive_type return_type
%type <node_ptr> program statements statement block func_definition
%type <node_ptr> declaration_statement assignment_statement if_statement print_statement return_statement
%type <node_ptr> expression literal
%type <node_ptr> param_list param 
%type <node_ptr> arg_list         

%type <sym_ptr> func_signature

%start program

%%

program:
      /* empty */ { ast_root = create_program_node(NULL); $$ = ast_root; init_scope_management(); }
    | statements  { ast_root = create_program_node($1); $$ = ast_root; }
    ;

statements:
      /* empty */      { $$ = NULL; } // Base case: no statements
    | statement statements { // Build a linked list of statements
                      AstNode* first = $1;
                      AstNode* rest = $2;
                      if (first) {
                          first->next = rest;
                          $$ = first;
                      } else {
                           $$ = rest; // Skip the NULL statement (e.g., ';')
                      }
                   }
    ;

// A block introduces a new scope
block:
      LCURLY
          { enter_scope(NULL); } // Enter new scope before processing statements
      statements
      RCURLY
          { $$ = $3; exit_scope(); } // Statements list is the result; exit scope after processing
    ;

statement:
      declaration_statement SEMICOLON { $$ = $1; }
    | assignment_statement SEMICOLON  { $$ = $1; }
    | expression SEMICOLON            { $$ = $1; } // Allow expressions as statements (e.g., function calls)
    | if_statement                    { $$ = $1; } // No semicolon needed after if
    | print_statement SEMICOLON       { $$ = $1; }
    | return_statement SEMICOLON      { $$ = $1; }
    | func_definition                 { $$ = $1; } // No semicolon needed after function def
    | block                           { $$ = $1; } // No semicolon needed after block
    | SEMICOLON                       { $$ = NULL; } // Empty statement
    ;

func_definition:
    // Parse signature (creates symbol, enters scope, handles params)
    // Then parse the body block (which uses the function's scope)
    func_signature block
    {
        symrec* func_sym = $1; // Get the function symbol from the signature
        AstNode* body_node = $2; // Get the statement list from the block

        if (func_sym) {
            // Associate the parsed body AST with the function symbol
            set_func_body(func_sym, body_node);
            // Create the AST node representing the function definition itself
            $$ = create_func_def_node(func_sym, body_node);
            if ($$) $$->lineno = @1.first_line; // Set line number from 'FUNC' keyword
        } else {
            // If signature parsing failed, func_sym might be NULL
            PARSER_ERROR(@1.first_line, "Failed to create function symbol, discarding function body.");
            free_ast(body_node); // Clean up the orphaned body
            $$ = NULL;
        }
        // The scope for the function parameters and body was handled
        // by func_signature (enter) and block (exit), so no exit_scope here.
        // The scope *containing* the function definition is exited elsewhere (e.g., end of program/block)
        current_func_sym_building = NULL; // Clear the global tracker
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
    LPAREN param_list RPAREN return_type // Matched '(params) : retType'
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
            // free_ast(param_ast_list); // Maybe not free yet?
        } else {
            free_ast(param_ast_list);
        }

        $$ = func_sym;
    }
    ;

return_type:
      /* empty */         { $$ = TYPE_VOID; } // Default return type
    | COLON primitive_type { $$ = $2; }      // Explicit return type
    ;

// Parameter list: a list of parameter declarations
param_list:
      /* empty */ { $$ = NULL; }           // No parameters
    | param       { $$ = $1; $1->next = NULL; } // Single parameter
    | param COMMA param_list {             // Multiple parameters
              $1->next = $3; // Link current param to the rest of the list
              $$ = $1;       // Return the head of the list
      }
    ;

// A single parameter declaration (creates symbol and AST node)
param:
    // Allow 'var name: type' or just 'name: type' for params
    IDENT COLON primitive_type {
        if (!current_func_sym_building) { // Check if we are inside a function signature
             PARSER_ERROR(@1.first_line, "Parameter '%s' declared outside function signature.", $1);
             free($1);
             $$ = NULL; YYERROR;
        }
        // Parameters are like local variables within the function's scope
        symrec* param_sym = putsym($1, SYM_PARAM, $3);
        if (!param_sym) {
            PARSER_ERROR(@1.first_line, "Failed to declare parameter '%s' (possibly redefined).", $1);
            free($1);
            $$ = NULL; YYERROR;
        }
        free($1); // Free identifier string
        // Create a VAR_DECL node for the parameter (helps structure/consistency)
        $$ = create_decl_node(NODE_VAR_DECL, param_sym, NULL);
         if ($$) $$->lineno = @1.first_line;
         // Mark param as initialized implicitly upon function call
         mark_symbol_initialized(param_sym);
    }
    | VAR IDENT COLON primitive_type { // Optional 'var' keyword for params
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
        free($2); // Free identifier string
        $$ = create_decl_node(NODE_VAR_DECL, param_sym, NULL);
         if ($$) $$->lineno = @2.first_line;
         mark_symbol_initialized(param_sym);
    }
    ;

primitive_type:
      T_INTEGER { $$ = TYPE_INT; }
    | T_DOUBLE  { $$ = TYPE_DOUBLE; }
    | T_FLOAT   { $$ = TYPE_DOUBLE; } // Treat float as double
    | T_CHAR    { $$ = TYPE_CHAR; }
    | T_STRING  { $$ = TYPE_STRING; }
    | T_BOOL    { $$ = TYPE_BOOL; }
    | T_VOID    { $$ = TYPE_VOID; } // Only valid as return type
    ;

declaration_statement:
    // Constant declaration: name : type = expr; (Implicitly const)
    IDENT COLON primitive_type ASSIGN expression {
            symrec* sym = putsym($1, SYM_VAR, $3);
            if (!sym) {
                 PARSER_ERROR(@1.first_line, "Failed to declare constant '%s' (possibly redefined).", $1);
                 free($1); free_ast($5); $$ = NULL; YYERROR;
            }
            set_symbol_const(sym, true); // Mark as constant
            $$ = create_decl_node(NODE_CONST_DECL, sym, $5); // Use CONST_DECL node type
            if ($$) $$->lineno = @1.first_line;
             // Initialization happens via the assignment, mark in semantic pass? Or here?
             // Let's mark here for simplicity, semantic pass can verify type.
             mark_symbol_initialized(sym);
            free($1); // Free identifier string
    }
    // Variable declaration with initialization: var name : type = expr;
    | VAR IDENT COLON primitive_type ASSIGN expression {
            symrec* sym = putsym($2, SYM_VAR, $4);
             if (!sym) {
                 PARSER_ERROR(@2.first_line, "Failed to declare variable '%s' (possibly redefined).", $2);
                 free($2); free_ast($6); $$ = NULL; YYERROR;
            }
            // Default is not constant (is_constant = false)
            $$ = create_decl_node(NODE_VAR_DECL, sym, $6);
            if ($$) $$->lineno = @1.first_line;
             mark_symbol_initialized(sym);
            free($2); // Free identifier string
    }
    // Variable declaration without initialization: var name : type;
    | VAR IDENT COLON primitive_type {
            symrec* sym = putsym($2, SYM_VAR, $4);
             if (!sym) {
                 PARSER_ERROR(@2.first_line, "Failed to declare variable '%s' (possibly redefined).", $2);
                 free($2); $$ = NULL; YYERROR;
            }
            // is_initialized remains false
            $$ = create_decl_node(NODE_VAR_DECL, sym, NULL); // No initializer expression
            if ($$) $$->lineno = @1.first_line;
            free($2); // Free identifier string
    }
    ;

assignment_statement:
    // Simple assignment: name = expr;
    IDENT ASSIGN expression {
        // Lookup the symbol for the identifier
        symrec* sym = getsym($1);
        if (!sym) {
            // Undeclared variable error - Semantic analysis should catch this primarily
            // But we can report early here too.
            PARSER_ERROR(@1.first_line, "Assignment to undeclared identifier '%s'.", $1);
            free($1); free_ast($3); $$ = NULL; YYERROR;
        }
        // Create an identifier expression node for the LHS
        AstNode* ident_node = create_ident_node(sym);
        if (ident_node) ident_node->lineno = @1.first_line;

        // Create the assignment statement node
        $$ = create_assign_node(ident_node, $3);
         if ($$) $$->lineno = @1.first_line; // Or maybe @2.first_line for '=' ?
         // Semantic analysis will check if 'sym' is constant and if types match.
         // Mark symbol as initialized after assignment (if not already).
         // This might be better done in semantic analysis after type checks pass.
         // mark_symbol_initialized(sym); // Maybe defer this

        free($1); // Free identifier string
    }
    // Could add more complex assignments later (e.g., array[index] = expr)
    ;

if_statement:
    IF LPAREN expression RPAREN statement // Mandatory 'then' branch (can be a block)
       { $$ = create_if_node($3, $5, NULL); if($$) $$->lineno = @1.first_line; } // No else
    | IF LPAREN expression RPAREN statement ELSE statement // With 'else' branch
       { $$ = create_if_node($3, $5, $7); if($$) $$->lineno = @1.first_line; }
    // Note: 'statement' can be a 'block' due to statement rules
    ;

print_statement:
    // Allow print() or print(expr)
    PRINT LPAREN RPAREN { $$ = create_print_node(NULL); if($$) $$->lineno = @1.first_line; }
    | PRINT LPAREN expression RPAREN { $$ = create_print_node($3); if($$) $$->lineno = @1.first_line; }
    ;

return_statement:
    RETURN expression { $$ = create_return_node($2); if($$) $$->lineno = @1.first_line; }
    | RETURN           { $$ = create_return_node(NULL); if($$) $$->lineno = @1.first_line; } // Return void
    ;

literal:
      INTEGER { $$ = create_int_node($1); if($$) $$->lineno = @1.first_line; }
    | DOUBLE  { $$ = create_double_node($1); if($$) $$->lineno = @1.first_line;}
    | CHAR    { $$ = create_char_node($1); if($$) $$->lineno = @1.first_line;}
    | STRING  { $$ = create_string_node($1); free($1); if($$) $$->lineno = @1.first_line;} // Free lexer string after copying
    | BOOL    { $$ = create_bool_node($1); if($$) $$->lineno = @1.first_line;} // Need BOOL token from lexer
    // Add TRUE/FALSE tokens later maybe
    ;

// Argument list for function calls: a linked list of expressions
arg_list:
    /* empty */             { $$ = NULL; }
    | expression            { $$ = $1; $1->next = NULL; } // Single argument
    | expression COMMA arg_list { // Multiple arguments
          $1->next = $3; // Link current argument expression to the rest
          $$ = $1;       // Return the head of the list
      }
    ;


expression:
      literal { $$ = $1; } // Literals are expressions
    | IDENT   { // Identifier used in an expression (variable lookup)
          symrec* sym = getsym($1);
          if (!sym) {
               PARSER_ERROR(@1.first_line, "Undeclared identifier '%s' used in expression.", $1);
               free($1); $$ = NULL; YYERROR;
          }
          // Create identifier expression node, semantic analysis checks initialization/type
          $$ = create_ident_node(sym);
          if ($$) $$->lineno = @1.first_line;
          free($1); // Free identifier string
      }
    // Binary Operators
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
    // Unary Operators
    | MINUS expression %prec UNARY_MINUS { $$ = create_unop_node(OP_NEGATE, $2); if($$) $$->lineno = @1.first_line;}
    | NOT expression                 { $$ = create_unop_node(OP_NOT, $2); if($$) $$->lineno = @1.first_line;}
    // Parenthesized expression
    | LPAREN expression RPAREN       { $$ = $2; /* Just pass the inner expression's node */ }
    // Function Call: funcName ( arg_list )
    | IDENT LPAREN arg_list RPAREN {
         // Find the function symbol
         symrec* func_sym = getsym($1);
         if (!func_sym) {
              PARSER_ERROR(@1.first_line, "Call to undeclared function '%s'.", $1);
              free($1); free_ast($3); $$ = NULL; YYERROR;
         }
         // Semantic analysis will check if it *is* a function and if args match.
         $$ = create_func_call_node(func_sym, $3); // $3 is the head of the arg expression list
         if ($$) $$->lineno = @1.first_line;
         free($1); 
    }
    ;

%%
void yyerror(YYLTYPE *llocp, void *scanner, const char *s) {
    int lineno = llocp ? llocp->first_line : 0; 
    compiler_error(lineno, "Syntax Error: %s", s);
} 