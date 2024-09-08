#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef struct AST_STRUCT
{
    enum
    {
        AST_VAR,
        AST_VAR_DEF,
        AST_STRING,
        AST_INT,
        AST_FLOAT,
        AST_MUL,
        AST_DIV,
        AST_ADD,
        AST_SUB,
        AST_ARRAY,
        AST_OBJECT,
        AST_OBJECT_PAIR,
        AST_FN_CALL,
        AST_BIN_OP,
        AST_AX,
        AST_LI,
        AST_LIAX,
        AST_GT,
        AST_LT,
        AST_LTE,
        AST_GTE,
        AST_EQ_EQ,
        AST_NEQ,
        AST_NOOP,
        AST_COMPOUND,
    } type;

    struct SCOPE_STRUCT *scope;

    // For variable definitions
    char *var_def_name;
    struct AST_STRUCT *var_def_value;

    // For variables
    char *var_name;

    // For function calls
    char *fn_name;
    struct AST_STRUCT **fn_call_args;
    size_t fn_call_args_size;

    // for if statements
    struct AST_STRUCT *condition;
    struct AST_STRUCT *then_branch;
    struct AST_STRUCT *else_if_branch;
    struct AST_STRUCT *else_branch;

    // For literals
    char *string_value;
    int int_value;
    float float_value;

    // For arrays
    struct AST_STRUCT **array_elements;
    size_t array_elements_size;

    // For objects (key-value pairs)
    struct AST_STRUCT **object_pairs;
    char *key;
    struct AST_STRUCT *value;
    size_t object_size;

    // For binary operations
    struct AST_STRUCT *left;
    struct AST_STRUCT *right;
    char *op;

    // AST compound for multiple statements
    struct AST_STRUCT **compound_value;
    size_t compound_value_size;
} AST;

AST *init_ast(int type);

#endif