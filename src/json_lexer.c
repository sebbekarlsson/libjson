#include "include/json_lexer.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


/**
 * Create a new lexer
 *
 * @param char* contents
 *
 * @return json_lexer_T*
 */
json_lexer_T* init_json_lexer(char* contents)
{
    json_lexer_T* json_lexer = calloc(1, sizeof(struct JSON_LEXER_STRUCT));
    json_lexer->contents = contents;
    json_lexer->i = 0;
    json_lexer->c = json_lexer->contents[json_lexer->i];

    return json_lexer;
}

/**
 * Deallocate a json_lexer
 *
 * @param json_lexer_T* json_lexer
 */
void json_lexer_free(json_lexer_T* json_lexer)
{
    if (json_lexer->contents)
        free(json_lexer->contents);

    free(json_lexer);
}

/**
 * Get the next json token
 *
 * @param json_lexer_T* json_lexer
 *
 * @return json_token_T*
 */
json_token_T* json_lexer_get_next_token(json_lexer_T* json_lexer)
{
    while (json_lexer->c != '\0' && json_lexer->i < strlen(json_lexer->contents))
    { 
        if (json_lexer->c == '\n' || json_lexer->c == ' ' || json_lexer->c == 13)
            json_lexer_skip_whitespace(json_lexer); 

        if (isdigit(json_lexer->c))
            return json_lexer_collect_number(json_lexer);

        switch (json_lexer->c)
        {
            case '{': return json_lexer_advance_with_token(json_lexer, init_json_token(TOKEN_LBRACE, json_lexer_current_charstr(json_lexer))); break;
            case '}': return json_lexer_advance_with_token(json_lexer, init_json_token(TOKEN_RBRACE, json_lexer_current_charstr(json_lexer))); break;
            case '[': return json_lexer_advance_with_token(json_lexer, init_json_token(TOKEN_LBRACKET, json_lexer_current_charstr(json_lexer))); break;
            case ']': return json_lexer_advance_with_token(json_lexer, init_json_token(TOKEN_RBRACKET, json_lexer_current_charstr(json_lexer))); break;
            case ',': return json_lexer_advance_with_token(json_lexer, init_json_token(TOKEN_COMMA, json_lexer_current_charstr(json_lexer))); break;
            case ':': return json_lexer_advance_with_token(json_lexer, init_json_token(TOKEN_COLON, json_lexer_current_charstr(json_lexer))); break;
            case '"': return json_lexer_advance_with_token(json_lexer, json_lexer_collect_string(json_lexer)); break;
        }

        if (json_lexer->c == 0)
            break;

        printf("[json_lexer] Unexpected `%c`.\n", json_lexer->c);
        exit(1);
    }

    return (void*)0;
}

/**
 * Advance and return a json token
 *
 * @param json_lexer_T* json_lexer
 * @param json_token_T* json_token
 *
 * @return json_token_T*
 */
json_token_T* json_lexer_advance_with_token(json_lexer_T* json_lexer, json_token_T* json_token)
{
    json_lexer_advance(json_lexer);

    return json_token;
}

/**
 * Advance, increment the current index.
 *
 * @param json_lexer_T* json_lexer
 */
void json_lexer_advance(json_lexer_T* json_lexer)
{
    if (json_lexer->i < strlen(json_lexer->contents))
    {
        json_lexer->i += 1;
        json_lexer->c = json_lexer->contents[json_lexer->i];
    }
}

/**
 * Advance until the current character is not a whitespace.
 *
 * @param json_lexer_T* json_lexer
 */
void json_lexer_skip_whitespace(json_lexer_T* json_lexer)
{
    while (json_lexer->c == '\n' || json_lexer->c == ' ' || json_lexer->c == 13)
        json_lexer_advance(json_lexer);
}

/**
 * Collect a string token
 *
 * @param json_lexer_T* json_lexer
 *
 * @return json_token_T*
 */
json_token_T* json_lexer_collect_string(json_lexer_T* json_lexer)
{
    json_lexer_advance(json_lexer); // skip first "
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (json_lexer->c != '"')
    {
        char* str = json_lexer_current_charstr(json_lexer);
        value = realloc(value, (strlen(value) + strlen(str) + 1) * sizeof(char));
        strcat(value, str);
        free(str);
        json_lexer_advance(json_lexer);
    }

    return init_json_token(TOKEN_STRING, value);
}

/**
 * Collect a numeric token, can be either integers or floats
 *
 * @param json_lexer_T* json_lexer
 *
 * @return json_token_T*
 */
json_token_T* json_lexer_collect_number(json_lexer_T* json_lexer)
{
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';
    int type = TOKEN_INTEGER;

    while (isdigit(json_lexer->c))
    {
        char* str = json_lexer_current_charstr(json_lexer);
        value = realloc(value, (strlen(value) + strlen(str) + 1) * sizeof(char));
        strcat(value, str);
        free(str);
        json_lexer_advance(json_lexer);
    }

    if (json_lexer->c == '.')
    {
        type = TOKEN_FLOAT;

        json_lexer_advance(json_lexer);

        while (isdigit(json_lexer->c))
        {
            char* str = json_lexer_current_charstr(json_lexer);
            strcat(value, str);
            free(str);
            json_lexer_advance(json_lexer);
        }
    }

    return init_json_token(type, value);
}

/**
 * Get the current character in the lexer as a string
 *
 * @param json_lexer_T* json_lexer
 *
 * @return char*
 */
char* json_lexer_current_charstr(json_lexer_T* json_lexer)
{
    char* str = calloc(2, sizeof(char));
    str[0] = json_lexer->c;
    str[1] = '\0';

    return str;
}
