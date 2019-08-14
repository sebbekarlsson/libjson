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
    json_parser_T* parser = init_json_parser(init_json_lexer(read_file("shards/person.json")));

    json_ast_T* ast = json_parser_parse(parser);

    for (int i = 0; i < ast->key_value_list_size; i++)
    {
        json_ast_T* key_value = ast->key_value_list_value[i];

        if (strcmp(key_value->key_value_key, "name") == 0)
        {
            printf("The name is: `%s`\n", key_value->key_value_value->string_value);
            break;
        }
    }

    json_parser_free(parser);
    json_ast_free(ast);
}
