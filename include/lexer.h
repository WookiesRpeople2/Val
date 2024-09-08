#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct LEXER_STRUCT
{
    char c;
    unsigned int index;
    char *contents;
} Lexer;

#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\n')

Lexer *init_lexer(char *contents);

void next(Lexer *lexer);

void skip_space(Lexer *lexer);

void skip_comment(Lexer *lexer);

Token *get_token(Lexer *lexer);

Token *collect_string(Lexer *lexer);

Token *collect_num(Lexer *lexer);

Token *collect_equal(Lexer *lexer);

Token *collect_variable(Lexer *lexer);

Token *collect_variable_type(char *value);

Token *collect_if(Lexer *lexer);

Token *collect_else_if(Lexer *lexer);

Token *collect_else(Lexer *lexer);

Token *collect_eq_eq(Lexer *lexer);

Token *collect_neq(Lexer *lexer);

Token *collect_gte(Lexer *lexer);

Token *collect_lte(Lexer *lexer);

Token *next_with_token(Lexer *lexer, Token *token);

char *char_as_string(Lexer *lexer);

int *get_current_char_as_int(Lexer *lexer);

#endif