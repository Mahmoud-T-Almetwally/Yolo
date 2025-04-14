// semantic.c
#include "semantic.h"
#include "utils.h" // For error reporting and type_to_string etc.
#include <stdio.h>
#include <string.h> // For strcmp
#include <stdbool.h> // Make sure bool is available

// --- Global State ---
symrec* current_analyzing_function = NULL; // Track function for return checks
int semantic_error_count = 0; // Count errors during analysis

// --- SEM_ERROR macro removed ---

// --- Type Compatibility Check ---
// Basic check, can be expanded for casting rules
bool check_type_compatibility(data_type required, data_type provided, bool allow_implicit_cast) {
    if (required == provided) {
        return true; // Exact match
    }
    if (required == TYPE_VOID || provided == TYPE_VOID) {
        return false; // Cannot assign to/from void (except specific cases like return;)
    }
     if (required == TYPE_UNDEFINED || provided == TYPE_UNDEFINED) {
        return false; // Error type is not compatible
    }

    // Example implicit cast: int to double
    if (allow_implicit_cast) {
        if (required == TYPE_DOUBLE && provided == TYPE_INT) {
            return true;
        }
        // Add other casting rules here if needed (e.g., char to int)
    }

    return false; // Not compatible
}

// Utility to get type string
const char* type_to_string(data_type t) {
    return data_type_to_string(t); // Use the one from utils
}

// Helper function (you might need to add this to utils.c/h or keep static here)
static const char* node_type_to_string(NodeType type) {
     switch(type) {
         case NODE_PROGRAM: return "NODE_PROGRAM";
         case NODE_FUNC_DEF: return "NODE_FUNC_DEF";
         case NODE_VAR_DECL: return "NODE_VAR_DECL";
         case NODE_CONST_DECL: return "NODE_CONST_DECL";
         case NODE_ASSIGN_STMT: return "NODE_ASSIGN_STMT";
         case NODE_IF_STMT: return "NODE_IF_STMT";
         case NODE_PRINT_STMT: return "NODE_PRINT_STMT";
         case NODE_RETURN_STMT: return "NODE_RETURN_STMT";
         case NODE_BINOP_EXPR: return "NODE_BINOP_EXPR";
         case NODE_UNOP_EXPR: return "NODE_UNOP_EXPR";
         case NODE_FUNC_CALL: return "NODE_FUNC_CALL";
         case NODE_IDENT_EXPR: return "NODE_IDENT_EXPR";
         case NODE_INT_LITERAL: return "NODE_INT_LITERAL";
         case NODE_DOUBLE_LITERAL: return "NODE_DOUBLE_LITERAL";
         case NODE_STRING_LITERAL: return "NODE_STRING_LITERAL";
         case NODE_CHAR_LITERAL: return "NODE_CHAR_LITERAL";
         case NODE_BOOL_LITERAL: return "NODE_BOOL_LITERAL";
         default: return "NODE_UNKNOWN";
     }
}


// --- Main Analysis Function ---
bool analyze_semantics(AstNode *ast_root) {
    semantic_error_count = 0; // Reset error count
    current_analyzing_function = NULL; // Reset function context

    if (!ast_root) {
        // Empty input likely, semantic analysis trivially passes
        return true;
    }
     if (ast_root->node_type != NODE_PROGRAM) {
        // Report error using the node's line number
        compiler_error(ast_root->lineno, "Internal Error: Root AST node is not a program node.");
        semantic_error_count++;
        return false; // Definitely an error if non-null and wrong type
    }

    // Start traversal from the program's statements
    check_program(ast_root); // Safe to pass non-NULL ast_root here

    // Check if any function remained unfinished (shouldn't happen with current parser)
    if (current_analyzing_function != NULL) {
         // Report error with line number 0 as there's no specific node
         compiler_error(0, "Internal Error: Semantic analysis finished inside function '%s' scope.", current_analyzing_function->name);
         semantic_error_count++;
    }

    return semantic_error_count == 0; // Success if no errors were counted
}

// --- Node Check Functions ---

data_type check_program(AstNode *node) {
    if (!node || node->node_type != NODE_PROGRAM) return TYPE_UNDEFINED;
    // Symbol table should be initialized before this (e.g., in parser start or main)
    check_statements(node->data.program.statements);
    return TYPE_VOID; // Program itself doesn't have a type
}

data_type check_statements(AstNode *node) {
    AstNode *current = node;
    while (current != NULL) {
        check_statement(current);
        current = current->next; // Move to the next statement in the list
    }
    return TYPE_VOID; // Statement lists don't have a type
}

data_type check_statement(AstNode *node) {
    if (!node) return TYPE_VOID; // Handle NULL node (e.g., from empty semicolon)

    // Set node type initially, specific checks might refine it
    node->expr_type = TYPE_VOID; // Statements generally don't evaluate to a value

    switch (node->node_type) {
        case NODE_FUNC_DEF:
            check_func_def(node);
            break;
        case NODE_VAR_DECL:
        case NODE_CONST_DECL:
            check_declaration(node);
            break;
        case NODE_ASSIGN_STMT:
            check_assignment(node);
            break;
        case NODE_IF_STMT:
            check_if_stmt(node);
            break;
        case NODE_PRINT_STMT:
            check_print_stmt(node);
            break;
        case NODE_RETURN_STMT:
            check_return_stmt(node);
            break;
        // Expression as statement (e.g., function call)
        case NODE_FUNC_CALL: // Check the expression itself
        case NODE_BINOP_EXPR: // Less common as statements, but possible
        case NODE_UNOP_EXPR:
             // Check the expression, its type is stored in the node but usually ignored here
             check_expression(node);
             node->expr_type = TYPE_VOID; // Statement context results in void overall
            break;
        // Other node types that might appear in a statement list
        case NODE_PROGRAM: // Should not be nested
        case NODE_IDENT_EXPR: // Identifier alone is not a valid statement
        case NODE_INT_LITERAL: // Literal alone is not a valid statement
        case NODE_DOUBLE_LITERAL:
        case NODE_STRING_LITERAL:
        case NODE_CHAR_LITERAL:
        case NODE_BOOL_LITERAL:
             compiler_error(node->lineno, "Invalid statement. Expression, declaration, or control structure expected.");
             semantic_error_count++;
             node->expr_type = TYPE_UNDEFINED;
             break;
        case NODE_UNKNOWN:
        default:
            compiler_error(node->lineno, "Internal Error: Unknown AST node type %d encountered in check_statement.", node->node_type);
            semantic_error_count++;
            node->expr_type = TYPE_UNDEFINED;
            break;
    }
    // The type of the statement itself is usually VOID
    return TYPE_VOID; // Return VOID for statements
}

data_type check_func_def(AstNode *node) {
     if (!node || node->node_type != NODE_FUNC_DEF) return TYPE_UNDEFINED;
     symrec *func_sym = node->data.func_def.symbol;
     AstNode *body = node->data.func_def.body;

     if (!func_sym || func_sym->kind != SYM_FUNC) {
         compiler_error(node->lineno, "Internal Error: Function definition node without valid function symbol.");
         semantic_error_count++;
         return TYPE_UNDEFINED;
     }

     // Check for nested functions if not allowed (depends on language spec)
     if (current_analyzing_function != NULL) {
         compiler_error(node->lineno, "Nested function definitions are not supported ('%s' defined within '%s').", func_sym->name, current_analyzing_function->name);
         semantic_error_count++;
         // Continue checking body anyway? Or return error? Let's continue for more errors.
     }

     // --- Enter Function Context ---
     symrec* previous_function = current_analyzing_function;
     current_analyzing_function = func_sym;

     // --- Check Function Body ---
     if (body) {
          check_statements(body); // Check the statements within the function body
     }
     // else if (func_sym->type != TYPE_VOID) { // Optional check for missing body in non-void
     //    compiler_error(node->lineno, "Non-void function '%s' has no body.", func_sym->name);
     //    semantic_error_count++;
     // }

     // --- Exit Function Context ---
     current_analyzing_function = previous_function; // Restore outer context

     node->expr_type = TYPE_VOID; // Function definition itself is not an expression
     return TYPE_VOID;
}

data_type check_declaration(AstNode *node) {
     if (!node || (node->node_type != NODE_VAR_DECL && node->node_type != NODE_CONST_DECL)) return TYPE_UNDEFINED;

     symrec *sym = node->data.declaration.symbol;
     AstNode *initializer = node->data.declaration.initializer;

     if (!sym) {
         compiler_error(node->lineno, "Internal Error: Declaration node missing symbol.");
         semantic_error_count++;
         return TYPE_UNDEFINED;
     }

     data_type declared_type = sym->type;
     data_type init_type = TYPE_UNDEFINED;

     if (initializer) {
         // Check the initializer expression first
         init_type = check_expression(initializer);

         // Check if initializer type is compatible with the declared type
         // Only perform check if init_type wasn't already an error
         if (init_type != TYPE_UNDEFINED && !check_type_compatibility(declared_type, init_type, true)) { // Allow implicit casts
              compiler_error(initializer->lineno, "Initializer type '%s' is not compatible with declared type '%s' for '%s'.",
                             type_to_string(init_type), type_to_string(declared_type), sym->name);
              semantic_error_count++;
              sym->is_initialized = false; // Mark as not properly initialized
         } else if (init_type != TYPE_UNDEFINED) {
             // Type is compatible, mark symbol as initialized
             mark_symbol_initialized(sym);
         } else {
             // Initializer itself had an error, mark as not initialized
             sym->is_initialized = false;
         }
     } else {
         // No initializer
         if (node->node_type == NODE_CONST_DECL) {
             compiler_error(node->lineno, "Constant '%s' must be initialized.", sym->name);
             semantic_error_count++;
         }
         // Variable declared without initializer remains uninitialized (is_initialized = false)
     }

     // Check for VOID type variable/constant (disallowed)
     if (declared_type == TYPE_VOID) {
         compiler_error(node->lineno, "Cannot declare variable or constant '%s' with type 'void'.", sym->name);
         semantic_error_count++;
         sym->type = TYPE_UNDEFINED; // Mark symbol type as error
     }

     node->expr_type = TYPE_VOID; // Declaration is not an expression
     return TYPE_VOID;
}


data_type check_assignment(AstNode *node) {
     if (!node || node->node_type != NODE_ASSIGN_STMT) return TYPE_UNDEFINED;

     AstNode *lvalue = node->data.assignment.lvalue;
     AstNode *rvalue = node->data.assignment.rvalue;

     if (!lvalue || !rvalue) {
         compiler_error(node->lineno, "Internal Error: Assignment node missing lvalue or rvalue.");
         semantic_error_count++;
         return TYPE_UNDEFINED;
     }

     // --- Check L-Value ---
     if (lvalue->node_type != NODE_IDENT_EXPR) {
          compiler_error(lvalue->lineno, "Invalid left-hand side of assignment (must be an identifier for now).");
          semantic_error_count++;
          check_expression(rvalue); // Still check RHS for errors
          return TYPE_UNDEFINED;
     }

     symrec *sym = lvalue->data.ident_expr.symbol;
     if (!sym) {
          compiler_error(lvalue->lineno, "Internal Error: Assignment lvalue has NULL symbol pointer.");
          semantic_error_count++;
          check_expression(rvalue);
          return TYPE_UNDEFINED;
     }

     bool assign_error = false; // Track if assignment itself is illegal
     // Check if assigning to a constant
     if (sym->is_constant) {
          int decl_line = sym->body_ast ? sym->body_ast->lineno : 0;
          compiler_error(lvalue->lineno, "Cannot assign to constant '%s'%s%d%s.",
                         sym->name,
                         (decl_line > 0 ? " (declared on line " : ""),
                         (decl_line > 0 ? decl_line : 0),
                         (decl_line > 0 ? ")" : ""));
          semantic_error_count++;
          assign_error = true;
     }

     // Check if assigning to a function name
     if (sym->kind == SYM_FUNC) {
         compiler_error(lvalue->lineno, "Cannot assign to function name '%s'.", sym->name);
         semantic_error_count++;
         assign_error = true;
     }

     data_type lvalue_type = sym->type;

     // --- Check R-Value ---
     data_type rvalue_type = check_expression(rvalue);

     // --- Check Type Compatibility ---
     // Only check if rvalue didn't have an error and lvalue isn't an error type
     if (rvalue_type != TYPE_UNDEFINED && lvalue_type != TYPE_UNDEFINED &&
         !check_type_compatibility(lvalue_type, rvalue_type, true)) {
         compiler_error(rvalue->lineno, "Type mismatch: Cannot assign type '%s' to variable '%s' of type '%s'.",
                        type_to_string(rvalue_type), sym->name, type_to_string(lvalue_type));
         semantic_error_count++;
         // Don't mark initialized if types mismatch
     } else if (!assign_error && rvalue_type != TYPE_UNDEFINED && lvalue_type != TYPE_UNDEFINED) {
         // Assignment is valid type-wise and target-wise, mark initialized
         mark_symbol_initialized(sym);
     }

     // Assignment *expression* itself has a type (usually the type of the lvalue)
     node->expr_type = lvalue_type;
     // Return UNDEFINED if there was any error in the assignment process
     return (assign_error || rvalue_type == TYPE_UNDEFINED || lvalue_type == TYPE_UNDEFINED) ? TYPE_UNDEFINED : lvalue_type;
}


data_type check_if_stmt(AstNode *node) {
     if (!node || node->node_type != NODE_IF_STMT) return TYPE_UNDEFINED;

     AstNode *condition = node->data.if_stmt.condition;
     AstNode *then_branch = node->data.if_stmt.then_branch;
     AstNode *else_branch = node->data.if_stmt.else_branch;

     // --- Check Condition ---
     if (condition) {
          data_type cond_type = check_expression(condition);
          // Check only if condition evaluation didn't already error out
          if (cond_type != TYPE_UNDEFINED && cond_type != TYPE_BOOL) {
               compiler_error(condition->lineno, "If statement condition must be of type 'bool', but got '%s'.", type_to_string(cond_type));
               semantic_error_count++;
          }
     } else {
          compiler_error(node->lineno, "Internal Error: If statement missing condition.");
          semantic_error_count++;
     }

     // --- Check Then Branch ---
     if (then_branch) {
          check_statement(then_branch);
     } else {
          compiler_error(node->lineno, "Internal Error: If statement missing 'then' branch.");
          semantic_error_count++;
     }

     // --- Check Else Branch ---
     if (else_branch) {
           check_statement(else_branch);
     }

     node->expr_type = TYPE_VOID; // If statement is not an expression
     return TYPE_VOID;
}

data_type check_print_stmt(AstNode *node) {
     if (!node || node->node_type != NODE_PRINT_STMT) return TYPE_UNDEFINED;
     AstNode *expr = node->data.print_stmt.expression;

     if (expr) {
          data_type expr_type = check_expression(expr);
          // Check if the type is printable only if no error during evaluation
          if (expr_type != TYPE_UNDEFINED &&
              (expr_type == TYPE_VOID || expr_type == TYPE_FUNCTION))
          {
              compiler_error(expr->lineno, "Cannot print value of type '%s'.", type_to_string(expr_type));
              semantic_error_count++;
          }
     }

     node->expr_type = TYPE_VOID; // Print statement is not an expression
     return TYPE_VOID;
}

data_type check_return_stmt(AstNode *node) {
    if (!node || node->node_type != NODE_RETURN_STMT) return TYPE_UNDEFINED;
    AstNode *expr = node->data.return_stmt.expression;
    data_type return_expr_type = TYPE_VOID; // Assume void return if no expression

    if (!current_analyzing_function) {
        compiler_error(node->lineno, "Return statement used outside of a function.");
        semantic_error_count++;
        if (expr) check_expression(expr); // Still check expr if present
        return TYPE_UNDEFINED; // Indicate error for the statement context
    }

    data_type expected_return_type = current_analyzing_function->type;

    if (expr) {
        // Return with expression
        return_expr_type = check_expression(expr);

        // Check only if expression evaluation succeeded
        if (return_expr_type != TYPE_UNDEFINED) {
            if (expected_return_type == TYPE_VOID) {
                compiler_error(expr->lineno, "Cannot return a value from void function '%s'.", current_analyzing_function->name);
                semantic_error_count++;
                return_expr_type = TYPE_UNDEFINED; // Mark as error overall
            } else if (!check_type_compatibility(expected_return_type, return_expr_type, true)) {
                compiler_error(expr->lineno, "Return type mismatch: Function '%s' expects '%s', but got '%s'.",
                               current_analyzing_function->name,
                               type_to_string(expected_return_type),
                               type_to_string(return_expr_type));
                semantic_error_count++;
                return_expr_type = TYPE_UNDEFINED; // Mark as error overall
            }
        }
        // If expression itself had error, return_expr_type is already UNDEFINED
    } else {
        // Return without expression (return;)
        if (expected_return_type != TYPE_VOID) {
            compiler_error(node->lineno, "Non-void function '%s' must return a value of type '%s'.",
                           current_analyzing_function->name, type_to_string(expected_return_type));
            semantic_error_count++;
            return_expr_type = TYPE_UNDEFINED; // Mark as error overall
        }
        // If function expects void and gets 'return;', it's okay.
        return_expr_type = TYPE_VOID;
    }

    node->expr_type = return_expr_type; // Store the type being returned (or intended)
    // Return statement itself doesn't really have a value in the flow, control transfers.
    // Return VOID to signify it's a statement, unless an error occurred during checks.
    return (return_expr_type == TYPE_UNDEFINED) ? TYPE_UNDEFINED : TYPE_VOID;
}

// --- Expression Checking ---

data_type check_expression(AstNode *node) {
     if (!node) {
         compiler_error(0, "Internal Error: Attempted to check NULL expression node.");
         semantic_error_count++;
         return TYPE_UNDEFINED;
     }

     // Determine and store the type of the expression in node->expr_type
     switch(node->node_type) {
        // Literals have known types (set during creation or here)
        case NODE_INT_LITERAL:    node->expr_type = TYPE_INT; break;
        case NODE_DOUBLE_LITERAL: node->expr_type = TYPE_DOUBLE; break;
        case NODE_STRING_LITERAL: node->expr_type = TYPE_STRING; break;
        case NODE_CHAR_LITERAL:   node->expr_type = TYPE_CHAR; break;
        case NODE_BOOL_LITERAL:   node->expr_type = TYPE_BOOL; break;

        // Look up identifiers
        case NODE_IDENT_EXPR:     node->expr_type = check_ident_expr(node); break;

        // Recursively check operators
        case NODE_BINOP_EXPR:     node->expr_type = check_binop_expr(node); break;
        case NODE_UNOP_EXPR:      node->expr_type = check_unop_expr(node); break;

        // Check function calls
        case NODE_FUNC_CALL:      node->expr_type = check_func_call(node); break;

        // Cases that shouldn't be expressions
        case NODE_PROGRAM:
        case NODE_FUNC_DEF:
        case NODE_VAR_DECL:
        case NODE_CONST_DECL:
        case NODE_ASSIGN_STMT: // Handle if assignment is an expression in your language
        case NODE_IF_STMT:
        case NODE_PRINT_STMT:
        case NODE_RETURN_STMT:
            compiler_error(node->lineno, "Statement node type %s used as expression.", node_type_to_string(node->node_type));
            semantic_error_count++;
            node->expr_type = TYPE_UNDEFINED;
            break;

        case NODE_UNKNOWN:
        default:
            compiler_error(node->lineno, "Internal Error: Unknown AST node type %d encountered in check_expression.", node->node_type);
            semantic_error_count++;
            node->expr_type = TYPE_UNDEFINED;
            break;
     }
     // Return the determined type, or TYPE_UNDEFINED if an error occurred
     return node->expr_type;
}


data_type check_ident_expr(AstNode *node) {
    if (!node || node->node_type != NODE_IDENT_EXPR) return TYPE_UNDEFINED;
    symrec *sym = node->data.ident_expr.symbol;

    if (!sym) {
        compiler_error(node->lineno, "Internal Error: Identifier expression node has NULL symbol pointer.");
        semantic_error_count++;
        return TYPE_UNDEFINED;
    }

    // Check if variable is used before initialization (if not a function or param)
    if (sym->kind != SYM_FUNC && sym->kind != SYM_PARAM && !sym->is_initialized) {
        compiler_error(node->lineno, "Variable '%s' used before being initialized.", sym->name);
        semantic_error_count++;
        // Return UNDEFINED here as using an uninitialized value is an error
        node->expr_type = TYPE_UNDEFINED;
        return TYPE_UNDEFINED;
    }

    // Check if using a function name directly as a value (if not allowed)
    if (sym->kind == SYM_FUNC) {
        compiler_error(node->lineno, "Cannot use function name '%s' directly as a value (use function call syntax).", sym->name);
        semantic_error_count++;
        node->expr_type = TYPE_UNDEFINED;
        return TYPE_UNDEFINED;
    }

    // The type of the expression is the type stored in the symbol table record
    node->expr_type = sym->type;
    return sym->type;
}

data_type check_literal(AstNode *node) {
     if (!node) return TYPE_UNDEFINED;
     // Types are set during node creation, just return it.
     switch(node->node_type) {
        case NODE_INT_LITERAL:    return TYPE_INT;
        case NODE_DOUBLE_LITERAL: return TYPE_DOUBLE;
        case NODE_STRING_LITERAL: return TYPE_STRING;
        case NODE_CHAR_LITERAL:   return TYPE_CHAR;
        case NODE_BOOL_LITERAL:   return TYPE_BOOL;
        default:
            compiler_error(node->lineno, "Internal Error: Non-literal node type %d in check_literal.", node->node_type);
            semantic_error_count++;
            return TYPE_UNDEFINED;
     }
}


data_type check_binop_expr(AstNode *node) {
    if (!node || node->node_type != NODE_BINOP_EXPR) return TYPE_UNDEFINED;

    AstNode *left = node->data.binop_expr.left;
    AstNode *right = node->data.binop_expr.right;
    OperatorType op = node->data.binop_expr.op;

    // Recursively check operands first
    data_type left_type = check_expression(left);
    data_type right_type = check_expression(right);
    data_type result_type = TYPE_UNDEFINED; // Default to error

    // Check if operands themselves had errors
    if (left_type == TYPE_UNDEFINED || right_type == TYPE_UNDEFINED) {
        node->expr_type = TYPE_UNDEFINED; // Propagate error
        return TYPE_UNDEFINED;
    }

    // --- Type Checking based on Operator ---
    switch (op) {
        case OP_PLUS:
            if (left_type == TYPE_STRING && right_type == TYPE_STRING) { result_type = TYPE_STRING; }
            else if ((left_type == TYPE_INT || left_type == TYPE_DOUBLE) && (right_type == TYPE_INT || right_type == TYPE_DOUBLE)) { result_type = (left_type == TYPE_DOUBLE || right_type == TYPE_DOUBLE) ? TYPE_DOUBLE : TYPE_INT; }
            else { compiler_error(node->lineno, "Operator '+' cannot be applied to types '%s' and '%s'.", type_to_string(left_type), type_to_string(right_type)); semantic_error_count++; }
            break;
        case OP_MINUS: case OP_MUL: case OP_DIV:
            if ((left_type == TYPE_INT || left_type == TYPE_DOUBLE) && (right_type == TYPE_INT || right_type == TYPE_DOUBLE)) {
                if (op == OP_DIV && left_type == TYPE_INT && right_type == TYPE_INT) { result_type = TYPE_INT; }
                else if (op == OP_DIV) { result_type = TYPE_DOUBLE; }
                else { result_type = (left_type == TYPE_DOUBLE || right_type == TYPE_DOUBLE) ? TYPE_DOUBLE : TYPE_INT; }
            } else { compiler_error(node->lineno, "Operator '%s' cannot be applied to types '%s' and '%s'.", operator_to_string(op), type_to_string(left_type), type_to_string(right_type)); semantic_error_count++; }
            break;
        case OP_POWER:
             if ((left_type == TYPE_INT || left_type == TYPE_DOUBLE) && (right_type == TYPE_INT || right_type == TYPE_DOUBLE)) { result_type = TYPE_DOUBLE; }
             else { compiler_error(node->lineno, "Operator '**' cannot be applied to types '%s' and '%s'.", type_to_string(left_type), type_to_string(right_type)); semantic_error_count++; }
             break;
        case OP_EQ: case OP_NEQ:
            if (check_type_compatibility(left_type, right_type, true) || check_type_compatibility(right_type, left_type, true) || (left_type == right_type && left_type != TYPE_VOID && left_type != TYPE_FUNCTION)) { result_type = TYPE_BOOL; }
             else { compiler_error(node->lineno, "Operator '%s' cannot compare types '%s' and '%s'.", operator_to_string(op), type_to_string(left_type), type_to_string(right_type)); semantic_error_count++; }
            break;
        case OP_LT: case OP_GT: case OP_LTE: case OP_GTE:
            if (((left_type == TYPE_INT || left_type == TYPE_DOUBLE) && (right_type == TYPE_INT || right_type == TYPE_DOUBLE)) || (left_type == TYPE_CHAR && right_type == TYPE_CHAR) ) { result_type = TYPE_BOOL; }
            // else if (left_type == TYPE_STRING && right_type == TYPE_STRING) { result_type = TYPE_BOOL; } // Add if needed
             else { compiler_error(node->lineno, "Operator '%s' cannot compare types '%s' and '%s'.", operator_to_string(op), type_to_string(left_type), type_to_string(right_type)); semantic_error_count++; }
            break;
        case OP_AND: case OP_OR:
            if (left_type == TYPE_BOOL && right_type == TYPE_BOOL) { result_type = TYPE_BOOL; }
            else { compiler_error(node->lineno, "Operator '%s' requires 'bool' operands, but got '%s' and '%s'.", operator_to_string(op), type_to_string(left_type), type_to_string(right_type)); semantic_error_count++; }
            break;
        default:
            compiler_error(node->lineno, "Internal Error: Unhandled binary operator '%s'.", operator_to_string(op));
            semantic_error_count++;
            break;
    }

    node->expr_type = result_type; // Store result type in the node
    return result_type;
}


data_type check_unop_expr(AstNode *node) {
    if (!node || node->node_type != NODE_UNOP_EXPR) return TYPE_UNDEFINED;

    AstNode *operand = node->data.unop_expr.operand;
    OperatorType op = node->data.unop_expr.op;

    // Recursively check operand first
    data_type operand_type = check_expression(operand);
    data_type result_type = TYPE_UNDEFINED; // Default to error

     // Check if operand itself had errors
    if (operand_type == TYPE_UNDEFINED) {
        node->expr_type = TYPE_UNDEFINED; // Propagate error
        return TYPE_UNDEFINED;
    }

    switch (op) {
        case OP_NEGATE: // Unary minus
            if (operand_type == TYPE_INT || operand_type == TYPE_DOUBLE) { result_type = operand_type; }
            else { compiler_error(node->lineno, "Unary minus '-' cannot be applied to type '%s'.", type_to_string(operand_type)); semantic_error_count++; }
            break;
        case OP_NOT: // Logical not
             if (operand_type == TYPE_BOOL) { result_type = TYPE_BOOL; }
            else { compiler_error(node->lineno, "Logical not '!' requires 'bool' operand, but got '%s'.", type_to_string(operand_type)); semantic_error_count++; }
            break;
        default:
             compiler_error(node->lineno, "Internal Error: Unhandled unary operator '%s'.", operator_to_string(op));
             semantic_error_count++;
             break;
    }

    node->expr_type = result_type; // Store result type
    return result_type;
}

data_type check_func_call(AstNode *node) {
    if (!node || node->node_type != NODE_FUNC_CALL) return TYPE_UNDEFINED;

    symrec *func_sym = node->data.func_call.symbol;
    AstNode *arg_node = node->data.func_call.arguments; // Head of argument list

    // --- Check if Symbol is actually a Function ---
    if (!func_sym) {
        compiler_error(node->lineno, "Internal Error: Function call node has NULL symbol pointer.");
        semantic_error_count++;
        while (arg_node) { check_expression(arg_node); arg_node = arg_node->next; }
        return TYPE_UNDEFINED;
    }
    if (func_sym->kind != SYM_FUNC) {
        compiler_error(node->lineno, "'%s' is not a function, cannot be called.", func_sym->name);
        semantic_error_count++;
        AstNode* current_arg = node->data.func_call.arguments;
        while (current_arg) { check_expression(current_arg); current_arg = current_arg->next; }
        return TYPE_UNDEFINED;
    }

    // --- Check Arguments ---
    symrec *expected_param = func_sym->param_list;
    AstNode *current_arg = node->data.func_call.arguments;
    int arg_count = 0;
    int expected_count = func_sym->param_count;
    bool error_in_args = false;

    while (expected_param != NULL && current_arg != NULL) {
        arg_count++;
        data_type arg_type = check_expression(current_arg);
        data_type expected_type = expected_param->type;

         if (arg_type == TYPE_UNDEFINED) { error_in_args = true; }
         else if (!check_type_compatibility(expected_type, arg_type, true)) {
            compiler_error(current_arg->lineno, "Type mismatch in argument %d of call to '%s': Expected '%s', but got '%s'.", arg_count, func_sym->name, type_to_string(expected_type), type_to_string(arg_type));
            semantic_error_count++;
            error_in_args = true;
        }

        expected_param = expected_param->next_in_scope;
        current_arg = current_arg->next;
    }

    if (expected_param != NULL && current_arg == NULL) {
         compiler_error(node->lineno, "Too few arguments provided for function call '%s'. Expected %d, got %d.", func_sym->name, expected_count, arg_count);
         semantic_error_count++;
         error_in_args = true;
    }
    else if (expected_param == NULL && current_arg != NULL) {
         // Report error at the first extra argument
         compiler_error(current_arg->lineno, "Too many arguments provided for function call '%s'. Expected %d.", func_sym->name, expected_count);
         semantic_error_count++;
         error_in_args = true;
         while(current_arg) { // Still check remaining args for other errors
             if (check_expression(current_arg) == TYPE_UNDEFINED) { error_in_args = true; }
             current_arg = current_arg->next;
         }
    }

    if (error_in_args) {
        node->expr_type = TYPE_UNDEFINED;
        return TYPE_UNDEFINED;
    }

    node->expr_type = func_sym->type;
    return func_sym->type;
}