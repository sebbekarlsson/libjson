#include <json/json_lexer.h>
#include <json/json_parser.h>
#include <json/json_visitor.h>


typedef json_ast_T json;

typedef struct JSON_ITERATOR_STRUCT {
  json** values;
  json* current;
  uint32_t index;
  uint32_t length;
} JSONIterator;


json* json_get_value(json* data, const char* key);
char* json_get_string(json* data, const char* key);
int json_get_int(json* data, const char* key);
float json_get_float(json* data, const char* key);
float json_get_number(json* data, const char* key);

JSONIterator json_iterate(json* data);

json* json_iterator_next(JSONIterator* iterator);

unsigned int json_is_list(json* data);

char* json_string(json* data);
int json_int(json* data);
float json_float(json* data);
float json_number(json* data);
