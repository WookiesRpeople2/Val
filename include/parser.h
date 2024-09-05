#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "abstract_syntax_tree.h"
#include "scope.h"

typedef struct PARSER_STRUCT
{
    Lexer *lexer;
    Token *current_token;
    Token *prev_token;
    Scope *scope;
} Parser;

Parser *init_parser(Lexer *lexer);

void take(Parser *parser, int token_type);

AST *parse(Parser *parser, Scope *scope);

AST *parse_compound(Parser *parser, Scope *scope);

AST *parse_compounds(Parser *parser, Scope *scope);

AST *parse_expr(Parser *parser, Scope *scope);

AST *parse_factor(Parser *parser, Scope *scope);

AST *helper_expr(Parser *parser, Scope *scope);

AST *parse_term(Parser *parser, Scope *scope);

AST *parse_fn_call(Parser *parser, Scope *scope);

AST *parse_var(Parser *parser, Scope *scope);

AST *parse_var_def(Parser *parser, Scope *scope);

AST *parse_string(Parser *parser, Scope *scope);

AST *parse_int(Parser *parser, Scope *scope);

AST *parse_float(Parser *parser, Scope *scope);

AST *parse_array(Parser *parser, Scope *scope);

AST *parse_object(Parser *parser, Scope *scope);

#endif