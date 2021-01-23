#ifndef JSON_VISITOR_H
#define JSON_VISITOR_H
#include "json_ast.h"

json_ast_T* json_visitor_visit(json_ast_T* json_ast);

json_ast_T* json_visitor_visit_key_value(json_ast_T* json_ast);

json_ast_T* json_visitor_visit_key_value_list(json_ast_T* json_ast);

json_ast_T* json_visitor_visit_list(json_ast_T* json_ast);

json_ast_T* json_visitor_visit_string(json_ast_T* json_ast);

json_ast_T* json_visitor_visit_integer(json_ast_T* json_ast);

json_ast_T* json_visitor_visit_float(json_ast_T* json_ast);
#endif
