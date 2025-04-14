// ast.c
#include "ast.h"
#include "utils.h" // For error reporting and string conversions
#include "symtab.h" // For symrec struct definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static AstNode* allocate_node(NodeType type) {
    AstNode* node = (AstNode*)calloc(1, sizeof(AstNode)); // Use calloc for zero-init
    if (!node) {
        perror("Failed to allocate AST node");
        exit(EXIT_FAILURE);
    }
    node->node_type = type;
    node->lineno = 0;
    node->next = NULL;
    return node;
}

AstNode* create_program_node(AstNode* stmts) {
    AstNode* node = allocate_node(NODE_PROGRAM);
    node->data.program.statements = stmts;
    return node;
}

// Removed create_stmt_node as it was redundant

AstNode *create_decl_node(NodeType type, struct symrec *sym, AstNode *init) {
    if (type != NODE_VAR_DECL && type != NODE_CONST_DECL) {
        compiler_error(0, "Internal Error: Invalid node type %d for create_decl_node.", type);
        return NULL; // Or handle error more gracefully
    }
    // Symbol existence check should happen during semantic analysis, but basic kind check here is ok
    if (!sym) {
         compiler_error(0, "Internal Error: NULL symbol passed to create_decl_node.");
         free_ast(init); // Clean up initializer if symbol is bad
         return NULL;
    }
     if (sym->kind != SYM_VAR && sym->kind != SYM_PARAM) {
         // Parameters become declarations in the function body's scope
         compiler_error(0, "Internal Error: Cannot create decl node for non-variable/param symbol '%s' (%s).", sym->name, sym_kind_to_string(sym->kind));
         free_ast(init);
         return NULL;
     }

    AstNode *node = allocate_node(type);
    node->data.declaration.symbol = sym;
    node->data.declaration.initializer = init;
    // Link the AST node back to the symbol (useful for semantic checks)
    if (sym) sym->body_ast = node; // Using body_ast for declarations too might be okay, or add a dedicated field? Let's use it for now.
    return node;
}

AstNode *create_assign_node(AstNode *lval, AstNode *rval) {
     // Basic check: lvalue should be an identifier for simple assignments
     // More complex lvalues (like array access) would need different checks.
    if (!lval || (lval->node_type != NODE_IDENT_EXPR)) {
        // Error reporting will be more robust in semantic analysis
        // Freeing passed nodes here might be problematic if they are shared.
        // Semantic analysis should handle this.
        compiler_error(lval ? lval->lineno : 0, "Invalid Left Hand Side in assignment (must be an identifier).");
        free_ast(lval); // Assume they are not reused if invalid here
        free_ast(rval);
        return NULL;
    }
    if (!rval) {
        compiler_error(lval->lineno, "Invalid (NULL) Right Hand Side in assignment.");
        free_ast(lval); // Assume they are not reused
        return NULL;
    }
    AstNode *node = allocate_node(NODE_ASSIGN_STMT);
    node->data.assignment.lvalue = lval;
    node->data.assignment.rvalue = rval;
    return node;
}

AstNode *create_if_node(AstNode *cond, AstNode *then_b, AstNode *else_b) {
    if (!cond) {
         compiler_error(0, "Internal Error: NULL condition for if statement.");
         free_ast(then_b);
         free_ast(else_b);
         return NULL;
    }
     if (!then_b) {
         // Note: An empty 'then' block might be represented by a NULL pointer
         // depending on how the parser handles empty blocks {}. Assuming non-null needed.
         compiler_error(cond->lineno, "Internal Error: NULL 'then' branch for if statement.");
         free_ast(cond);
         free_ast(else_b);
         return NULL;
     }
    AstNode *node = allocate_node(NODE_IF_STMT);
    node->data.if_stmt.condition = cond;
    node->data.if_stmt.then_branch = then_b;
    node->data.if_stmt.else_branch = else_b; // Can be NULL
    return node;
}

AstNode *create_print_node(AstNode *expr) {
    // Allowing NULL expression for just printing a newline seems acceptable.
    // if (!expr) {
    //     // Create a default newline string if needed, but parser handles this?
    //     // Let's assume parser provides NULL if nothing is between ().
    // }
    AstNode *node = allocate_node(NODE_PRINT_STMT);
    node->data.print_stmt.expression = expr; // Can be NULL
    return node;
}

AstNode *create_return_node(AstNode *expr) {
    AstNode *node = allocate_node(NODE_RETURN_STMT);
    node->data.return_stmt.expression = expr; // Can be NULL for void returns
    return node;
}

AstNode *create_binop_node(OperatorType op, AstNode *left, AstNode *right) {
    if (!left || !right) {
        // Use operator_to_string from utils.h
        compiler_error(left ? left->lineno : (right ? right->lineno : 0),
                       "Internal Error: NULL operand for binary operator '%s'.", operator_to_string(op));
        free_ast(left);
        free_ast(right);
        return NULL;
    }
    AstNode *node = allocate_node(NODE_BINOP_EXPR);
    node->data.binop_expr.op = op;
    node->data.binop_expr.left = left;
    node->data.binop_expr.right = right;
    return node;
}

AstNode *create_unop_node(OperatorType op, AstNode *operand) {
    if (!operand) {
         compiler_error(0, "Internal Error: NULL operand for unary operator '%s'.", operator_to_string(op));
        return NULL;
    }
    AstNode *node = allocate_node(NODE_UNOP_EXPR);
    node->data.unop_expr.op = op;
    node->data.unop_expr.operand = operand;
    return node;
}

// Removed create_args_node and append_arg_node

AstNode *create_ident_node(struct symrec *var_sym) {
    // Robust check for symbol existence happens in semantic analysis
    // This node just stores the pointer provided by the parser (result of getsym)
    if (!var_sym) {
       // This indicates an error in the parser or symtab lookup (getsym failed)
       // Semantic analysis will catch undeclared identifiers properly.
       // We still create the node but semantic pass will flag it.
       compiler_warning(0, "Internal Warning: Creating identifier node with NULL symbol pointer.");
    }
    // else if (var_sym->kind != SYM_VAR && var_sym->kind != SYM_PARAM) {
       // Allow creating ident nodes for functions too initially, semantic check differentiates usage.
       // compiler_warning(0, "Warning: Creating identifier node for non-variable symbol '%s' (%s).", var_sym->name, sym_kind_to_string(var_sym->kind));
    // }
    AstNode *node = allocate_node(NODE_IDENT_EXPR);
    node->data.ident_expr.symbol = var_sym; // Store the symbol pointer
    return node;
}

AstNode *create_int_node(int value) {
    AstNode *node = allocate_node(NODE_INT_LITERAL);
    node->data.int_literal = value;
    node->expr_type = TYPE_INT; // Literals have known types
    return node;
}

AstNode *create_double_node(double value) {
    AstNode *node = allocate_node(NODE_DOUBLE_LITERAL);
    node->data.double_literal = value;
    node->expr_type = TYPE_DOUBLE; // Literals have known types
    return node;
}

AstNode *create_string_node(const char *value) {
    AstNode *node = allocate_node(NODE_STRING_LITERAL);
    if (value) {
        node->data.string_literal = strdup(value);
        if (!node->data.string_literal) {
            perror("Failed to duplicate string literal in AST");
            free(node); // Free the allocated node
            return NULL;
        }
    } else {
        // Representing null string explicitly? Or error?
        // Let's assume null means empty string for now.
        node->data.string_literal = strdup("");
         if (!node->data.string_literal) {
            perror("Failed to allocate empty string literal in AST");
            free(node);
            return NULL;
        }
    }
    node->expr_type = TYPE_STRING; // Literals have known types
    return node;
}

AstNode *create_char_node(char value) {
    AstNode *node = allocate_node(NODE_CHAR_LITERAL);
    node->data.char_literal = value;
    node->expr_type = TYPE_CHAR; // Literals have known types
    return node;
}

AstNode *create_bool_node(bool value) {
    AstNode *node = allocate_node(NODE_BOOL_LITERAL);
    node->data.bool_literal = value;
    node->expr_type = TYPE_BOOL; // Literals have known types
    return node;
}

AstNode *create_func_def_node(struct symrec *sym, AstNode *body) {
    if (!sym) {
        compiler_error(0, "Internal Error: NULL symbol for function definition.");
        free_ast(body);
        return NULL;
    }
     if (sym->kind != SYM_FUNC) {
        compiler_error(0, "Internal Error: Cannot create function definition for non-function symbol '%s' (%s).", sym->name, sym_kind_to_string(sym->kind));
        free_ast(body); // Clean up body if symbol is wrong type
        return NULL;
    }
    AstNode *node = allocate_node(NODE_FUNC_DEF);
    node->data.func_def.symbol = sym;
    node->data.func_def.body = body; // Body is the block node (statements)
    // Link the AST node back to the symbol entry
    sym->body_ast = node;
    return node;
}

AstNode *create_func_call_node(struct symrec *func_sym, AstNode *args) {
    // args is now the head of a linked list of argument expression nodes (or NULL if no args)
    // Robust check for symbol validity and argument matching happens in semantic analysis.
    if (!func_sym) {
         // This implies getsym failed in the parser. Semantic analysis will report undeclared function.
         compiler_warning(0, "Internal Warning: Creating function call node with NULL symbol pointer.");
         // We might still create the node for structure, semantic analysis handles the error.
    }
    // else if (func_sym->kind != SYM_FUNC) {
    //      // Allow creating the node, semantic analysis will report calling a non-function.
    //      compiler_warning(0, "Warning: Creating function call node for non-function symbol '%s' (%s).", func_sym->name, sym_kind_to_string(func_sym->kind));
    // }

    AstNode *node = allocate_node(NODE_FUNC_CALL);
    node->data.func_call.symbol = func_sym;
    node->data.func_call.arguments = args; // args is the head of the list
    return node;
}


// Freeing the AST (works with malloc/free now)
void free_ast(AstNode *node) {
    if (!node) {
        return;
    }

    // Recursively free children based on node type
    switch (node->node_type) {
        case NODE_PROGRAM:
            free_ast(node->data.program.statements);
            break;
        case NODE_FUNC_DEF:
            // Symbol associated with func_def is freed with symbol table
            free_ast(node->data.func_def.body);
            break;
        case NODE_VAR_DECL:
        case NODE_CONST_DECL:
             // Symbol associated with decl is freed with symbol table
            free_ast(node->data.declaration.initializer);
            break;
        case NODE_ASSIGN_STMT:
            free_ast(node->data.assignment.lvalue);
            free_ast(node->data.assignment.rvalue);
            break;
        case NODE_IF_STMT:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_branch);
            free_ast(node->data.if_stmt.else_branch);
            break;
        case NODE_PRINT_STMT:
            free_ast(node->data.print_stmt.expression);
            break;
        case NODE_RETURN_STMT:
            free_ast(node->data.return_stmt.expression);
            break;
        case NODE_BINOP_EXPR:
            free_ast(node->data.binop_expr.left);
            free_ast(node->data.binop_expr.right);
            break;
        case NODE_UNOP_EXPR:
            free_ast(node->data.unop_expr.operand);
            break;
        case NODE_FUNC_CALL:
             // Symbol associated with func_call is freed with symbol table
             // Arguments are a linked list, free them using the 'next' pointer traversal
             free_ast(node->data.func_call.arguments); // This handles the list via node->next recursion
             break;
        case NODE_IDENT_EXPR:
            // Symbol associated with ident is freed with symbol table
            break; // No children to free

        // Literals
        case NODE_STRING_LITERAL:
            free(node->data.string_literal); // Free the duplicated string
            break;
        case NODE_INT_LITERAL:
        case NODE_DOUBLE_LITERAL:
        case NODE_CHAR_LITERAL:
        case NODE_BOOL_LITERAL:
        case NODE_UNKNOWN: // Should not happen
             break; // No children or dynamic data to free here

        // Removed NODE_FUNC_CALL_ARGS
        // Default case for safety
        default:
             fprintf(stderr, "Warning: Unknown node type %d encountered in free_ast.\n", node->node_type);
             break;

    }

    // Free the list sibling *after* dealing with the current node and its children
    free_ast(node->next);

    // Finally, free the node itself
    free(node);
}