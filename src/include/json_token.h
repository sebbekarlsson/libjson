#ifndef JSON_TOKEN_H
#define JSON_TOKEN_H
typedef struct JSON_TOKEN_STRUCT
{
    enum
    {
        TOKEN_STRING,
        TOKEN_INTEGER,
        TOKEN_FLOAT,
        TOKEN_COMMA,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_COLON
    } type;

    char* value;
} json_token_T;

json_token_T* init_json_token(int type, char* value);

void json_token_free(json_token_T* json_token);
#endif
