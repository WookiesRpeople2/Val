#include "../include/scope.h"
#include "../include/abstract_syntax_tree.h"
#include <stdio.h>
#include <string.h>

Scope *init_scope()
{
    Scope *scope = calloc(1, sizeof(struct SCOPE_STRUCT));
    scope->fn_defs = (void *)0;
    scope->fn_defs_size = 0;

    scope->var_defs = (void *)0;
    scope->var_defs_size = 0;

    return scope;
}

AST *add_fn_def(Scope *scope, AST *fn_def)
{
    scope->fn_defs_size += 1;

    if (scope->fn_defs == (void *)0)
    {
        scope->fn_defs = calloc(1, sizeof(struct AST_STRUCT **));
    }
    else
    {
        scope->fn_defs = realloc(scope->fn_defs, scope->fn_defs_size + sizeof(struct AST_STRUCT **));
    }

    scope->fn_defs[scope->fn_defs_size - 1] = fn_def;
    return fn_def;
}

AST *get_fn_def(Scope *scope, const char *fn_name)
{
    for (int i = 0; i < scope->fn_defs_size; i++)
    {
        AST *fn_def = scope->fn_defs[i];

        if (strcmp(fn_def->fn_name, fn_name) == 0)
        {
            return fn_def;
        }
    }

    return (void *)0;
}

AST *add_var_def(Scope *scope, AST *var_def)
{
    // printf("hit");
    // printf("Adding variable: %s\n", var_def->var_def_name);  // In add_var_def
    // printf("Looking for variable: %s\n", var_def->var_name); // In vist_var
    if (scope->var_defs == (void *)0)
    {
        scope->var_defs = calloc(1, sizeof(struct AST_STRUCT *));
        scope->var_defs[0] = var_def;
        scope->var_defs_size += 1;
    }
    else
    {
        scope->var_defs_size += 1;
        scope->var_defs = realloc(
            scope->var_defs,
            scope->var_defs_size * sizeof(struct AST_STRUCT *));
        scope->var_defs[scope->var_defs_size - 1] = var_def;
    }

    return var_def;
}

AST *get_var_def(Scope *scope, const char *vname)
{
    for (int i = 0; i < scope->var_defs_size; i++)
    {
        AST *var_def = scope->var_defs[i];
        if (strcmp(var_def->var_def_name, vname) == 0)
        {
            return var_def;
        }
    }

    return (void *)0;
}