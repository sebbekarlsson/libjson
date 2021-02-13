#ifndef JSON_LEXER_H
#define JSON_LEXER_H
#include "json_token.h"

typedef struct JSON_LEXER_STRUCT {
  char *contents;
  int i;
  char c;
} json_lexer_T;

json_lexer_T *init_json_lexer(char *contents);

void json_lexer_free(json_lexer_T *json_lexer);

json_token_T *json_lexer_get_next_token(json_lexer_T *json_lexer);

json_token_T *json_lexer_advance_with_token(json_lexer_T *json_lexer,
                                            json_token_T *json_token);

void json_lexer_advance(json_lexer_T *json_lexer);

void json_lexer_skip_whitespace(json_lexer_T *json_lexer);

json_token_T *json_lexer_collect_string(json_lexer_T *json_lexer);

json_token_T *json_lexer_collect_id(json_lexer_T *json_lexer);

json_token_T *json_lexer_collect_number(json_lexer_T *json_lexer);

char *json_lexer_current_charstr(json_lexer_T *json_lexer);

char json_lexer_peek(json_lexer_T *json_lexer);
#endif
