#include "../include/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Parser *init_parser(Lexer *lexer)
{
    Parser *parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = get_token(lexer);
    parser->prev_token = parser->current_token;
    parser->scope = init_scope();

    return parser;
}

void take(Parser *parser, int token_type)
{
    // printf("Current token: %s (%d), Expected: %d\n", parser->current_token->value, parser->current_token->type, token_type);
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = get_token(parser->lexer);
        // printf("Token matched: %d (%s)\n", parser->prev_token->type, parser->prev_token->value);
        // printf("Token Type: %d, Token Value: %s\n", parser->current_token->type, parser->current_token->value);
    }
    else
    {
        printf("Unexpected token %s, expected %d\n", parser->current_token->value, token_type);
        exit(1);
    }
}

AST *parse(Parser *parser, Scope *scope)
{
    // printf("Current token: %s (%d)", parser->current_token->value, parser->current_token->type);
    return parse_compounds(parser, scope);
}

AST *parse_compound(Parser *parser, Scope *scope)
{
    // printf("Current token: %s (%d)\n", parser->current_token->value, parser->current_token->type);
    switch (parser->current_token->type)
    {
    case TOKEN_ID:
        return parse_var(parser, scope);
    case TOKEN_EUPHORIA:
    case TOKEN_ELXIR:
    case TOKEN_LASSITUDE:
    case TOKEN_SEQUENTIA:
    case TOKEN_FULMINARE:
        return parse_var_def(parser, scope);
    }

    return init_ast(AST_NOOP);
}

AST *parse_compounds(Parser *parser, Scope *scope)
{
    AST *compound = init_ast(AST_COMPOUND);
    compound->scope = scope;
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT *));

    AST *ast_statement = parse_compound(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement;
    compound->compound_value_size += 1;

    while (parser->current_token->type == TOKEN_SEMI_COLONE)
    {
        take(parser, TOKEN_SEMI_COLONE);
        AST *ast_statement = parse_compound(parser, scope);
        compound->compound_value_size += 1;
        compound->compound_value = realloc(compound->compound_value, compound->compound_value_size * sizeof(struct AST_STRUCT *));
        compound->compound_value[compound->compound_value_size - 1] = ast_statement;
    }

    return compound;
}

AST *parse_expr(Parser *parser, Scope *scope)
{
    // printf("(%d, %s)", parser->current_token->type, parser->current_token->value);
    switch (parser->current_token->type)
    {
    case TOKEN_ID:
        return parse_var(parser, scope);
    case TOKEN_STRING:
        return parse_string(parser, scope);
    case TOKEN_INT:
    case TOKEN_FLOAT:
        return helper_expr(parser, scope);
    case TOKEN_SBRACKET:
        return parse_object(parser, scope);
    case TOEKN_SARR:
        return parse_array(parser, scope);
    case TOKEN_MUL:
    case TOKEN_DIV:
        return parse_term(parser, scope);
    case TOKEN_PLUS:
    case TOKEN_DASH:
        return helper_expr(parser, scope);
    default:
        return init_ast(AST_NOOP);
    }
}

AST *parse_factor(Parser *parser, Scope *scope)
{
    AST *ast = NULL;

    switch (parser->current_token->type)
    {
    case TOKEN_ID:
        return parse_var(parser, scope);
    case TOKEN_INT:
        ast = parse_int(parser, scope);
        break;
    case TOKEN_FLOAT:
        ast = parse_float(parser, scope);
        break;
    case TOKEN_SPAREN:
        take(parser, TOKEN_SPAREN);
        ast = parse_expr(parser, scope);
        take(parser, TOKEN_EPAREN);
        break;
    }

    return ast;
}

AST *helper_expr(Parser *parser, Scope *scope)
{
    AST *left = parse_term(parser, scope);

    while (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_DASH)
    {
        AST *op = NULL;
        if (parser->current_token->type == TOKEN_PLUS)
        {
            op = init_ast(AST_ADD);
        }
        else if (parser->current_token->type == TOKEN_DASH)
        {
            op = init_ast(AST_SUB);
        }

        take(parser, parser->current_token->type);

        // printf("hit babyyyy");
        op->left = left;
        op->right = parse_term(parser, scope);

        left = op;
    }

    return left;
}

AST *parse_term(Parser *parser, Scope *scope)
{
    AST *left = parse_factor(parser, scope);

    while (parser->current_token->type == TOKEN_MUL || parser->current_token->type == TOKEN_DIV)
    {
        AST *op = NULL;
        if (parser->current_token->type == TOKEN_MUL)
        {
            op = init_ast(AST_MUL);
        }
        else if (parser->current_token->type == TOKEN_DIV)
        {
            op = init_ast(AST_DIV);
        }

        take(parser, parser->current_token->type); // consume the operator

        op->left = left;
        op->right = parse_factor(parser, scope); // parse the next factor

        left = op; // the result becomes the new left side for potential further operations
    }

    return left;
}

AST *parse_fn_call(Parser *parser, Scope *scope)
{
    AST *ast = init_ast(AST_FN_CALL);
    ast->fn_name = parser->prev_token->value;

    take(parser, TOKEN_SPAREN);

    ast->fn_call_args = calloc(1, sizeof(struct AST_STRUCT *));
    AST *ast_expr = parse_expr(parser, scope);
    ast->fn_call_args[0] = ast_expr;
    ast->fn_call_args_size += 1;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        take(parser, TOKEN_COMMA);
        AST *ast_expr = parse_expr(parser, scope);

        ast->fn_call_args_size += 1;
        ast->fn_call_args = realloc(ast->fn_call_args, ast->fn_call_args_size * sizeof(struct AST_STRUCT *));
        ast->fn_call_args[ast->fn_call_args_size - 1] = ast_expr;
    }

    take(parser, TOKEN_EPAREN);

    ast->scope = scope;

    return ast;
}

AST *parse_var(Parser *parser, Scope *scope)
{
    // printf("Parsing var_def: Current token = %s (%d)\n", parser->current_token->value, parser->current_token->type);

    char *token_value = parser->current_token->value;
    // printf("PARSER(searching for value: %s)\n", token_value);
    take(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_SPAREN)
        return parse_fn_call(parser, scope);

    AST *ast = init_ast(AST_VAR);
    ast->var_name = token_value;

    ast->scope = scope;

    return ast;
}

AST *parse_var_def(Parser *parser, Scope *scope)
{
    take(parser, parser->current_token->type);
    char *var_def_name = parser->current_token->value;

    take(parser, TOKEN_ID);
    take(parser, TOKEN_EQUAL);
    AST *var_def_value = parse_expr(parser, scope);

    AST *var_def = init_ast(AST_VAR_DEF);
    var_def->var_def_name = var_def_name;
    var_def->var_def_value = var_def_value;

    // printf("PARSER(Adding variable: %s)\n", var_def->var_def_name); // In add_var_def

    var_def->scope = scope;

    return var_def;
}

AST *parse_string(Parser *parser, Scope *scope)
{
    AST *ast = init_ast(AST_STRING);
    ast->string_value = parser->current_token->value;

    take(parser, TOKEN_STRING);
    ast->scope = scope;

    return ast;
}

AST *parse_int(Parser *parser, Scope *scope)
{
    AST *ast = init_ast(AST_INT);
    ast->int_value = atoi(parser->current_token->value);

    take(parser, TOKEN_INT);
    ast->scope = scope;

    return ast;
}

AST *parse_float(Parser *parser, Scope *scope)
{
    AST *ast = init_ast(AST_FLOAT);
    ast->float_value = atof(parser->current_token->value);

    take(parser, TOKEN_FLOAT);
    ast->scope = scope;

    return ast;
}

AST *parse_array(Parser *parser, Scope *scope)
{
    take(parser, TOEKN_SARR); // Consume the '[' token

    AST *array_ast = init_ast(AST_ARRAY);                               // Create a new AST node for the array
    array_ast->array_elements = calloc(1, sizeof(struct AST_STRUCT *)); // Initialize elements
    array_ast->array_elements_size = 0;

    // Parse the first element
    AST *element = parse_expr(parser, scope);
    array_ast->array_elements[0] = element;
    array_ast->array_elements_size += 1;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        take(parser, TOKEN_COMMA); // Consume the ',' token
        AST *element = parse_expr(parser, scope);

        // Resize the elements array
        array_ast->array_elements_size += 1;
        array_ast->array_elements = realloc(array_ast->array_elements,
                                            array_ast->array_elements_size * sizeof(struct AST_STRUCT *));
        array_ast->array_elements[array_ast->array_elements_size - 1] = element;
    }

    take(parser, TOKEN_EARR); // Consume the ']' token
    array_ast->scope = scope;

    return array_ast;
}

AST *parse_object(Parser *parser, Scope *scope)
{
    take(parser, TOKEN_SBRACKET); // Consume the '{' token

    AST *object_ast = init_ast(AST_OBJECT);                            // Create a new AST node for the object
    object_ast->object_pairs = calloc(1, sizeof(struct AST_STRUCT *)); // Initialize pairs
    object_ast->object_size = 0;

    // Parse the first key-value pair
    char *key = parser->current_token->value; // Get the key
    // printf("\n%s\n", parser->current_token->value);
    take(parser, parser->current_token->type); // Consume the key token
    take(parser, TOKEN_DOUBLEP);               // Consume the ':' token
    AST *value = parse_expr(parser, scope);    // Parse the value

    // Create a key-value pair AST node
    AST *pair = init_ast(AST_OBJECT_PAIR);
    pair->key = key;
    pair->value = value;

    object_ast->object_pairs[0] = pair;
    object_ast->object_size += 1;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        take(parser, TOKEN_COMMA);
        char *key = parser->current_token->value;
        take(parser, TOKEN_ID);
        take(parser, TOKEN_DOUBLEP);
        AST *value = parse_expr(parser, scope);

        // Resize the pairs array
        object_ast->object_size += 1;
        object_ast->object_pairs = realloc(object_ast->object_pairs,
                                           object_ast->object_size * sizeof(struct AST_STRUCT *));
        // Create and store the new key-value pair
        AST *pair = init_ast(AST_OBJECT_PAIR);
        pair->key = key;
        pair->value = value;
        object_ast->object_pairs[object_ast->object_size - 1] = pair;
    }

    take(parser, TOKEN_EBRACKET);
    object_ast->scope = scope;

    return object_ast;
}