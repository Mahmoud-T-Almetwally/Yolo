// symtab.c
#include "symtab.h"
#include "utils.h"  // For error reporting and string conversions
#include "ast.h"    // For AstNode definition (used in symrec)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global scope pointers
SymbolTable *global_scope = NULL;
SymbolTable *current_scope = NULL;
static int current_scope_level = -1; // Start at -1, global becomes 0

// --- Scope Management ---

void init_scope_management() {
    if (global_scope != NULL) {
        // Avoid re-initializing if called multiple times
        cleanup_scope_management();
    }
    global_scope = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!global_scope) {
        perror("Failed to allocate global scope");
        exit(EXIT_FAILURE);
    }
    global_scope->scope_level = 0;
    global_scope->head = NULL;
    global_scope->parent = NULL;
    global_scope->scope_name = strdup("global");
    global_scope->symbol_count = 0;

    current_scope = global_scope;
    current_scope_level = 0;
    // TODO: Add built-in functions/types here if needed
    // e.g., putsym("true", SYM_VAR, TYPE_BOOL); set_symbol_const(...); mark_symbol_initialized(...);
}

void enter_scope(const char* name) {
    current_scope_level++;
    SymbolTable* new_scope = (SymbolTable*)malloc(sizeof(SymbolTable));
     if (!new_scope) {
        perror("Failed to allocate new scope");
        // Attempt to recover? For now, exit.
        exit(EXIT_FAILURE);
    }
    new_scope->scope_level = current_scope_level;
    new_scope->head = NULL;
    new_scope->parent = current_scope; // Link to parent
    new_scope->scope_name = name ? strdup(name) : strdup("block"); // Default name if NULL
    new_scope->symbol_count = 0;

    current_scope = new_scope; // Update current scope pointer
}

void exit_scope() {
    if (current_scope == global_scope || current_scope == NULL) {
        compiler_error(0, "Internal Error: Attempting to exit global scope or NULL scope.");
        return;
    }

    SymbolTable *parent_scope = current_scope->parent;
    // Optional: Print table before freeing for debugging
    // print_symbol_table(current_scope, current_scope->scope_level * 2);

    free_symbol_table_entries(current_scope); // Free symbols defined in this scope
    free(current_scope->scope_name);
    free(current_scope); // Free the table structure itself

    current_scope = parent_scope; // Move back to parent
    current_scope_level--;

     if (current_scope == NULL || current_scope_level < 0) {
         compiler_error(0, "Internal Error: Scope management inconsistency after exit_scope.");
         // Attempt to reset to global to prevent crashes, but indicates a bug
         current_scope = global_scope;
         current_scope_level = 0;
     }
}

SymbolTable* get_current_symbol_table() {
    return current_scope;
}

// --- Symbol Management Functions ---

symrec *putsym(const char *name, sym_kind kind, data_type type) {
    if (!current_scope) {
        compiler_error(0, "Internal Error: No current scope to add symbol '%s'.", name);
        return NULL;
    }
    // Check for redefinition in the *current* scope only
    if (getsym_in_scope(name, current_scope) != NULL) {
        // Error reporting handled by semantic analysis usually (better line info)
        // PARSER_ERROR(yylineno, "Symbol '%s' redefined in the current scope.", name); // Use yylineno if called from parser
        return NULL; // Indicate failure
    }

    // Create new symbol record
    symrec *new_sym = (symrec*)malloc(sizeof(symrec));
    if (!new_sym) {
        perror("Failed to allocate symbol record");
        return NULL; // Indicate failure
    }

    new_sym->name = strdup(name);
    if (!new_sym->name) {
        perror("Failed to duplicate symbol name");
        free(new_sym);
        return NULL;
    }

    new_sym->kind = kind;
    new_sym->type = type;
    new_sym->is_constant = false; // Default to non-constant
    new_sym->is_initialized = false; // Default to uninitialized
    new_sym->scope_level = current_scope->scope_level;
    new_sym->definition_scope = current_scope;

    // Function specific defaults
    new_sym->param_list = NULL;
    new_sym->body_ast = NULL;
    new_sym->param_count = 0;

    // Link into the current scope's list (add at head)
    new_sym->next_in_scope = current_scope->head;
    current_scope->head = new_sym;
    current_scope->symbol_count++;

    return new_sym; // Return pointer to the new symbol
}

symrec *getsym(const char *name) {
    if (!name) return NULL;
    SymbolTable *scope = current_scope;
    while (scope != NULL) {
        symrec *sym = getsym_in_scope(name, scope);
        if (sym != NULL) {
            return sym; // Found in this scope or a parent scope
        }
        scope = scope->parent; // Move up to the parent scope
    }
    return NULL; // Not found in any accessible scope
}

symrec *getsym_in_scope(const char *name, SymbolTable *scope) {
     if (!scope || !name) return NULL;
     symrec *current_sym = scope->head;
     while (current_sym != NULL) {
        if (strcmp(current_sym->name, name) == 0) {
            return current_sym; // Found
        }
        current_sym = current_sym->next_in_scope;
    }
    return NULL; // Not found in this specific scope
}


// --- Setters/Modifiers for Symbols ---

void set_symbol_type(symrec *sym, data_type type) {
    if (sym) {
        sym->type = type;
    }
}

void set_symbol_const(symrec *sym, bool is_const) {
     if (sym) {
        sym->is_constant = is_const;
    }
}

void mark_symbol_initialized(symrec *sym) {
    if (sym) {
        sym->is_initialized = true;
    }
}

void add_func_param(symrec *func_sym, symrec *param_sym) {
    if (!func_sym || func_sym->kind != SYM_FUNC || !param_sym) {
        compiler_error(0, "Internal Error: Invalid arguments to add_func_param.");
        return;
    }

    param_sym->next_in_scope = NULL; // Ensure param's next pointer is clear for list use

    // Add to the end of the function's parameter list
    if (func_sym->param_list == NULL) {
        // First parameter
        func_sym->param_list = param_sym;
    } else {
        // Find the last parameter and append
        symrec *last_param = func_sym->param_list;
        while (last_param->next_in_scope != NULL) {
            last_param = last_param->next_in_scope;
        }
        last_param->next_in_scope = param_sym;
    }
    func_sym->param_count++;
}

void set_func_body(symrec *func_sym, struct AstNode *body) {
     if (func_sym && func_sym->kind == SYM_FUNC) {
        func_sym->body_ast = body;
    } else {
         compiler_error(0, "Internal Error: Attempted to set body on non-function or NULL symbol.");
    }
}

// --- Cleanup ---

void free_symbol_table_entries(SymbolTable *table) {
    if (!table) return;
    symrec *current_sym = table->head;
    symrec *next_sym;

    while (current_sym != NULL) {
        next_sym = current_sym->next_in_scope; // Save next pointer

        // Free allocated data within the symbol record
        free(current_sym->name);

        // Parameters are owned by the function symbol's list,
        // but were also added to the function's scope table.
        // Avoid double-freeing parameter name/struct.
        // We only free the main list chain here.
        // The actual param symrecs are freed when the function scope exits.
        // For function symbols, free the parameter list *they* own.
        if (current_sym->kind == SYM_FUNC) {
            symrec *param = current_sym->param_list;
            symrec *next_param;
            while (param) {
                // Parameters added via add_func_param are just pointers
                // to symrecs created in the function's scope table.
                // They will be freed when that scope table is freed.
                // So, we *don't* free the param symrec itself here,
                // just break the list structure.
                next_param = param->next_in_scope;
                param->next_in_scope = NULL; // Avoid dangling pointers
                param = next_param;
            }
            current_sym->param_list = NULL; // Clear the list head
        }


        // Free the symbol record itself
        free(current_sym);

        current_sym = next_sym; // Move to the next symbol in the original scope list
    }
    table->head = NULL; // Reset the head of the list for the table
    table->symbol_count = 0;
}

void cleanup_scope_management() {
    // Exit scopes until only global remains (shouldn't be necessary if balanced)
    while (current_scope != global_scope && current_scope != NULL) {
        compiler_warning(0, "Warning: Exiting scope '%s' during cleanup - scope exit likely missed.", current_scope->scope_name);
        exit_scope();
    }

    if (global_scope) {
        // Optional: Print global scope before freeing
        // print_symbol_table(global_scope, 0);
        free_symbol_table_entries(global_scope);
        free(global_scope->scope_name);
        free(global_scope);
    }

    global_scope = NULL;
    current_scope = NULL;
    current_scope_level = -1;
}


// --- Debugging / Helpers ---

void print_symbol_table(SymbolTable *table, int indent) {
    if (!table) return;

    char indent_str[indent + 1];
    for (int i = 0; i < indent; ++i) indent_str[i] = ' ';
    indent_str[indent] = '\0';

    printf("%s--- Symbol Table (Scope: %s, Level: %d) ---\n", indent_str, table->scope_name ? table->scope_name : "??", table->scope_level);

    symrec *sym = table->head;
    if (!sym) {
        printf("%s(empty)\n", indent_str);
    }
    while (sym != NULL) {
        printf("%s  Name: %-15s | Kind: %-10s | Type: %-10s | Const: %s | Init: %s",
               indent_str,
               sym->name,
               sym_kind_to_string(sym->kind),
               data_type_to_string(sym->type),
               sym->is_constant ? "yes" : "no ",
               sym->is_initialized ? "yes" : "no ");

        if (sym->kind == SYM_FUNC) {
            printf(" | Params: (");
            symrec *param = sym->param_list;
            int count = 0;
            while(param) {
                if (count > 0) printf(", ");
                printf("%s: %s", param->name, data_type_to_string(param->type));
                param = param->next_in_scope; // Params linked via next_in_scope in func list
                count++;
            }
             printf(") %d expected\n", sym->param_count);
        } else {
            printf("\n");
        }
        sym = sym->next_in_scope;
    }
     printf("%s-------------------------------------------\n", indent_str);
}

void print_all_scopes() {
    printf("\n=== All Scopes (Current: %s, Level: %d) ===\n",
           current_scope ? (current_scope->scope_name ? current_scope->scope_name : "??") : "NULL",
           current_scope_level);
    SymbolTable *scope = current_scope;
    while(scope != NULL) {
        print_symbol_table(scope, scope->scope_level * 2);
        scope = scope->parent;
    }
     printf("========================================\n\n");
}