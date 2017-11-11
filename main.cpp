#include <cstdio>

#include "parser.hpp"
#include "generated/enum.hpp"

int main(int arg_count, char** args)
{
    // Reading file contents and setup proper storage
    char* str;
    FILE* f = fopen(args[1], "rb");

    fseek(f, 0L, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0L, SEEK_SET);

    str = (char*) malloc(len + 1);
    fread(str, len, 1, f);
    str[len + 1] = '\0';

    fclose(f);
    
    Tokenizer parser{str};
    for(;;)
    {
	auto token = parser.next();

	if(token.type == Token_Type::EOS)
	{
	    break;
	}

	printf("Token {type: %s, text: \"%.*s\", length: %zu}\n", get_enum_name(token.type), token.length, token.text, token.length);
    }
    
    free(str);

    return 0;
}
