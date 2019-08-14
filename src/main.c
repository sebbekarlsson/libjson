#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/json_parser.h"


char* read_file(const char* filename)
{
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Could not open %s\n", filename);
        exit(1);
    }

    char* data = calloc(1, sizeof(char));
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        data = realloc(data, (strlen(data) + strlen(line) + 1) * sizeof(char));
        strcat(data, line);
    }

    fclose(fp);

    if (line)
        free(line);

    return data;
}

int main(int argc, char* argv[])
{
    char* contents = read_file("shards/boards.json");
    json_lexer_T* lexer = init_json_lexer(contents);
    json_parser_T* parser = init_json_parser(lexer);

    json_ast_T* ast = json_parser_parse(parser);

    json_parser_free(parser);
}
