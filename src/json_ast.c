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

/**
 * Deallocate a json_ast
 *
 * @param json_ast_T* json_ast
 */
void json_ast_free(json_ast_T* json_ast)
{
    switch (json_ast->type)
    {
        case AST_STRING: free(json_ast->string_value); break;
        case AST_LIST: {
            for (int i = 0; i < json_ast->list_size; i++)
                json_ast_free(json_ast->list_value[i]);

            free(json_ast->list_value);
        } break;
        case AST_KEY_VALUE_LIST: {
            for (int i = 0; i < json_ast->key_value_list_size; i++)
            {
                json_ast_T* kv = json_ast->key_value_list_value[i];
                json_ast_free(kv);
            }
            free(json_ast->key_value_list_value);
        } break;
        case AST_KEY_VALUE: {
            if (json_ast->key_value_key)
                free(json_ast->key_value_key);
            if (json_ast->key_value_value)
                json_ast_free(json_ast->key_value_value);            
        } break;
        case AST_FLOAT: case AST_INTEGER: /* silence */ break;
    }

    free(json_ast);
}
