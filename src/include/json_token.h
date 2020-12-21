#ifndef JSON_JSON_TOKEN_H
#define JSON_JSON_TOKEN_H
typedef struct JSON_JSON_TOKEN_STRUCT
{
    enum
    {
        JSON_TOKEN_STRING,
        JSON_TOKEN_INTEGER,
        JSON_TOKEN_FLOAT,
        JSON_TOKEN_COMMA,
        JSON_TOKEN_LBRACE,
        JSON_TOKEN_RBRACE,
        JSON_TOKEN_LBRACKET,
        JSON_TOKEN_RBRACKET,
        JSON_TOKEN_ID,
        JSON_TOKEN_COLON
    } type;

    char* value;
} json_token_T;

json_token_T* init_json_token(int type, char* value);

void json_token_free(json_token_T* json_token);
#endif
