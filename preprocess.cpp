#include <cstdio>

#include "parser.hpp"

bool string_equals(const char* a, const char* b, size_t length)
{
    for(int i = 0; i < length; i++)
    {
	if(a[i] != b[i])
	{
	    return false;
	}
    }

    return true;
}

bool token_literal_equals(Token token, const char* str)
{
    if(token.type == Token_Type::LITERAL)
    {
	if(token.length == strlen(str))
	{
	    return string_equals(token.text, str, token.length);
	}
    }

    return false;
}

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
    
    // printf("File content:\n%s\n", str);

    printf("#pragma once\n\n");
    printf("#include \"parser.hpp\"\n");
    
    Tokenizer parser{str};
    for(;;)
    {
	auto token = parser.next();

	if(token.type == Token_Type::EOS)
	{
	    break;
	}

	if(token.type == Token_Type::LITERAL)
	{
	    if(token_literal_equals(token, "printable"))
	    {
		if(token_literal_equals(parser.next(), "enum"))
		{
		    if(token_literal_equals(parser.next(), "class"))
		    {
			Token enum_name = parser.next();
			printf("const char* get_enum_name(%.*s e)\n", enum_name.length, enum_name.text);
			printf("{\n");
			printf("\tswitch(e)\n");
			printf("\t{\n");
			parser.next();
			bool found_comma = true;
			while(token.type != Token_Type::BRACES_CLOSE)
			{
			    Token enum_element = parser.next();
			    if(found_comma && enum_element.type == Token_Type::LITERAL)
			    {
				printf("\t\tcase %.*s::%.*s: return \"%.*s\";\n", enum_name.length, enum_name.text, enum_element.length, enum_element.text, enum_element.length, enum_element.text);

				found_comma = false;
			    }
			    else if(enum_element.type == Token_Type::COMMA)
			    {
				found_comma = true;
			    }
			    else
			    {
				break;
			    }
			}
			printf("\t\tdefault: return NULL;\n");
			printf("\t}\n");
			printf("}\n");
		    }
		}
	    }
	}
    }
    
    free(str);

    return 0;
}
