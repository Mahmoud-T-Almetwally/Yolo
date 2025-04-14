// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

#include "types.h"    // Must be first for enums
// #include "runtime.h" - Removed runtime value for now, focus on static analysis
#include <stdbool.h>
#include <stdlib.h>

// --- Forward Declarations ---
struct AstNode;     // Defined in ast.h
struct SymbolTable; // Fully defined below

// --- Symbol Record ---
typedef struct symrec {
    char *name;             // Symbol name (dynamically allocated)
    sym_kind kind;          // Kind of symbol (VAR, FUNC, PARAM)
    data_type type;         // Data type (or return type for functions)
    bool is_constant;       // If the symbol is a constant (cannot be reassigned)
    bool is_initialized;    // If the variable/constant has been assigned a value
    int scope_level;        // Lexical scope level where defined
    struct SymbolTable* definition_scope; // Pointer to the table where defined

    // Function specific fields
    struct symrec *param_list; // Head of linked list of parameter symbols
    struct AstNode *body_ast;  // Pointer to the function body's AST node (or decl node for vars)
    int param_count;        // Number of parameters

    // Linking
    struct symrec *next_in_scope; // Next symbol defined in the same scope

    // Future: Could add offset for code generation, runtime value for interpreter, etc.
    // RuntimeValue runtime_value; // Temporarily removed

} symrec;

// --- Symbol Table (Scope) ---
typedef struct SymbolTable {
    int scope_level;            // Nesting depth (0 = global)
    symrec *head;               // Head of the linked list of symbols in this scope
    struct SymbolTable *parent; // Pointer to the parent scope's table (NULL for global)
    char* scope_name;           // Optional name (e.g., function name, "block")
    int symbol_count;           // Number of symbols directly in this scope
} SymbolTable;

// --- Scope Management Globals ---
extern SymbolTable *global_scope;
extern SymbolTable *current_scope;

// --- Scope Management ---
void init_scope_management();
void enter_scope(const char* name); // Name is for debugging/identification
void exit_scope();
SymbolTable* get_current_symbol_table(); // Get current scope table pointer

// --- Symbol Management Functions ---
// Adds symbol to the *current* scope. Returns pointer to the new symrec or NULL on failure (e.g., redefinition).
symrec *putsym(const char *name, sym_kind kind, data_type type);
// Searches for symbol starting from *current* scope up to global. Returns symrec or NULL if not found.
symrec *getsym(const char *name);
// Searches for symbol only within the *specified* scope. Returns symrec or NULL.
symrec *getsym_in_scope(const char *name, SymbolTable *scope);

// --- Setters/Modifiers for Symbols ---
// These should ideally be used by the parser or semantic analyzer
void set_symbol_type(symrec *sym, data_type type); // Useful for functions parsed in stages
void set_symbol_const(symrec *sym, bool is_const);
void mark_symbol_initialized(symrec *sym);
void add_func_param(symrec *func_sym, symrec *param_sym); // Links param to func's list
void set_func_body(symrec *func_sym, struct AstNode *body); // Associates AST with func sym

// --- Cleanup ---
// Frees symbols *within* a specific table, but not the table struct itself
void free_symbol_table_entries(SymbolTable *table);
// Frees all symbol tables and their entries, cleans up globals
void cleanup_scope_management();

// --- Debugging / Helpers ---
// (Implementations should be in utils.c or symtab.c as appropriate)
// const char* sym_kind_to_string(sym_kind k); // Provided in utils.h/c
// const char* data_type_to_string(data_type t); // Provided in utils.h/c
void print_symbol_table(SymbolTable *table, int indent); // For debugging
void print_all_scopes(); // Prints current scope stack for debugging

#endif // SYMTAB_H