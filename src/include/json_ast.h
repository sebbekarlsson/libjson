#ifndef JSON_JSON_AST_H
#define JSON_JSON_AST_H
#include <stdlib.h>


typedef struct JSON_JSON_AST_STRUCT
{
    enum
    {
        JSON_AST_KEY_VALUE,
        JSON_AST_KEY_VALUE_LIST,
        JSON_AST_STRING,
        JSON_AST_INTEGER,
        JSON_AST_FLOAT,
        JSON_AST_LIST
    } type;
    char* string_value;
    int integer_value;
    float float_value;

    struct JSON_JSON_AST_STRUCT** list_value;
    size_t list_size;

    char* key_value_key;
    struct JSON_JSON_AST_STRUCT* key_value_value;

    struct JSON_JSON_AST_STRUCT** key_value_list_value;
    size_t key_value_list_size;
} json_ast_T;

json_ast_T* init_json_ast(int type);

void json_ast_free(json_ast_T* json_ast);
#endif
