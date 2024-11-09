#include "../include/abstract_syntax_tree.h"
#include <stdlib.h>

AST *init_ast(int type)
{
    AST *ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;
    ast->scope = (void *)0;

    // For variable definitions
    ast->var_def_name = (void *)0;
    ast->var_def_value = (void *)0;
    ast->var_redef_var = (void *)0;

    // For variables
    ast->var_name = (void *)0;

    // For function calls
    ast->fn_name = (void *)0;
    ast->fn_call_args = (void *)0;
    ast->fn_call_args_size = 0;

    // for if statements
    ast->condition = (void *)0;
    ast->then_branch = (void *)0;
    ast->else_if_branch = (void *)0;
    ast->else_branch = (void *)0;

    // for loops
    ast->loop_init = (void *)0;
    ast->loop_body = (void *)0;
    ast->loop_break = false;

    // For literals
    ast->string_value = (void *)0;
    ast->int_value = 0;
    ast->float_value = 0.0f;

    // For arrays
    ast->array_elements = (void *)0;
    ast->array_index = (void *)0;
    ast->array_elements_size = 0;

    // For objects (key-value pairs)
    ast->object_pairs = (void *)0;
    ast->key = (void *)0;
    ast->value = (void *)0;
    ast->object_size = 0;

    // For binary operations
    ast->left = (void *)0;
    ast->right = (void *)0;
    ast->op = (void *)0;

    // AST compound for multiple statements
    ast->compound_value = (void *)0;
    ast->compound_value_size = 0;

    return ast;
}