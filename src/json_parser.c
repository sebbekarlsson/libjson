#include "include/json_parser.h"
#include <stdio.h>

#define CURRENT_TOKEN()\
    json_parser->current_token


/**
 * Create a new json_parser
 *
 * @param json_lexer_T* lexer
 */
json_parser_T* init_json_parser(json_lexer_T* lexer)
{
    json_parser_T* json_parser = calloc(1, sizeof(struct JSON_PARSER_STRUCT));
    json_parser->lexer = lexer;
    json_parser->current_token = json_lexer_get_next_token(json_parser->lexer);
    
    return json_parser;
}

void json_parser_free(json_parser_T* json_parser)
{
    if (json_parser->lexer)
        json_lexer_free(json_parser->lexer);
    if (json_parser->current_token)
        json_token_free(json_parser->current_token);

    free(json_parser);
}

void json_parser_eat(json_parser_T* json_parser, int type)
{
    if (json_parser->current_token->type != type)
    {
        printf(
            "[json_parser] Unexpected TOKEN(%d, %s)\nWas expecting: %d.\n(lexer->i == %d)\n",
            json_parser->current_token->type,
            json_parser->current_token->value,
            type,
            json_parser->lexer->i
        );
        exit(1);
    }

    json_parser->current_token = json_lexer_get_next_token(json_parser->lexer);
}

json_ast_T* json_parser_parse(json_parser_T* json_parser)
{
    switch (CURRENT_TOKEN()->type)
    {
        case TOKEN_LBRACE: return json_parser_parse_key_value_list(json_parser); break;
        case TOKEN_LBRACKET: return json_parser_parse_list(json_parser); break;
        case TOKEN_STRING: return json_parser_parse_string(json_parser); break;
        case TOKEN_INTEGER: return json_parser_parse_integer(json_parser); break;
        case TOKEN_FLOAT: return json_parser_parse_float(json_parser); break;
        case TOKEN_EOF: return (void*)0; break;
    }

    printf("[json_parser] Unexpected TOKEN(%d, %s)\n", CURRENT_TOKEN()->type, CURRENT_TOKEN()->value);
}

json_ast_T* json_parser_parse_key_value_list(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_KEY_VALUE_LIST);
    ast->key_value_list_value = (void*)0;

    json_parser_eat(json_parser, TOKEN_LBRACE);

    ast->key_value_list_size += 1;
    ast->key_value_list_value = realloc(ast->key_value_list_value, ast->key_value_list_size * sizeof(struct JSON_AST_STRUCT));
    ast->key_value_list_value[ast->key_value_list_size-1] = json_parser_parse_key_value(json_parser);

    if (CURRENT_TOKEN()->type == TOKEN_COMMA)
        json_parser_eat(json_parser, TOKEN_COMMA);

    while (CURRENT_TOKEN()->type != TOKEN_RBRACE)
    {
        ast->key_value_list_size += 1;
        ast->key_value_list_value = realloc(ast->key_value_list_value, ast->key_value_list_size * sizeof(struct JSON_AST_STRUCT));
        ast->key_value_list_value[ast->key_value_list_size-1] = json_parser_parse_key_value(json_parser);
        
        if (CURRENT_TOKEN()->type == TOKEN_COMMA)
            json_parser_eat(json_parser, TOKEN_COMMA);
    }

    json_parser_eat(json_parser, TOKEN_RBRACE);

    return ast;
}

json_ast_T* json_parser_parse_key_value(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_KEY_VALUE);
    ast->key_value_key = CURRENT_TOKEN()->value;
    json_parser_eat(json_parser, TOKEN_STRING);
    json_parser_eat(json_parser, TOKEN_COLON);
    ast->key_value_value = json_parser_parse(json_parser);

    return ast;
}

json_ast_T* json_parser_parse_string(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_STRING);
    ast->string_value = json_parser->current_token->value;
    json_parser_eat(json_parser, TOKEN_STRING);

    return ast;
}

json_ast_T* json_parser_parse_integer(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_INTEGER);
    ast->integer_value = atoi(json_parser->current_token->value);
    json_parser_eat(json_parser, TOKEN_INTEGER);

    return ast;
}

json_ast_T* json_parser_parse_float(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_FLOAT);
    ast->float_value = atof(json_parser->current_token->value);
    json_parser_eat(json_parser, TOKEN_FLOAT);

    return ast;
}

json_ast_T* json_parser_parse_list(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_LIST);
    ast->list_value = (void*)0;

    json_parser_eat(json_parser, TOKEN_LBRACKET);

    ast->list_size += 1;
    ast->list_value = realloc(ast->list_value, ast->list_size * sizeof(struct JSON_AST_STRUCT));
    ast->list_value[ast->list_size-1] = json_parser_parse(json_parser);

    while (CURRENT_TOKEN()->type == TOKEN_COMMA)
    {
        json_parser_eat(json_parser, TOKEN_COMMA);

        ast->list_size += 1;
        ast->list_value = realloc(ast->list_value, ast->list_size * sizeof(struct JSON_AST_STRUCT));
        ast->list_value[ast->list_size-1] = json_parser_parse(json_parser);
    }

    json_parser_eat(json_parser, TOKEN_RBRACKET);

    return ast;
}
