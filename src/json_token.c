#include "include/json_token.h"
#include <stdlib.h>


json_token_T* init_json_token(int type, char* value)
{
    json_token_T* json_token = calloc(1, sizeof(struct JSON_TOKEN_STRUCT));
    json_token->type = type;
    json_token->value = value;

    return json_token;
}

void json_token_free(json_token_T* json_token)
{
    free(json_token);
}
