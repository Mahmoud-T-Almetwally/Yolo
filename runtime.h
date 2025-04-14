// runtime.h
#ifndef RUNTIME_H
#define RUNTIME_H

#include "types.h"
#include <stdbool.h>

struct AstNode;
struct symrec;
struct Environment;

typedef struct RuntimeValue {
    data_type type;
    union {
        int int_val;
        double double_val;
        char *string_val;
        char char_val;
        bool bool_val;
    } value;
    bool is_error;
    bool is_return;
} RuntimeValue;

typedef struct EnvEntry {
    struct symrec *symbol;
    RuntimeValue rt_value;
    struct EnvEntry *next;
} EnvEntry;

typedef struct Environment {
    int scope_level;
    EnvEntry *head;
    struct Environment *parent;
    const char* scope_name;
} Environment;

// Runtime Value Helpers
RuntimeValue create_error_value();
RuntimeValue create_int_value(int val);
RuntimeValue create_double_value(double val);
RuntimeValue create_string_value(const char* val);
RuntimeValue create_char_value(char val);
RuntimeValue create_bool_value(bool val);
RuntimeValue create_void_value();
RuntimeValue mark_as_return(RuntimeValue rv); // <<< ADDED prototype

void free_runtime_value_contents(RuntimeValue *rv);
void print_runtime_value(RuntimeValue val);

// Environment Management
Environment* create_environment(Environment* parent, const char* name);
void free_environment(Environment* env);
void store_value(Environment* env, struct symrec* sym, RuntimeValue val);
RuntimeValue lookup_value(Environment* env, struct symrec* sym);

// Interpreter prototype (if used)
RuntimeValue interpret(struct AstNode *node, Environment *env);

#endif // RUNTIME_H