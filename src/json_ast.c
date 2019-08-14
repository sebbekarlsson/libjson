#include "include/json_ast.h"


/**
 * Create a new JSON AST Node.
 *
 * @param int type
 *
 * @return json_ast_T*
 */
json_ast_T* init_json_ast(int type)
{
    json_ast_T* json_ast = calloc(1, sizeof(struct JSON_AST_STRUCT));
    json_ast->type = type; 
    json_ast->string_value = (void*)0;
    json_ast->list_value = (void*)0;
    json_ast->list_size = 0;
    json_ast->key_value_list_value = (void*)0;
    json_ast->key_value_list_size = 0;

    return json_ast;
}
