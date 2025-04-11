#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symrec *sym_table = NULL;

/* Create/update symbol entry */
symrec *putsym(const char *name, sym_kind kind) {
    symrec *ptr = (symrec *)malloc(sizeof(symrec));
    ptr->name = strdup(name);
    ptr->kind = kind;
    ptr->type = TYPE_UNDEFINED;
    ptr->is_constant = (kind == SYM_FUNC); /* Functions are always constant */
    ptr->next = sym_table;
    sym_table = ptr;
    return ptr;
}

/* Retrieve symbol by name */
symrec *getsym(const char *name) {
    symrec *ptr;
    for (ptr = sym_table; ptr != NULL; ptr = ptr->next)
        if (strcmp(ptr->name, name) == 0)
            return ptr;
    return NULL;
}

void print_symbol_table(void) {
    symrec *ptr = sym_table;
    while (ptr != NULL) {
        printf("Name: %s, Kind: %d, Type: %d\n", ptr->name, ptr->kind, ptr->type);
        if (ptr->kind == SYM_VAR) {
            switch (ptr->type) {
                case TYPE_INT:    printf("Value: %d\n", ptr->value.var.int_val); break;
                case TYPE_DOUBLE: printf("Value: %f\n", ptr->value.var.double_val); break;
                case TYPE_STRING: printf("Value: %s\n", ptr->value.var.string_val); break;
                case TYPE_CHAR:   printf("Value: %c\n", ptr->value.var.char_val); break;
                case TYPE_BOOL:   printf("Value: %d\n", ptr->value.var.bool_val); break;
                default: break;
            }
        }
        ptr = ptr->next;
    }
}

/* Type setters */
void set_var_type(symrec *sym, data_type type) {
    if (sym->kind == SYM_VAR) sym->type = type;
}

void set_func_return_type(symrec *sym, data_type type) {
    if (sym->kind == SYM_FUNC) sym->type = type;
}

/* Value setters */
void set_var_value_int(symrec *sym, int val) {
    if (sym->kind == SYM_VAR && sym->type == TYPE_INT)
        sym->value.var.int_val = val;
}

void set_var_value_double(symrec *sym, double val) {
    if (sym->kind == SYM_VAR && sym->type == TYPE_DOUBLE)
        sym->value.var.double_val = val;
}

void set_var_value_string(symrec *sym, char *val) {
    if (sym->kind == SYM_VAR && sym->type == TYPE_STRING)
        sym->value.var.string_val = strdup(val);
}

void set_var_value_char(symrec *sym, char val) {
    if (sym->kind == SYM_VAR && sym->type == TYPE_CHAR)
        sym->value.var.char_val = val;
}

void set_var_value_bool(symrec *sym, bool val) {
    if (sym->kind == SYM_VAR && sym->type == TYPE_BOOL)
        sym->value.var.bool_val = val;
}

/* Function implementation setter */
void set_func_impl(symrec *sym, void *func_ptr) {
    if (sym->kind != SYM_FUNC) return;
    switch(sym->type) {
        case TYPE_INT:    sym->value.func.int_func = (int_func_t)func_ptr; break;
        case TYPE_DOUBLE: sym->value.func.double_func = (double_func_t)func_ptr; break;
        case TYPE_STRING: sym->value.func.string_func = (string_func_t)func_ptr; break;
        default: break;
    }
}

/* Cleanup resources */
void free_symbol_table(void) {
    symrec *ptr = sym_table;
    while (ptr != NULL) {
        symrec *next = ptr->next;
        free(ptr->name);
        if (ptr->kind == SYM_VAR && ptr->type == TYPE_STRING)
            free(ptr->value.var.string_val);
        free(ptr);
        ptr = next;
    }
    sym_table = NULL;
}