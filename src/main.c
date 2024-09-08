#include <stdio.h>
#include "..\include\lexer.h"
#include "..\include\parser.h"
#include "..\include\visitor.h"
#include "..\include\io.h"

int main(int argc, char *argv[])
{
    // if (argc >= 2)
    // {
    //     if (has_valid_extension(argv[1]))
    //     {
    //         char *contents = get_contents(argv[1]);
    //         Lexer *lexer = init_lexer(contents);
    //         Parser *parser = init_parser(lexer);
    //         AST *root = parse(parser, parser->scope);
    //         vist(root);
    //     }
    // }
    // else
    // {
    //     char input[MAX_INPUT];
    //     printf("\nhello and welcome to val v%s\n", VERSION);
    //     printf("Created by WookiesRpeople2\n");
    //     printf("Type exit() to quit\n");
    //     while (1)
    //     {
    //         printf("---> ");
    //         fgets(input, MAX_INPUT, stdin);
    //         Lexer *lexer = init_lexer(input);
    //         Parser *parser = init_parser(lexer);
    //         AST *root = parse(parser, parser->scope);
    //         vist(root);
    //     }
    // }

    Lexer *lexer = init_lexer(
        "elixir year_born -> 2010;\n"
        "ax(2024 - year_born < 22){\n"
        "ebullience(\"You are born in the the 20th centary\");\n"
        "}liax(2024 - year_born >-> 22 ){\n"
        "ebullience(\"You are born before the 19th centary\");\n"
        "}\n"
        "ebullience(\"you are a whole lot older\");\n"
        "}\n");

    // Token *token = (void *)0;

    // while ((token = get_token(lexer)) != (void *)0)
    // {
    //     printf("TOKEN(%d, %s)\n", token->type, token->value);
    // }

    Parser *parser = init_parser(lexer);
    AST *root = parse(parser, parser->scope);
    vist(root);
    return 0;
}