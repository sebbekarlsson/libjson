#include "include/json_parser.h"
#include "include/json_visitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *filename) {
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Could not open %s\n", filename);
    exit(1);
  }

  char *data = calloc(1, sizeof(char));
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1) {
    data = realloc(data, (strlen(data) + strlen(line) + 1) * sizeof(char));
    strcat(data, line);
  }

  fclose(fp);

  if (line)
    free(line);

  return data;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Must specify file to parse.\n");
    exit(1);
  }

  json_parser_T *parser = init_json_parser(init_json_lexer(read_file(argv[1])));

  json_ast_T *ast = json_parser_parse(parser);

  json_visitor_visit(ast);

  json_parser_free(parser);
  json_ast_free(ast);
}
