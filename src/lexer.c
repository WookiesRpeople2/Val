#include "../include/lexer.h"
#include "../include/token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

Lexer *init_lexer(char *contents)
{
    Lexer *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->contents = contents;
    lexer->index = 0;
    lexer->c = contents[lexer->index];

    return lexer;
}

void next(Lexer *lexer)
{
    if (lexer->c != '\0' && lexer->index < strlen(lexer->contents))
    {
        lexer->index += 1;
        lexer->c = lexer->contents[lexer->index];
    }
}

void skip_space(Lexer *lexer)
{
    while (lexer->c == ' ' || lexer->c == 10)
    {
        next(lexer);
    }
}

void skip_comment(Lexer *lexer)
{
    if (lexer->contents[lexer->index + 1] == '/')
    {
        // Skip single-line comments
        while (lexer->c != '\n' && lexer->c != '\0')
        {
            next(lexer);
        }
    }
    else if (lexer->contents[lexer->index + 1] == '*')
    {
        // Skip multi-line comments
        next(lexer); // Skip '/'
        next(lexer); // Skip '*'

        while (lexer->c != '\0')
        {
            if (lexer->c == '*' && lexer->contents[lexer->index + 1] == '/')
            {
                next(lexer);
                next(lexer);
                break;
            }
            next(lexer);
        }
    }
}

Token *get_token(Lexer *lexer)
{
    while (lexer->c != '\0' && lexer->index < strlen(lexer->contents))
    {
        if (lexer->c == ' ' || lexer->c == 10)
            skip_space(lexer);

        if (isdigit(lexer->c))
            return collect_num(lexer);

        if (isalnum(lexer->c))
            return collect_variable(lexer);

        if (lexer->c == '"')
            return collect_string(lexer);

        if (lexer->c == '/' && (lexer->contents[lexer->index + 1] == '/' || lexer->contents[lexer->index + 1] == '*'))
        {
            skip_comment(lexer);
        }
        // printf("%s", lexer->contents[lexer->index + 1]);
        if (lexer->index + 1 < strlen(lexer->contents) && lexer->c == '-' && lexer->contents[lexer->index + 1] == '>')
        {
            return collect_equal(lexer);
        }

        switch (lexer->c)
        {
        // case '=':
        //     return next_with_token(lexer, init_token(TOKEN_EQUAL, char_as_string(lexer)));
        //     break;
        case ';':
            return next_with_token(lexer, init_token(TOKEN_SEMI_COLONE, char_as_string(lexer)));
            break;
        case '{':
            return next_with_token(lexer, init_token(TOKEN_SBRACKET, char_as_string(lexer)));
            break;
        case '}':
            return next_with_token(lexer, init_token(TOKEN_EBRACKET, char_as_string(lexer)));
            break;
        case '(':
            return next_with_token(lexer, init_token(TOKEN_SPAREN, char_as_string(lexer)));
            break;
        case ')':
            return next_with_token(lexer, init_token(TOKEN_EPAREN, char_as_string(lexer)));
            break;
        case '[':
            return next_with_token(lexer, init_token(TOEKN_SARR, char_as_string(lexer)));
            break;
        case ']':
            return next_with_token(lexer, init_token(TOKEN_EARR, char_as_string(lexer)));
            break;
        case '+':
            return next_with_token(lexer, init_token(TOKEN_PLUS, char_as_string(lexer)));
            break;
        case '/':
            return next_with_token(lexer, init_token(TOKEN_DIV, char_as_string(lexer)));
            break;
        case '*':
            return next_with_token(lexer, init_token(TOKEN_MUL, char_as_string(lexer)));
            break;
        case ',':
            return next_with_token(lexer, init_token(TOKEN_COMMA, char_as_string(lexer)));
            break;
        case ':':
            return next_with_token(lexer, init_token(TOKEN_DOUBLEP, char_as_string(lexer)));
            break;
        case '.':
            return next_with_token(lexer, init_token(TOKEN_DOT, char_as_string(lexer)));
            break;
        case '-':
            return next_with_token(lexer, init_token(TOKEN_DASH, char_as_string(lexer)));
            break;
            // case '>':
            //     return next_with_token(lexer, init_token(TOKEN_ARROW, char_as_string(lexer)));
            //     break;
        }
    }

    return init_token(TOKEN_EOF, "\0");
}

Token *collect_string(Lexer *lexer)
{
    next(lexer); // starts with " so we advance to get the nex position in the string for example "x we would get x

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c != '"' && lexer->c != '\'')
    {
        char *str = char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(str) + 1) * sizeof(char));
        strcat(value, str);

        next(lexer);
    }

    next(lexer); // break out of the string the ending quoet "

    return init_token(TOKEN_STRING, value);
}

Token *collect_num(Lexer *lexer)
{
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    // Collect the integer part
    while (isdigit(lexer->c))
    {
        char *num = char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(num) + 1) * sizeof(char));
        strcat(value, num);

        next(lexer);
    }

    // Check for decimal point
    if (lexer->c == '.')
    {
        char *dot = char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(dot) + 1) * sizeof(char));
        strcat(value, dot);

        next(lexer);

        // Collect the fractional part
        while (isdigit(lexer->c))
        {
            char *num = char_as_string(lexer);
            value = realloc(value, (strlen(value) + strlen(num) + 1) * sizeof(char));
            strcat(value, num);

            next(lexer);
        }
    }
    else
    {
        return init_token(TOKEN_INT, value);
    }

    return init_token(TOKEN_FLOAT, value);
}

Token *collect_equal(Lexer *lexer)
{
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lexer->c == '-' && lexer->contents[lexer->index + 1] == '>')
    {
        char *equal = char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(equal) + 1) * sizeof(char));
        strcat(value, equal);

        next(lexer);
    }

    next(lexer);

    return init_token(TOKEN_EQUAL, value);
}

Token *collect_variable(Lexer *lexer)
{
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lexer->c) || lexer->c == '_')
    {
        char *s = char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        next(lexer);
    }
    return collect_variable_type(value);
}

Token *collect_variable_type(char *value)
{
    if (strcmp(value, "euphoria") == 0)
    {
        return init_token(TOKEN_EUPHORIA, value);
    }
    if (strcmp(value, "elixir") == 0)
    {
        return init_token(TOKEN_ELXIR, value);
    }
    if (strcmp(value, "lassitude") == 0)
    {
        return init_token(TOKEN_LASSITUDE, value);
    }
    if (strcmp(value, "sequentia") == 0)
    {
        return init_token(TOKEN_SEQUENTIA, value);
    }
    if (strcmp(value, "fulminare") == 0)
    {
        return init_token(TOKEN_FULMINARE, value);
    }

    return init_token(TOKEN_ID, value);
}

Token *next_with_token(Lexer *lexer, Token *token)
{
    next(lexer);

    return token;
}

char *char_as_string(Lexer *lexer)
{
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0'; // null byte terminated

    return str;
}
