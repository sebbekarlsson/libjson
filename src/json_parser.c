#include "include/json_parser.h"
#include <stdio.h>
#include <string.h>

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
    json_parser->stack = (void*)0;
    json_parser->stack_size = 0;
    
    return json_parser;
}

/**
 * Deallocation method for the json parser,
 * this one also frees the lexer.
 *
 * @param json_parser_T* json_parser
 */
void json_parser_free(json_parser_T* json_parser)
{
    if (json_parser->lexer)
        json_lexer_free(json_parser->lexer);

    if (json_parser->stack != (void*)0)
    {
        for (int i = 0; i < json_parser->stack_size; i++)
            json_token_free(json_parser->stack[i]);

        free(json_parser->stack);
    }

    free(json_parser);
}

/**
 * Eat / expect a certain token type
 *
 * @param json_parser_T* json_parser
 * @param int type
 */
void json_parser_eat(json_parser_T* json_parser, int type)
{
    json_parser_stack(json_parser, json_parser->current_token);

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

/**
 * Add a json_token to the stack.
 * The `stack` in this case is a flat list of tokens that we can free later.
 *
 * @param json_parser_T* json_parser
 * @param json_token_T* json_token
 */
void json_parser_stack(json_parser_T* json_parser, json_token_T* json_token)
{
    json_parser->stack_size += 1;

    if (json_parser->stack == (void*)0)
        json_parser->stack = calloc(1, sizeof(struct JSON_AST_STRUCT*));
    else
        json_parser->stack = realloc(json_parser->stack, json_parser->stack_size * sizeof(struct JSON_AST_STRUCT*));

    json_parser->stack[json_parser->stack_size-1] = json_token;
}

/**
 * Main entry point of the json parser, parse json.
 *
 * @param json_parser_T* json_parser
 *
 * @return json_ast_T*
 */
json_ast_T* json_parser_parse(json_parser_T* json_parser)
{
    if (CURRENT_TOKEN() != (void*)0)
    {
        switch (CURRENT_TOKEN()->type)
        {
            case TOKEN_LBRACE: return json_parser_parse_key_value_list(json_parser); break;
            case TOKEN_LBRACKET: return json_parser_parse_list(json_parser); break;
            case TOKEN_STRING: return json_parser_parse_string(json_parser); break;
            case TOKEN_INTEGER: return json_parser_parse_integer(json_parser); break;
            case TOKEN_FLOAT: return json_parser_parse_float(json_parser); break;
            case TOKEN_COMMA: case TOKEN_RBRACE: case TOKEN_RBRACKET: case TOKEN_COLON: printf("%d\n", CURRENT_TOKEN()->type); break;
        }

        printf("[json_parser] Unexpected TOKEN(%d, %s)\n", CURRENT_TOKEN()->type, CURRENT_TOKEN()->value);
    }

    return (void*) 0;
}

/**
 * Parse a json key_value_list, example: `{"age": 12, "name": "john", ...}`
 *
 * @param json_parser_T* json_parser
 *
 * @return json_ast_T*
 */
json_ast_T* json_parser_parse_key_value_list(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_KEY_VALUE_LIST);
    ast->key_value_list_value = (void*)0;

    json_parser_eat(json_parser, TOKEN_LBRACE);

    if (CURRENT_TOKEN()->type != TOKEN_RBRACE)
    {
        ast->key_value_list_size += 1;
        ast->key_value_list_value = realloc(ast->key_value_list_value, ast->key_value_list_size * sizeof(struct JSON_AST_STRUCT));
        ast->key_value_list_value[ast->key_value_list_size-1] = json_parser_parse_key_value(json_parser);

        if (CURRENT_TOKEN()->type == TOKEN_COMMA)
            json_parser_eat(json_parser, TOKEN_COMMA);

        while (CURRENT_TOKEN()->type == TOKEN_STRING || CURRENT_TOKEN()->type == TOKEN_INTEGER || CURRENT_TOKEN()->type == TOKEN_FLOAT)
        {
            ast->key_value_list_size += 1;
            ast->key_value_list_value = realloc(ast->key_value_list_value, ast->key_value_list_size * sizeof(struct JSON_AST_STRUCT));
            ast->key_value_list_value[ast->key_value_list_size-1] = json_parser_parse_key_value(json_parser);

            if (CURRENT_TOKEN()->type == TOKEN_COMMA)
                json_parser_eat(json_parser, TOKEN_COMMA);

            if (CURRENT_TOKEN()->type == TOKEN_RBRACE)
                break;
        }
    }

    json_parser_eat(json_parser, TOKEN_RBRACE);

    return ast;
}

/**
 * Parse a json key_value, example: `"age": 12`
 *
 * @param json_parser_T* json_parser
 *
 * @return json_ast_T*
 */
json_ast_T* json_parser_parse_key_value(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_KEY_VALUE);

    ast->key_value_key = calloc(strlen(CURRENT_TOKEN()->value) + 1, sizeof(char));
    strcpy(ast->key_value_key, CURRENT_TOKEN()->value);

    json_parser_eat(json_parser, TOKEN_STRING);
    json_parser_eat(json_parser, TOKEN_COLON);
    ast->key_value_value = json_parser_parse(json_parser);

    return ast;
}

/**
 * Parse a json string, example: `"john doe"`
 *
 * @param json_parser_T* json_parser
 *
 * @return json_ast_T*
 */
json_ast_T* json_parser_parse_string(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_STRING);
    ast->string_value = calloc(strlen(CURRENT_TOKEN()->value) + 1, sizeof(char));
    strcpy(ast->string_value, CURRENT_TOKEN()->value);
    json_parser_eat(json_parser, TOKEN_STRING);

    return ast;
}

/**
 * Parse a json integer, example: `5`
 *
 * @param json_parser_T* json_parser
 *
 * @return json_ast_T*
 */
json_ast_T* json_parser_parse_integer(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_INTEGER);
    ast->integer_value = atoi(json_parser->current_token->value);
    json_parser_eat(json_parser, TOKEN_INTEGER);

    return ast;
}

/**
 * Parse a json float, example: `5.6`
 *
 * @param json_parser_T* json_parser
 *
 * @return json_ast_T*
 */
json_ast_T* json_parser_parse_float(json_parser_T* json_parser)
{
    json_ast_T* ast = init_json_ast(AST_FLOAT);
    ast->float_value = atof(json_parser->current_token->value);
    json_parser_eat(json_parser, TOKEN_FLOAT);

    return ast;
}

/**
 * Parse a json list `[ ... ]`
 *
 * @param json_parser-T* json_parser
 *
 * @return json_ast_T*
 */
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
