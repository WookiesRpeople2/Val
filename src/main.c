#include <stdio.h>
#include "..\include\lexer.h"
#include "..\include\parser.h"
#include "..\include\visitor.h"
#include "..\include\io.h"

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        if (has_valid_extension(argv[1]))
        {
            char *contents = get_contents(argv[1]);
            Lexer *lexer = init_lexer(contents);
            Parser *parser = init_parser(lexer);
            AST *root = parse(parser, parser->scope);
            vist(root);
        }
    }
    else
    {
        char input[MAX_INPUT];
        printf("\nhello and welcome to val v%s\n", VERSION);
        printf("Created by WookiesRpeople2\n");
        printf("Type exit() to quit\n");
        while (1)
        {
            printf("---> ");
            fgets(input, MAX_INPUT, stdin);
            Lexer *lexer = init_lexer(input);
            Parser *parser = init_parser(lexer);
            AST *root = parse(parser, parser->scope);
            vist(root);
        }
    }
    return 0;
}