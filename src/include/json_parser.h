#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include "json_lexer.h"
#include "json_ast.h"


typedef struct JSON_PARSER_STRUCT
{
    json_lexer_T* lexer;
    json_token_T* current_token;
    json_token_T** stack;
    size_t stack_size;
} json_parser_T;

json_parser_T* init_json_parser(json_lexer_T* lexer);

void json_parser_free(json_parser_T* json_parser);

void json_parser_eat(json_parser_T* json_parser, int type);

void json_parser_stack(json_parser_T* json_parser, json_token_T* json_token);

json_ast_T* json_parser_parse(json_parser_T* json_parser);

json_ast_T* json_parser_parse_key_value_list(json_parser_T* json_parser);

json_ast_T* json_parser_parse_key_value(json_parser_T* json_parser);

json_ast_T* json_parser_parse_string(json_parser_T* json_parser);

json_ast_T* json_parser_parse_integer(json_parser_T* json_parser);

json_ast_T* json_parser_parse_float(json_parser_T* json_parser);

json_ast_T* json_parser_parse_list(json_parser_T* json_parser);
#endif
