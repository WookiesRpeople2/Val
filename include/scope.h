#ifndef SCOPE_H
#define SCOPE_H

#include "abstract_syntax_tree.h"

typedef struct SCOPE_STRUCT
{
    AST **fn_defs;
    size_t fn_defs_size;

    AST **var_defs;
    size_t var_defs_size;
} Scope;

Scope *init_scope();

AST *add_fn_def(Scope *scope, AST *fn_def);

AST *get_fn_def(Scope *scope, const char *fn_name);

AST *add_var_def(Scope *scope, AST *var_def);

AST *get_var_def(Scope *scope, const char *vname);

AST *reassign_var_def(Scope *scope, const char *vname, AST *new_var_def);

#endif