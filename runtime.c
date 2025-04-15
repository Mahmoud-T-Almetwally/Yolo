// runtime.c
#include "runtime.h"
#include "symtab.h" // Need symrec definition for EnvEntry
#include "ast.h"    // Need AstNode definition for interpret prototype
#include "utils.h"  // For error reporting and type_to_string

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // For pow() in interpreter if needed

// --- Runtime Value Helpers ---

// Helper to create a base RuntimeValue
static RuntimeValue create_base_value(data_type type) {
    RuntimeValue rv;
    rv.type = type;
    rv.is_error = false;
    rv.is_return = false;
    // Initialize union members to avoid potential garbage values (optional but good practice)
    memset(&rv.value, 0, sizeof(rv.value));
    return rv;
}

RuntimeValue create_error_value(const char* message) {
    // Consider logging the error message here or storing it if needed
    // For simplicity, just mark as error.
    RuntimeValue rv = create_base_value(TYPE_UNDEFINED); // Use UNDEFINED for error type
    rv.is_error = true;
    // Optional: Store error message if RuntimeValue struct is extended
    // fprintf(stderr, "Runtime Error: %s\n", message); // Log immediately?
    return rv;
}

RuntimeValue create_int_value(int val) {
    RuntimeValue rv = create_base_value(TYPE_INT);
    rv.value.int_val = val;
    return rv;
}

RuntimeValue create_double_value(double val) {
    RuntimeValue rv = create_base_value(TYPE_DOUBLE);
    rv.value.double_val = val;
    return rv;
}

RuntimeValue create_string_value(const char* val) {
    RuntimeValue rv = create_base_value(TYPE_STRING);
    if (val) {
        rv.value.string_val = strdup(val);
        if (!rv.value.string_val) {
            perror("Failed to duplicate string for runtime value");
            return create_error_value("Memory allocation failure for string value");
        }
    } else {
        // Handle null input string - represent as empty string?
        rv.value.string_val = strdup("");
         if (!rv.value.string_val) {
            perror("Failed to allocate empty string for runtime value");
            return create_error_value("Memory allocation failure for empty string value");
        }
    }
    return rv;
}

RuntimeValue create_char_value(char val) {
    RuntimeValue rv = create_base_value(TYPE_CHAR);
    rv.value.char_val = val;
    return rv;
}

RuntimeValue create_bool_value(bool val) {
    RuntimeValue rv = create_base_value(TYPE_BOOL);
    rv.value.bool_val = val;
    return rv;
}

RuntimeValue create_void_value() {
    return create_base_value(TYPE_VOID);
}

// Marks a value as being the result of a 'return' statement
RuntimeValue mark_as_return(RuntimeValue rv) {
    // Cannot return an error value directly as a return value (error should propagate)
    if (!rv.is_error) {
        rv.is_return = true;
    }
    return rv;
}

// Unmarks a value after it has been handled by the caller (e.g., function call)
RuntimeValue unmark_return(RuntimeValue rv) {
    rv.is_return = false;
    return rv;
}


void free_runtime_value_contents(RuntimeValue *rv) {
    if (rv && rv->type == TYPE_STRING && rv->value.string_val) {
        free(rv->value.string_val);
        rv->value.string_val = NULL; // Avoid double free
    }
    // Other types don't have dynamically allocated contents in this basic version
}

void print_runtime_value(RuntimeValue val) {
    if (val.is_error) {
        printf("<error>");
        return;
    }
    // Note: Do not print if it's just a 'return' marker unless intended
    // if (val.is_return) printf("<return> "); // Optional: Indicate return status

    switch (val.type) {
        case TYPE_INT:    printf("%d", val.value.int_val); break;
        case TYPE_DOUBLE: printf("%g", val.value.double_val); break; // %g is often good for doubles
        case TYPE_STRING: printf("%s", val.value.string_val ? val.value.string_val : "null"); break;
        case TYPE_CHAR:   printf("%c", val.value.char_val); break;
        case TYPE_BOOL:   printf("%s", val.value.bool_val ? "true" : "false"); break;
        case TYPE_VOID:   printf("<void>"); break;
        case TYPE_FUNCTION: printf("<function>"); break; // Cannot print function itself
        case TYPE_UNDEFINED: printf("<undefined>"); break; // Should ideally be an error
        default: printf("<unknown_type:%d>", val.type); break;
    }
}

// --- Environment Management ---

Environment* create_environment(Environment* parent, const char* name) {
    Environment* env = (Environment*)malloc(sizeof(Environment));
    if (!env) {
        perror("Failed to allocate runtime environment");
        return NULL; // Indicate failure
    }
    env->scope_level = parent ? parent->scope_level + 1 : 0;
    env->head = NULL; // Initialize empty list of entries
    env->parent = parent;
    env->scope_name = name ? strdup(name) : strdup("anon_scope");
     if (!env->scope_name) {
        perror("Failed to duplicate scope name");
        free(env);
        return NULL;
    }
    return env;
}

// Helper to find an entry in this environment or parent environments
static EnvEntry* find_entry(Environment* env, struct symrec* sym) {
    if (!env || !sym) return NULL;
    Environment* current_env = env;
    while (current_env) {
        EnvEntry* entry = current_env->head;
        while (entry) {
            // Compare symbol pointers directly for efficiency
            if (entry->symbol == sym) {
                return entry;
            }
            entry = entry->next;
        }
        current_env = current_env->parent; // Search in parent scope
    }
    return NULL; // Not found
}


// Helper to find an entry ONLY in the specified environment
static EnvEntry* find_entry_in_current_scope(Environment* env, struct symrec* sym) {
    if (!env || !sym) return NULL;
    EnvEntry* entry = env->head;
    while (entry) {
        if (entry->symbol == sym) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}


void store_value(Environment* env, struct symrec* sym, RuntimeValue val) {
    if (!env || !sym) {
        fprintf(stderr, "Runtime Error: Attempted to store value with NULL environment or symbol.\n");
        free_runtime_value_contents(&val); // Clean up passed value if necessary
        return;
    }
     if (val.is_error) {
         fprintf(stderr, "Runtime Warning: Storing an error value for symbol '%s'.\n", sym->name);
         // Don't free contents of error value
         return; // Or handle error storage differently
     }

    // Find if the symbol *already exists* in this or parent scopes
    EnvEntry* existing_entry = find_entry(env, sym);

    if (existing_entry) {
        // --- Update existing entry ---
        // Free the old value's contents if necessary (e.g., old string)
        free_runtime_value_contents(&(existing_entry->rt_value));

        // Assign the new value (copying string if needed)
        existing_entry->rt_value = val;
        if (val.type == TYPE_STRING && val.value.string_val != NULL) {
            existing_entry->rt_value.value.string_val = strdup(val.value.string_val);
            if (!existing_entry->rt_value.value.string_val) {
                perror("Failed to duplicate string during update");
                existing_entry->rt_value = create_error_value("Memory allocation failure updating string");
            }
            // Free the original duplicated string from the input 'val' if it was created by create_string_value
            // This is tricky - assumes 'val' string was freshly strdup'd. Let's avoid freeing input val's string here.
            // free(val.value.string_val); // Risky - avoid this. Caller manages input 'val'.
        }
        // Ensure return flag is not spuriously stored
        existing_entry->rt_value.is_return = false;

    } else {
        // --- Create new entry in the current environment 'env' ---
        EnvEntry* new_entry = (EnvEntry*)malloc(sizeof(EnvEntry));
        if (!new_entry) {
            perror("Failed to allocate environment entry");
            free_runtime_value_contents(&val);
            return;
        }
        new_entry->symbol = sym; // Store the symbol pointer

        // Assign the value (copying string if needed)
        new_entry->rt_value = val;
         if (val.type == TYPE_STRING && val.value.string_val != NULL) {
            new_entry->rt_value.value.string_val = strdup(val.value.string_val);
            if (!new_entry->rt_value.value.string_val) {
                perror("Failed to duplicate string for new entry");
                free(new_entry);
                // Don't free input 'val' contents
                return;
            }
            // free(val.value.string_val); // Risky - avoid freeing input 'val' string
        }
        new_entry->rt_value.is_return = false; // Clear return flag on storage


        // Add to the head of the current environment's list
        new_entry->next = env->head;
        env->head = new_entry;
    }
}


RuntimeValue lookup_value(Environment* env, struct symrec* sym) {
    EnvEntry* entry = find_entry(env, sym);
    if (entry) {
        return entry->rt_value; // Return the found value
    } else {
        // Should not happen if semantic analysis passed (checked for undeclared vars)
        fprintf(stderr, "Runtime Error: Symbol '%s' not found in environment (should have been caught earlier).\n", sym ? sym->name : "NULLSYM");
        return create_error_value("Symbol not found during runtime lookup");
    }
}


void free_environment(Environment* env) {
    if (!env) return;

    EnvEntry* entry = env->head;
    EnvEntry* next_entry;
    while (entry) {
        next_entry = entry->next;
        // Free the contents of the runtime value stored in the entry (e.g., strings)
        free_runtime_value_contents(&(entry->rt_value));
        // Free the entry structure itself
        free(entry);
        entry = next_entry;
    }

    // Free the scope name
    free((void*)env->scope_name); // Cast needed if const char*

    // Free the environment structure itself
    // IMPORTANT: Do NOT free env->parent here. The caller manages parent scope lifetime.
    free(env);
}


// --- Interpreter ---

// Forward declarations for recursive calls within interpret
static RuntimeValue interpret_statements(AstNode *node, Environment *env);
static RuntimeValue interpret_expression(AstNode *node, Environment *env);
static RuntimeValue interpret_binop(AstNode *node, Environment *env);
static RuntimeValue interpret_unop(AstNode *node, Environment *env);
static RuntimeValue interpret_func_call(AstNode *node, Environment *env);

// Main recursive interpret function
RuntimeValue interpret(AstNode *node, Environment *env) {
    if (!node) {
        return create_void_value(); // Interpreting nothing results in void
    }
    // Check for prior errors or return values from siblings if part of a list (handled in interpret_statements)

    RuntimeValue result = create_void_value(); // Default result

    switch (node->node_type) {
        // --- Program / Blocks / Statements ---
        case NODE_PROGRAM: {
            // Global environment is created by main. Interpret statements within it.
            result = interpret_statements(node->data.program.statements, env);
            // Result of program is typically void, unless last statement returned.
            if (result.is_return) {
                fprintf(stderr, "Runtime Warning: 'return' used in global scope.\n");
                result = unmark_return(result); // Unmark return at top level
            }
            break;
        }
        // Note: parser.y creates statement lists directly, block node is just { stmts }
        // We don't have a specific NODE_BLOCK in the AST structure provided.
        // Scope handling for blocks happens implicitly via FUNC_DEF or IF/WHILE statements.

        // --- Statements ---
        case NODE_VAR_DECL:
        case NODE_CONST_DECL: {
            symrec *sym = node->data.declaration.symbol;
            RuntimeValue init_val = create_void_value(); // Default for uninitialized 'var'
            if (node->data.declaration.initializer) {
                init_val = interpret_expression(node->data.declaration.initializer, env);
                if (init_val.is_error) return init_val; // Propagate error
            } else {
                // Default initialize based on type? (e.g., 0 for int/double, "" for string, false for bool)
                switch (sym->type) {
                    case TYPE_INT: init_val = create_int_value(0); break;
                    case TYPE_DOUBLE: init_val = create_double_value(0.0); break;
                    case TYPE_STRING: init_val = create_string_value(""); break;
                    case TYPE_BOOL: init_val = create_bool_value(false); break;
                    case TYPE_CHAR: init_val = create_char_value('\0'); break;
                    default: /* void or error types */ break;
                }
            }
            
            store_value(env, sym, init_val);

            result = create_void_value();
            break;
        }
        case NODE_ASSIGN_STMT: {
            symrec *sym = node->data.assignment.lvalue->data.ident_expr.symbol;
            RuntimeValue rval = interpret_expression(node->data.assignment.rvalue, env);
            if (rval.is_error) return rval;

            store_value(env, sym, rval);

            result = create_void_value();
            break;
        }
        case NODE_IF_STMT: {
            RuntimeValue cond_val = interpret_expression(node->data.if_stmt.condition, env);
            if (cond_val.is_error) return cond_val;

            if (cond_val.type != TYPE_BOOL) {
                free_runtime_value_contents(&cond_val);
                fprintf(stderr, "Runtime Error: If condition did not evaluate to boolean (got %s)\n", data_type_to_string(cond_val.type));
                return create_error_value("If condition not boolean");
            }

            bool condition_result = cond_val.value.bool_val;
            free_runtime_value_contents(&cond_val);

            if (condition_result) {
                result = interpret_statements(node->data.if_stmt.then_branch, env);
            } else if (node->data.if_stmt.else_branch) {
                result = interpret_statements(node->data.if_stmt.else_branch, env);
            } else {
                 result = create_void_value();
            }
            
            break;
        }
        case NODE_PRINT_STMT: {
            if (node->data.print_stmt.expression) {
                RuntimeValue val_to_print = interpret_expression(node->data.print_stmt.expression, env);
                if (val_to_print.is_error) return val_to_print;
                print_runtime_value(val_to_print);
                free_runtime_value_contents(&val_to_print); // Free string if created
            }
            printf("\n"); // Print newline after expression (or if no expression)
            result = create_void_value();
            break;
        }
        case NODE_RETURN_STMT: {
            if (node->data.return_stmt.expression) {
                result = interpret_expression(node->data.return_stmt.expression, env);
                 if (result.is_error) return result; // Propagate error
            } else {
                result = create_void_value(); // Return void
            }
            result = mark_as_return(result); // Mark for propagation
            break;
        }
        case NODE_FUNC_DEF:
            // Function definition doesn't execute directly, just establishes the symbol.
            // Store the function symbol itself? No, symbol table handles it.
            // We might store a closure (function pointer + definition env) if supporting closures.
            // For now, do nothing during interpretation of the definition itself.
            result = create_void_value();
            break;


        // --- Expressions ---
        case NODE_INT_LITERAL:    result = create_int_value(node->data.int_literal); break;
        case NODE_DOUBLE_LITERAL: result = create_double_value(node->data.double_literal); break;
        case NODE_STRING_LITERAL: result = create_string_value(node->data.string_literal); break;
        case NODE_CHAR_LITERAL:   result = create_char_value(node->data.char_literal); break;
        case NODE_BOOL_LITERAL:   result = create_bool_value(node->data.bool_literal); break;

        case NODE_IDENT_EXPR:
            result = lookup_value(env, node->data.ident_expr.symbol);
            // Need to copy string value if returning it, as the entry might be overwritten later?
            // Let's assume lookup returns a value that is safe for immediate use.
            // If it's a string, the caller might need to copy if storing long term.
             if (result.type == TYPE_STRING && !result.is_error) {
                 char* original_string = result.value.string_val;
                 result.value.string_val = strdup(original_string ? original_string : "");
                 if (!result.value.string_val) {
                     result = create_error_value("Memory allocation failed copying looked-up string");
                 }
            }
            break;
        case NODE_BINOP_EXPR:
            result = interpret_binop(node, env);
            break;
        case NODE_UNOP_EXPR:
             result = interpret_unop(node, env);
             break;
        case NODE_FUNC_CALL:
             result = interpret_func_call(node, env);
             break;

        // --- Unknown/Error ---
        case NODE_UNKNOWN:
        default:
            fprintf(stderr, "Runtime Error: Cannot interpret unknown AST node type %d\n", node->node_type);
            result = create_error_value("Unknown AST node type");
            break;
    }

    return result;
}


// Helper to interpret a list of statements (handles return propagation)
static RuntimeValue interpret_statements(AstNode *node, Environment *env) {
    AstNode *current = node;
    RuntimeValue last_val = create_void_value(); // Result of the sequence

    while (current != NULL) {
        // Free previous statement's temporary result contents if needed (e.g., strings created but not stored)
        // This needs careful consideration depending on how values are managed.
        // Let's assume interpret handles freeing its intermediate results for now,
        // but keep free_runtime_value_contents(&last_val); in mind if memory leaks appear.
        // If last_val held a string returned by an expression-statement, it needs freeing.

        RuntimeValue current_result = interpret(current, env);

        // Free the contents of the *previous* statement's result value BEFORE overwriting last_val
        // This is crucial if the statement was an expression evaluating to a string.
        free_runtime_value_contents(&last_val);
        last_val = current_result; // Store the result of the current statement


        // If an error occurred or a return was executed, stop processing statements
        if (last_val.is_error || last_val.is_return) {
            return last_val; // Propagate immediately
        }

        current = current->next; // Move to the next statement
    }
    // Return the value of the last statement evaluated in the sequence
    // (or void/error/return if that's what terminated the loop)
    return last_val;
}

// Helper to interpret just expressions (used by interpret)
static RuntimeValue interpret_expression(AstNode *node, Environment *env) {
     // This simply calls the main interpret function for expression nodes
     // Could add checks here to ensure node is actually an expression type
     if (!node) return create_error_value("NULL expression node");

     switch(node->node_type) {
        case NODE_INT_LITERAL:
        case NODE_DOUBLE_LITERAL:
        case NODE_STRING_LITERAL:
        case NODE_CHAR_LITERAL:
        case NODE_BOOL_LITERAL:
        case NODE_IDENT_EXPR:
        case NODE_BINOP_EXPR:
        case NODE_UNOP_EXPR:
        case NODE_FUNC_CALL:
            return interpret(node, env);
        default:
             fprintf(stderr, "Runtime Error: Non-expression AST node type %d used as expression\n", node->node_type);
             return create_error_value("Invalid node type in expression context");
     }
}


// Handle Binary Operations
static RuntimeValue interpret_binop(AstNode *node, Environment *env) {
    RuntimeValue left_val = interpret_expression(node->data.binop_expr.left, env);
    if (left_val.is_error) return left_val;

    // Short-circuit evaluation for AND/OR
    OperatorType op = node->data.binop_expr.op;
    if (op == OP_AND) {
        if (left_val.type != TYPE_BOOL) { free_runtime_value_contents(&left_val); return create_error_value("Left operand of AND is not boolean"); }
        if (!left_val.value.bool_val) return create_bool_value(false); // Short circuit
    }
    if (op == OP_OR) {
         if (left_val.type != TYPE_BOOL) { free_runtime_value_contents(&left_val); return create_error_value("Left operand of OR is not boolean"); }
        if (left_val.value.bool_val) return create_bool_value(true); // Short circuit
    }


    RuntimeValue right_val = interpret_expression(node->data.binop_expr.right, env);
    if (right_val.is_error) { free_runtime_value_contents(&left_val); return right_val; }

    RuntimeValue result = create_error_value("Unhandled binary operation"); // Default to error

    // --- Perform Operation ---
    // Simplify by defining macros or functions for type checks and operations
    // Example: Arithmetic
    if (op == OP_PLUS || op == OP_MINUS || op == OP_MUL || op == OP_DIV || op == OP_POWER) {
        // Special case: String concatenation
        if (op == OP_PLUS && left_val.type == TYPE_STRING && right_val.type == TYPE_STRING) {
            size_t len1 = strlen(left_val.value.string_val);
            size_t len2 = strlen(right_val.value.string_val);
            char* new_str = (char*)malloc(len1 + len2 + 1);
            if (!new_str) { result = create_error_value("Memory allocation failed for string concat"); }
            else {
                strcpy(new_str, left_val.value.string_val);
                strcat(new_str, right_val.value.string_val);
                result = create_string_value(new_str); // create makes another copy
                free(new_str); // free temp buffer
            }
        }
        // Numeric operations (handle type promotion int -> double)
        else if ((left_val.type == TYPE_INT || left_val.type == TYPE_DOUBLE) &&
                 (right_val.type == TYPE_INT || right_val.type == TYPE_DOUBLE))
        {
            double l = (left_val.type == TYPE_DOUBLE) ? left_val.value.double_val : (double)left_val.value.int_val;
            double r = (right_val.type == TYPE_DOUBLE) ? right_val.value.double_val : (double)right_val.value.int_val;
            double res_d = 0.0;
            int res_i = 0;
            bool use_double = (left_val.type == TYPE_DOUBLE || right_val.type == TYPE_DOUBLE);
             bool int_division = (op == OP_DIV && !use_double); // Special handling for int division

            switch (op) {
                case OP_PLUS: res_d = l + r; break;
                case OP_MINUS: res_d = l - r; break;
                case OP_MUL: res_d = l * r; break;
                case OP_DIV:
                    if (r == 0.0) { result = create_error_value("Division by zero"); goto binop_cleanup; } // Use goto for cleanup
                     if (int_division) res_i = (int)l / (int)r; else res_d = l / r;
                    break;
                case OP_POWER: res_d = pow(l, r); break; // Requires math.h
                default: /* Should not happen */ break;
            }

            if (result.is_error) goto binop_cleanup; // Check if division by zero happened

            if (int_division) result = create_int_value(res_i);
            else if (use_double || op == OP_POWER) result = create_double_value(res_d);
            else result = create_int_value((int)res_d); // Result of int ops is int

        } else {
             result = create_error_value("Invalid types for arithmetic operation");
        }
    }
    // Example: Comparison
    else if (op == OP_EQ || op == OP_NEQ || op == OP_LT || op == OP_GT || op == OP_LTE || op == OP_GTE) {
        bool comp_result = false;
        // Compare based on type
        if (left_val.type == right_val.type) {
             switch(left_val.type) {
                case TYPE_INT: {
                    int l = left_val.value.int_val; int r = right_val.value.int_val;
                    if (op == OP_EQ) comp_result = (l == r); else if (op == OP_NEQ) comp_result = (l != r);
                    else if (op == OP_LT) comp_result = (l < r); else if (op == OP_GT) comp_result = (l > r);
                    else if (op == OP_LTE) comp_result = (l <= r); else if (op == OP_GTE) comp_result = (l >= r);
                    break; }
                 case TYPE_DOUBLE: {
                    double l = left_val.value.double_val; double r = right_val.value.double_val;
                     // Be careful with direct == comparison for doubles if needed
                     if (op == OP_EQ) comp_result = (l == r); else if (op == OP_NEQ) comp_result = (l != r);
                    else if (op == OP_LT) comp_result = (l < r); else if (op == OP_GT) comp_result = (l > r);
                    else if (op == OP_LTE) comp_result = (l <= r); else if (op == OP_GTE) comp_result = (l >= r);
                    break; }
                 case TYPE_CHAR: {
                    char l = left_val.value.char_val; char r = right_val.value.char_val;
                     if (op == OP_EQ) comp_result = (l == r); else if (op == OP_NEQ) comp_result = (l != r);
                    else if (op == OP_LT) comp_result = (l < r); else if (op == OP_GT) comp_result = (l > r);
                    else if (op == OP_LTE) comp_result = (l <= r); else if (op == OP_GTE) comp_result = (l >= r);
                    break; }
                 case TYPE_BOOL: {
                    bool l = left_val.value.bool_val; bool r = right_val.value.bool_val;
                     if (op == OP_EQ) comp_result = (l == r); else if (op == OP_NEQ) comp_result = (l != r);
                     else { result = create_error_value("Cannot use <, >, <=, >= on booleans"); goto binop_cleanup;}
                    break; }
                  case TYPE_STRING: {
                     int cmp = strcmp(left_val.value.string_val, right_val.value.string_val);
                     if (op == OP_EQ) comp_result = (cmp == 0); else if (op == OP_NEQ) comp_result = (cmp != 0);
                     else if (op == OP_LT) comp_result = (cmp < 0); else if (op == OP_GT) comp_result = (cmp > 0);
                     else if (op == OP_LTE) comp_result = (cmp <= 0); else if (op == OP_GTE) comp_result = (cmp >= 0);
                    break; }
                 default: result = create_error_value("Cannot compare types"); goto binop_cleanup;
             }
             result = create_bool_value(comp_result);
        }
        // Handle int/double comparison promotion for EQ/NEQ maybe? Others less common.
        else if ((op == OP_EQ || op == OP_NEQ) &&
                 (left_val.type == TYPE_INT || left_val.type == TYPE_DOUBLE) &&
                 (right_val.type == TYPE_INT || right_val.type == TYPE_DOUBLE))
        {
             double l = (left_val.type == TYPE_DOUBLE) ? left_val.value.double_val : (double)left_val.value.int_val;
             double r = (right_val.type == TYPE_DOUBLE) ? right_val.value.double_val : (double)right_val.value.int_val;
             if (op == OP_EQ) comp_result = (l == r); else comp_result = (l != r);
             result = create_bool_value(comp_result);
        }
         else {
            result = create_error_value("Incompatible types for comparison operator");
        }
    }
     // Example: Logical AND/OR (short-circuit handled earlier)
    else if (op == OP_AND || op == OP_OR) {
         // We only get here if short-circuiting didn't happen
         if (left_val.type == TYPE_BOOL && right_val.type == TYPE_BOOL) {
              if (op == OP_AND) result = create_bool_value(left_val.value.bool_val && right_val.value.bool_val);
              else result = create_bool_value(left_val.value.bool_val || right_val.value.bool_val);
         } else {
              result = create_error_value("Invalid types for logical operator (post-short-circuit)");
         }
    }

binop_cleanup:
    // Free intermediate values' contents (strings)
    free_runtime_value_contents(&left_val);
    free_runtime_value_contents(&right_val);
    return result;
}

// Handle Unary Operations
static RuntimeValue interpret_unop(AstNode *node, Environment *env) {
     RuntimeValue operand_val = interpret_expression(node->data.unop_expr.operand, env);
    if (operand_val.is_error) return operand_val;

    OperatorType op = node->data.unop_expr.op;
    RuntimeValue result = create_error_value("Unhandled unary operation"); // Default

    switch (op) {
        case OP_NEGATE: // Unary minus
            if (operand_val.type == TYPE_INT) {
                result = create_int_value(-operand_val.value.int_val);
            } else if (operand_val.type == TYPE_DOUBLE) {
                result = create_double_value(-operand_val.value.double_val);
            } else {
                 result = create_error_value("Invalid type for unary minus");
            }
            break;
        case OP_NOT: // Logical not
             if (operand_val.type == TYPE_BOOL) {
                result = create_bool_value(!operand_val.value.bool_val);
            } else {
                 result = create_error_value("Invalid type for logical not");
            }
            break;
        default: /* Should not happen */ break;
    }

    free_runtime_value_contents(&operand_val); // Free intermediate operand value contents
    return result;
}


// Handle Function Calls
static RuntimeValue interpret_func_call(AstNode *node, Environment *env) {
    symrec *func_sym = node->data.func_call.symbol;

    // --- Built-in Functions (Example) ---
    // if (strcmp(func_sym->name, "some_builtin") == 0) {
    //     // Handle built-in call
    // }

    // --- User-Defined Functions ---
    AstNode *body = func_sym->body_ast;
    if (!body) {
         return create_error_value("Function has no body to interpret"); // Should be caught earlier
    }

    // 1. Create new environment for the function call
    //    Parent is the *caller's* environment (Static Scoping)
    Environment *call_env = create_environment(env, func_sym->name);
     if (!call_env) return create_error_value("Failed to create function call environment");

    // 2. Evaluate arguments *in the caller's environment*
    AstNode *current_arg_node = node->data.func_call.arguments;
    symrec *current_param_sym = func_sym->param_list;
    RuntimeValue evaluated_args[func_sym->param_count]; // Temp storage for evaluated args
    int arg_idx = 0;

    while (current_param_sym != NULL && current_arg_node != NULL) {
        RuntimeValue arg_val = interpret_expression(current_arg_node, env);
        if (arg_val.is_error) {
            // Clean up already evaluated args
            for (int i = 0; i < arg_idx; ++i) free_runtime_value_contents(&evaluated_args[i]);
            free_environment(call_env);
            return arg_val; // Propagate argument evaluation error
        }
        evaluated_args[arg_idx++] = arg_val; // Store evaluated arg

        current_arg_node = current_arg_node->next;
        current_param_sym = current_param_sym->next_in_scope;
    }
    // Semantic analysis should have checked arg count mismatch

    // 3. Store evaluated arguments into the function's environment using parameter symbols
    current_param_sym = func_sym->param_list; // Reset param pointer
    for (int i = 0; i < arg_idx; ++i) {
        // store_value copies the value (e.g., strdup for strings)
        store_value(call_env, current_param_sym, evaluated_args[i]);
        // We don't need to free evaluated_args[i] contents now, store_value handled it.
        current_param_sym = current_param_sym->next_in_scope;
    }


    // 4. Interpret the function body in the new environment
    RuntimeValue result = interpret_statements(body, call_env); // interpret_statements handles blocks


    // 5. Clean up
    free_environment(call_env); // Free the function call scope

    // 6. Handle return value
    if (result.is_error) {
        return result; // Propagate error from function body
    } else if (result.is_return) {
        return unmark_return(result); // Return the actual value, unmarking the flag
    } else {
        // Function completed without hitting a 'return' statement
        if (func_sym->type != TYPE_VOID) {
            // Semantic analysis should catch non-void function without return, but good runtime check too
             fprintf(stderr, "Runtime Warning: Non-void function '%s' finished without returning a value.\n", func_sym->name);
             // Return a default value for the expected type? Or error? Let's return default.
             switch(func_sym->type) {
                 case TYPE_INT: return create_int_value(0);
                 case TYPE_DOUBLE: return create_double_value(0.0);
                 case TYPE_STRING: return create_string_value("");
                 case TYPE_BOOL: return create_bool_value(false);
                 case TYPE_CHAR: return create_char_value('\0');
                 default: return create_error_value("Function ended without return for unexpected type");
             }
        } else {
            return create_void_value(); // Implicit void return is fine for void functions
        }
    }
}