#include "include/json_visitor.h"
#include <stdio.h>


/**
 * Main entry point to visit a json_ast node.
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit(json_ast_T* json_ast)
{
    switch (json_ast->type)
    {
        case AST_KEY_VALUE: return json_visitor_visit_key_value(json_ast); break;
        case AST_KEY_VALUE_LIST: return json_visitor_visit_key_value_list(json_ast); break;
        case AST_LIST: return json_visitor_visit_list(json_ast); break;
        case AST_STRING: return json_visitor_visit_string(json_ast); break;
        case AST_INTEGER: return json_visitor_visit_integer(json_ast); break;
        case AST_FLOAT: return json_visitor_visit_float(json_ast); break;
    }

    printf("[json_visitor] Unhandled json_ast->type: `%d`\n", json_ast->type);
    exit(1);
}

/**
 * Visit a key_value ast
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit_key_value(json_ast_T* json_ast)
{
    char* key = json_ast->key_value_key;
    printf("\"%s\": ", json_ast->key_value_key);
    json_visitor_visit(json_ast->key_value_value);
}

/**
 * Visit a key_value_list ast
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit_key_value_list(json_ast_T* json_ast)
{
    printf("{");

    for (int i = 0; i < json_ast->key_value_list_size; i++)
    {
        json_visitor_visit(json_ast->key_value_list_value[i]);

        if (i < json_ast->key_value_list_size - 1)
            printf(",");
    }

    printf("}");
}

/**
 * Visit a list ast
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit_list(json_ast_T* json_ast)
{
    printf("[");

    for (int i = 0; i < json_ast->list_size; i++)
    {
        json_visitor_visit(json_ast->list_value[i]);

        if (i < json_ast->list_size - 1)
            printf(",");
    }

    printf("]");
}

/**
 * Visit a string ast
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit_string(json_ast_T* json_ast)
{
    printf("\"%s\"", json_ast->string_value);
}

/**
 * Visit a integer ast
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit_integer(json_ast_T* json_ast)
{
    printf("%d", json_ast->integer_value);
}

/**
 * Visit a float ast
 *
 * @param json_ast_T* json_ast
 *
 * @return json_ast_T*
 */
json_ast_T* json_visitor_visit_float(json_ast_T* json_ast)
{
    printf("%12.6f", json_ast->float_value);
}
