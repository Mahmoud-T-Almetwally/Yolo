// ast.h
#ifndef AST_H
#define AST_H

#include "types.h"
#include "symtab.h" // Include symtab header which forward declares AstNode
#include <stdbool.h>
#include <stdlib.h>

// --- AST Node Structure ---
typedef struct AstNode {
    NodeType node_type;
    int lineno;             // Line number where the node starts
    struct AstNode *next;   // For linking statements, arguments, parameters
    data_type expr_type;    // *** ADDED: To store type after semantic analysis ***

    union {
        // Non-Expression Nodes (Statements, Definitions, etc.)
        struct { struct AstNode *statements; } program;
        struct { struct symrec *symbol; struct AstNode *body; } func_def;
        struct { struct symrec *symbol; struct AstNode *initializer; } declaration; // For VAR and CONST
        struct { struct AstNode *lvalue; struct AstNode *rvalue; } assignment;
        struct { struct AstNode *condition; struct AstNode *then_branch; struct AstNode *else_branch; } if_stmt;
        struct { struct AstNode *expression; } print_stmt;
        struct { struct AstNode *expression; } return_stmt;
        // Could add While, For nodes here later

        // Expression Nodes
        struct { OperatorType op; struct AstNode *left; struct AstNode *right; } binop_expr;
        struct { OperatorType op; struct AstNode *operand; } unop_expr;
        struct { struct symrec* symbol; struct AstNode* arguments; } func_call; // 'arguments' is the head of the arg list
        struct { struct symrec *symbol; } ident_expr;

        // Literal Nodes
        int int_literal;
        double double_literal;
        char *string_literal;
        char char_literal;
        bool bool_literal;
    } data;
} AstNode;

// AST creation function prototypes
AstNode* create_program_node(AstNode* stmts);
AstNode* create_func_def_node(struct symrec* sym, AstNode* body);
// Removed create_args_node and append_arg_node
AstNode* create_decl_node(NodeType type, struct symrec* sym, AstNode* init);
AstNode* create_assign_node(AstNode* lval, AstNode* rval);
AstNode* create_if_node(AstNode* cond, AstNode* then_b, AstNode* else_b);
AstNode* create_print_node(AstNode* expr);
AstNode* create_return_node(AstNode* expr);
AstNode* create_binop_node(OperatorType op, AstNode* left, AstNode* right);
AstNode* create_unop_node(OperatorType op, AstNode* operand);
AstNode* create_func_call_node(struct symrec* func_sym, AstNode* args); // args is now the first expression node
AstNode* create_ident_node(struct symrec* var_sym);
AstNode* create_int_node(int value);
AstNode* create_double_node(double value);
AstNode* create_string_node(const char* value);
AstNode* create_char_node(char value);
AstNode* create_bool_node(bool value);

// AST memory management
void free_ast(AstNode *node);

#endif // AST_H