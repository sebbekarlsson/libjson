#include "include/json_token.h"
#include <stdlib.h>


/**
 * Create new json token
 *
 * @param int type
 * @param char* value
 */
json_token_T* init_json_token(int type, char* value)
{
    json_token_T* json_token = calloc(1, sizeof(struct JSON_JSON_TOKEN_STRUCT));
    json_token->type = type;
    json_token->value = value;

    return json_token;
}

/**
 * Deallocation method for json_token
 *
 * @param json_token_T* json_token
 */
void json_token_free(json_token_T* json_token)
{
    if (json_token->value)
        free(json_token->value);

    free(json_token);
}
