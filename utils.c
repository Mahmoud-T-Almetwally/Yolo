// utils.c
#include "utils.h"
#include <stdio.h> // For stderr in error cases

const char* data_type_to_string(data_type t) {
    switch(t) {
        case TYPE_INT: return "int";
        case TYPE_DOUBLE: return "double";
        case TYPE_STRING: return "string";
        case TYPE_CHAR: return "char";
        case TYPE_BOOL: return "bool";
        case TYPE_VOID: return "void";
        case TYPE_FUNCTION: return "function"; // Usually used internally for symtab
        case TYPE_UNDEFINED: return "undefined"; // Error type
        default:
            fprintf(stderr, "Warning: Unknown data_type enum value: %d\n", t);
            return "UNKNOWN_TYPE";
    }
}

const char* operator_to_string(OperatorType op) {
    switch(op) {
        case OP_ASSIGN:  return "="; // Added for completeness, though often handled separately
        case OP_PLUS:    return "+";
        case OP_MINUS:   return "-";
        case OP_MUL:     return "*";
        case OP_DIV:     return "/";
        case OP_POWER:   return "**";
        case OP_EQ:      return "==";
        case OP_NEQ:     return "!=";
        case OP_LT:      return "<";
        case OP_GT:      return ">";
        case OP_LTE:     return "<=";
        case OP_GTE:     return ">=";
        case OP_AND:     return "&&";
        case OP_OR:      return "||";
        case OP_NEGATE:  return "- (unary)";
        case OP_NOT:     return "!";
        default:
             fprintf(stderr, "Warning: Unknown OperatorType enum value: %d\n", op);
             return "?OP?";
    }
}

const char* sym_kind_to_string(sym_kind k) {
    switch(k) {
        case SYM_VAR: return "Variable";
        case SYM_FUNC: return "Function";
        case SYM_PARAM: return "Parameter";
        default:
             fprintf(stderr, "Warning: Unknown sym_kind enum value: %d\n", k);
             return "UNKNOWN_KIND";
    }
}

// Simple error reporting utility
void compiler_error(int lineno, const char *format, ...) {
    va_list args;
    fprintf(stderr, "Error line %d: ", lineno);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
    // In a real compiler, you might want to set a flag here
    // to indicate an error occurred and potentially stop compilation phases.
}

void compiler_warning(int lineno, const char *format, ...) {
    va_list args;
    fprintf(stderr, "Warning line %d: ", lineno);
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}