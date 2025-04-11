#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Symbol kinds */
typedef enum {
    SYM_VAR,
    SYM_FUNC
} sym_kind;

/* Data types for variables and return types for functions */
typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_UNDEFINED
} data_type;

/* Function pointer types */
typedef double (*double_func_t)(double);
typedef int (*int_func_t)(double);
typedef char* (*string_func_t)(double);

/* Symbol table entry */
typedef struct symrec {
    char *name;             /* Symbol name */
    sym_kind kind;          /* Variable or Function */
    data_type type;         /* Data type or return type */
    bool is_constant;       /* Constancy flag */
    union {                 /* Value storage */
        struct {
            union {
                int int_val;
                double double_val;
                char *string_val;
                char char_val;
                bool bool_val;
            };
        } var;
        struct {
            union {
                int_func_t int_func;
                double_func_t double_func;
                string_func_t string_func;
            };
        } func;
    } value;
    struct symrec *next;    /* Linked list pointer */
} symrec;

extern symrec *sym_table;   /* Symbol table head */

/* Function prototypes */
symrec *putsym(const char *name, sym_kind kind);
symrec *getsym(const char *name);
void set_var_type(symrec *sym, data_type type);
void set_var_value_int(symrec *sym, int val);
void set_var_value_double(symrec *sym, double val);
void set_var_value_string(symrec *sym, char *val);
void set_var_value_char(symrec *sym, char val);
void set_var_value_bool(symrec *sym, bool val);
void set_func_return_type(symrec *sym, data_type type);
void set_func_impl(symrec *sym, void *func_ptr);
void free_symbol_table(void);
void print_symbol_table(void);

#endif /* SYMTAB_H */