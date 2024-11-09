#ifndef TOKEN_H
#define TOKEN_H

typedef struct TokenStruct
{
    enum
    {
        TOKEN_ID,
        TOKEN_EUPHORIA,
        TOKEN_ELXIR,
        TOKEN_LASSITUDE,
        TOKEN_SEQUENTIA,
        TOKEN_FULMINARE,
        TOKEN_BLUE,
        TOKEN_EQUAL,
        TOKEN_EQUAL_EQUAL,
        TOKEN_STRING,
        TOKEN_INT,
        TOKEN_FLOAT,
        TOKEN_SEMI_COLONE,
        TOKEN_SBRACKET,
        TOKEN_EBRACKET,
        TOKEN_SPAREN,
        TOKEN_EPAREN,
        TOKEN_SARR,
        TOKEN_EARR,
        TOKEN_COMMA,
        TOKEN_DOUBLEP,
        TOKEN_DOT,
        TOKEN_DASH,
        TOKEN_PLUS,
        TOKEN_MUL,
        TOKEN_DIV,
        TOKEN_AX,
        TOKEN_LI,
        TOKEN_LIAX,
        TOKEN_PETRICHOR,
        TOKEN_INCENDIARY,
        TOKEN_INURE,
        TOKEN_GT,
        TOKEN_LT,
        TOKEN_GTE,
        TOKEN_LTE,
        TOKEN_NEQ,
        TOKEN_EOF,
    } type;

    char *value;
} Token;

Token *init_token(int type, char *value);

#endif // TOKEN_H