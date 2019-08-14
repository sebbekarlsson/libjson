# libjson
> Super fast JSON implementation in C, parse JSON in C with the speed of light.

## Usage
> To parse json in C, here is an example.  
> Let's say we have a json file called **person.json**:
```json
{
    "age": 32,
    "name": "John Doe",
    "email": "doe@doecompany.com"    
}
```
> ... and we want to parse it,
> and read the `name` field:
```C
#include <json/json_parser.h>


int main(int argc, char* argv[])
{
    // load the parser with the file
    json_parser_T* parser = init_json_parser(init_json_lexer(read_file("shards/person.json")));

    // parse the file
    json_ast_T* ast = json_parser_parse(parser);

    // look for the key we are interested in
    for (int i = 0; i < ast->key_value_list_size; i++)
    {
        json_ast_T* key_value = ast->key_value_list_value[i];

        if (strcmp(key_value->key_value_key, "name") == 0)
        {
            // print it
            printf("The name is: `%s`\n", key_value->key_value_value->string_value);

            // we were only interested in the name, so let's just break.
            break;
        }
    }

    // don't forget to free some memory.
    json_parser_free(parser);
    json_ast_free(ast);
}
```

## Speed
> The parser parses the [shards/boards.json](shards/boards.json) file in about
> `0.002` seconds:
```bash
time ./a.out

real	0m0.002s
user	0m0.003s
sys	0m0.000s
```