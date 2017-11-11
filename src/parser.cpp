#include "parser.hpp"

#include <string>

Tokenizer::Tokenizer(const char* str) : str{str} {}
Tokenizer::Tokenizer(std::string std_str) : str{std_str.c_str()} {}

Token Tokenizer::next()
{
    eat_whitespace();

    Token token{};
    token.text = str + at;
    switch(str[at])
    {
    case '\0':
    {
	token.type = Token_Type::EOS;
	token.length = 1;

	at--; // Decrementing at makes so the parser won't read past the end of line
    } break;
    case '=':
    {
	token.type = Token_Type::EQUALS;
	token.length = 1;
	
    } break;
    case '+':
    {
	token.type = Token_Type::PLUS;
	token.length = 1;
	
    } break;
    case '-':
    {
	token.type = Token_Type::MINUS;
	token.length = 1;
	
    } break;
    case '*':
    {
	token.type = Token_Type::ASTERISK;
	token.length = 1;
	
    } break;
    case '/':
    {
	if(str[at + 1] == '/')
	{
	    at += 2;

	    token.type = Token_Type::COMMENT;
	    token.text += 2; // Skipping the double slash
	    while(str[at] != '\n' && str[at] != '\0')
	    {
		token.length++;
		at++;
	    }
	    
	    if(str[at - 2] == '\r') // Removing windows end-line special char
	    {
		token.length -= 2;
	    }
	    else                    // Removing '\n' or '\0'
	    {
		token.length--;
	    }

	    at--;
	}
	else
	{
	    token.type = Token_Type::SLASH;
	    token.length = 1;
	}
	
    } break;
    case '\\':
    {
	if(!is_whitespace(str[at + 1]))
	{
	    token.type = Token_Type::ESCAPED;
	    token.length = 2;
	}
	else
	{
	    token.type = Token_Type::BACKSLASH;
	    token.length = 1;
	}

    } break;
    case '%':
    {
	token.type = Token_Type::PERCENTAGE;
	token.length = 1;
    } break;
    case '$':
    {
	token.type = Token_Type::DOLLAR;
	token.length = 1;
    } break;
    case '~':
    {
	token.type = Token_Type::TILDE;
	token.length = 1;
    } break;
    case '!':
    {
	token.type = Token_Type::EXCLAMATION;
	token.length = 1;
    } break;
    case '?':
    {
	token.type = Token_Type::QUESTION_MARK;
	token.length = 1;
    } break;
    case '#':
    {
	token.type = Token_Type::HASH;
	token.length = 1;
    } break;
    case '^':
    {
	token.type = Token_Type::POWER;
	token.length = 1;
    } break;
    case '.':
    {
	token.type = Token_Type::DOT;
	token.length = 1;
    } break;
    case ',':
    {
	token.type = Token_Type::COMMA;
	token.length = 1;
    } break;
    case ';':
    {
	token.type = Token_Type::SEMICOLON;
	token.length = 1;
    } break;
    case ':':
    {
	token.type = Token_Type::COLON;
	token.length = 1;
    } break;
    case '_': // @FIXME Currently parsing the underscore character without any checks for literal
    {
	token.type = Token_Type::UNDERSCORE;
	token.length = 1;
    } break;
    case '@':
    {
	token.type = Token_Type::AT;
	token.length = 1;
    } break;
    case '\'':
    {
	token.type = Token_Type::QUOTE;
	token.length = 1;
    } break;
    case '"':
    {
	token.type = Token_Type::DOUBLE_QUOTES;
	token.length = 1;
    } break;
    case '<':
    {
	token.type = Token_Type::ANGULAR_OPEN;
	token.length = 1;
    } break;
    case '>':
    {
	token.type = Token_Type::ANGULAR_CLOSE;
	token.length = 1;
    } break;
    case '(':
    {
	token.type = Token_Type::PARENTHESIS_OPEN;
	token.length = 1;
    } break;
    case ')':
    {
	token.type = Token_Type::PARENTHESIS_CLOSE;
	token.length = 1;
    } break;
    case '[':
    {
	token.type = Token_Type::BRACKET_OPEN;
	token.length = 1;
    } break;
    case ']':
    {
	token.type = Token_Type::BRACKET_CLOSE;
	token.length = 1;
    } break;
    case '{':
    {
	token.type = Token_Type::BRACES_OPEN;
	token.length = 1;
    } break;
    case '}':
    {
	token.type = Token_Type::BRACES_CLOSE;
	token.length = 1;
    } break;
    default:
    {
	if(is_literal(str[at]))
	{
	    token.type = Token_Type::LITERAL;
	    
	    while(is_literal(str[at]))
	    {
		token.length++;
		at++;
	    }
	    
	    at--;
	}
	else if(is_numeric(str[at]))
	{
	    token.type = Token_Type::NUMERIC;
	    
	    while(is_numeric(str[at]))
	    {
		token.length++;
		at++;
	    }

	    at--;
	}
	else
	{
	    token.type = Token_Type::UNKNOWN;
	    token.length = 1;
	}
    }
    }

    at++;

    return token;
}

void Tokenizer::eat_whitespace()
{
    while(is_whitespace(str[at]))
    {
	at++;
    }
}
