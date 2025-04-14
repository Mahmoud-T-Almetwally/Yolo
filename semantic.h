// semantic.h
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symtab.h"
#include <stdbool.h>

// --- Global State (Optional) ---
// Keep track of the function currently being analyzed (for return statement checks)
extern symrec* current_analyzing_function;
extern int semantic_error_count; // Track if errors occurred

// --- Main Analysis Function ---
// Performs semantic checks on the entire AST.
// Returns true if analysis is successful (no errors), false otherwise.
bool analyze_semantics(AstNode *ast_root);

// --- Helper Functions (Potentially exposed for testing or modularity) ---
// These traverse specific node types, perform checks, and determine expression types.
// They typically return the data_type of the expression or TYPE_VOID/TYPE_UNDEFINED.

data_type check_program(AstNode *node);
data_type check_statements(AstNode *node); // Check a list of statements
data_type check_statement(AstNode *node);  // Check a single statement

data_type check_func_def(AstNode *node);
data_type check_declaration(AstNode *node);
data_type check_assignment(AstNode *node);
data_type check_if_stmt(AstNode *node);
data_type check_print_stmt(AstNode *node);
data_type check_return_stmt(AstNode *node);
// Add checks for while, for loops later

// Expression checks return the resulting type of the expression
data_type check_expression(AstNode *node);
data_type check_binop_expr(AstNode *node);
data_type check_unop_expr(AstNode *node);
data_type check_func_call(AstNode *node);
data_type check_ident_expr(AstNode *node);
data_type check_literal(AstNode *node);

// --- Type Checking Helpers ---
bool check_type_compatibility(data_type required, data_type provided, bool allow_implicit_cast);
const char* type_to_string(data_type t); // Link to utility function

#endif // SEMANTIC_H