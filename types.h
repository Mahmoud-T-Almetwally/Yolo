// types.h
#ifndef TYPES_H
#define TYPES_H

typedef enum {
    NODE_UNKNOWN,
    
    NODE_PROGRAM,
    
    NODE_FUNC_DEF,
    NODE_FUNC_CALL_ARGS,
    NODE_VAR_DECL,
    NODE_CONST_DECL,
    NODE_ASSIGN_STMT,
    NODE_IF_STMT,
    NODE_PRINT_STMT,
    NODE_RETURN_STMT,
    
    NODE_BINOP_EXPR,
    NODE_UNOP_EXPR,
    NODE_FUNC_CALL,
    NODE_IDENT_EXPR, 

    NODE_INT_LITERAL,
    NODE_DOUBLE_LITERAL,
    NODE_STRING_LITERAL,
    NODE_CHAR_LITERAL,
    NODE_BOOL_LITERAL
} NodeType;

// --- Operator Types ---
typedef enum {
    OP_ASSIGN,
    OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_POWER,
    OP_EQ, OP_NEQ, OP_LT, OP_GT, OP_LTE, OP_GTE,
    OP_AND, OP_OR,
    OP_NEGATE, OP_NOT
} OperatorType;

// --- Symbol Kinds ---
typedef enum {
    SYM_VAR,
    SYM_FUNC,
    SYM_PARAM
} sym_kind;

// --- Data Types ---
typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_BOOL,
    TYPE_VOID,
    TYPE_UNDEFINED,
    TYPE_FUNCTION 
} data_type;

#endif // TYPES_H