#ifndef VISITOR_H
#define VISITOR_H

#include "abstract_syntax_tree.h"

// typedef struct VISITOR_STRUCT
// {
//     struct AST_STRUCT **var_defs;
//     size_t var_defs_size;
// } Vistor;

AST *vist(AST *child);

AST *vist_var_def(AST *child);

AST *vist_var(AST *child);

AST *vist_var_reassign(AST *child);

AST *vist_fn_call(AST *child);

AST *vist_conditional(AST *child);

AST *vist_relation(AST *child);

AST *vist_loop_petrichor(AST *child);

AST *vist_loop_incendiary(AST *child);

AST *vist_string(AST *child);

AST *vist_int(AST *child);

AST *vist_float(AST *child);

AST *vist_array(AST *child);

AST *vist_object(AST *child);

AST *vist_binop(AST *child);

AST *vist_compund(AST *child);

#endif