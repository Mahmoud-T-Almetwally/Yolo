// utils.h
#ifndef UTILS_H
#define UTILS_H

#include "types.h"    // Include types for enums
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>   // For va_list in error reporting

// --- Enum to String Helpers ---
const char* data_type_to_string(data_type t);
const char* operator_to_string(OperatorType op);
const char* sym_kind_to_string(sym_kind k);

// --- Error Reporting ---
// Use lineno = 0 if line number is not applicable/available
void compiler_error(int lineno, const char *format, ...);
void compiler_warning(int lineno, const char *format, ...);

// Macro for AST specific errors (could add file context later)
#define AST_ERROR(node, ...) compiler_error((node) ? (node)->lineno : 0, __VA_ARGS__)
#define AST_WARN(node, ...) compiler_warning((node) ? (node)->lineno : 0, __VA_ARGS__)
#define PARSER_ERROR(lineno, ...) compiler_error(lineno, __VA_ARGS__) // For errors during parsing before AST exists

#endif // UTILS_H