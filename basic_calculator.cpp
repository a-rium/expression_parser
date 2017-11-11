#include <stdio.h>

#include <vector>
#include <stack>
#include <unordered_set>

#include "parser.hpp"
#include "generated/enum.hpp"

std::unordered_set<Token_Type> priority{Token_Type::SLASH, Token_Type::ASTERISK};

std::vector<Token> priorityfy_expression(const std::vector<Token> expression)
{
    std::vector<Token> result;
    
    int encountered_operands = 0;
    std::stack<int> encountered_operands_history{};

    bool parenthesis_opened = false;
    bool adding_priority = false;
    int offset = 0; // Added parenthesis offset
    for(int i = 0; i < expression.size(); i++)
    {
	auto& token = expression[i];
	if(token.type == Token_Type::NUMERIC)
	{
	    encountered_operands++;
	    result.push_back(token);
	    if(adding_priority)
	    {
		result.push_back(Token{")", 1, Token_Type::PARENTHESIS_CLOSE});
		adding_priority = false;
		offset++;
	    }
	    if(encountered_operands == 2)
	    {
		encountered_operands = 1;
	    }
	}
	else if(token.type == Token_Type::PARENTHESIS_OPEN)
	{
	    encountered_operands_history.push(encountered_operands);
	    encountered_operands = 0;
	    parenthesis_opened = true;
	    result.push_back(token);
	}
	else if(token.type == Token_Type::PARENTHESIS_CLOSE)
	{
	    // @Todo: this should not happen
	    if(encountered_operands_history.empty())
	    {
		encountered_operands = 0;
	    }
	    else
	    {
		encountered_operands = encountered_operands_history.top();
		encountered_operands_history.pop();
	    }
	    parenthesis_opened = false;
	    result.push_back(token);
	    if(adding_priority)
	    {
		result.push_back(Token{")", 1, Token_Type::PARENTHESIS_CLOSE});
		adding_priority = false;
		offset++;
	    }
	}
	else if(priority.find(token.type) != priority.end())
	{
	    result.push_back(token);
	    // @Todo: if this happens when no encountered operands
	    if(offset + i - 1 >= 0)
	    {
		if(result[offset + i - 1].type == Token_Type::NUMERIC)
		{
		    result.insert(result.begin() + offset + i - 1, Token{"(", 1, Token_Type::PARENTHESIS_OPEN});
		    offset++;
		    adding_priority = true;
		}
		else if(result[offset + i - 1].type == Token_Type::PARENTHESIS_CLOSE)
		{
		    int insert_index = i - 2;
		    while(result[insert_index].type == Token_Type::PARENTHESIS_OPEN)
		    {
			insert_index--;
		    }
		    result.insert(result.begin() + insert_index, Token{"(", 1, Token_Type::PARENTHESIS_OPEN});
		    offset++;
		    adding_priority = true;
		}
	    }
	}
	else
	{
	    result.push_back(token);
	}
    }

    return result;
}

std::vector<Token> parenthesify_expression(const std::vector<Token> expression)
{
    std::vector<Token> result;
    
    int encountered_operands = 0;
    std::stack<int> encountered_operands_history{};
    int parenthesis_index = 0;
    std::stack<int> parenthesis_index_history{};

    int offset = 0;
    for(int i = 0; i < expression.size(); i++)
    {
	auto& token = expression[i];
	if(token.type == Token_Type::NUMERIC)
	{
	    encountered_operands++;
	    result.push_back(token);
	    if(encountered_operands == 2)
	    {
		if(i + 1 >= expression.size() || expression[i + 1].type != Token_Type::PARENTHESIS_CLOSE)
		{
		    result.insert(result.begin() + parenthesis_index, Token{"(", 1, Token_Type::PARENTHESIS_OPEN});
		    result.push_back(Token{")", 1, Token_Type::PARENTHESIS_CLOSE});
		    encountered_operands = 1;
		    offset += 2;
		}
	    }
	}
	else if(token.type == Token_Type::PARENTHESIS_OPEN)
	{
	    encountered_operands_history.push(encountered_operands);
	    encountered_operands = 0;
	    parenthesis_index_history.push(parenthesis_index);
	    parenthesis_index = i + offset;
	    result.push_back(token);
	}
	else if(token.type == Token_Type::PARENTHESIS_CLOSE)
	{
	    if(encountered_operands_history.empty())
	    {
		encountered_operands = 0;
	    }
	    else
	    {
		encountered_operands = encountered_operands_history.top() + 1; // the parenthesis section is a new operand, so plus 1 to the counter
		encountered_operands_history.pop();

		if(encountered_operands == 2)
		{
		    result.insert(result.begin() + parenthesis_index, Token{"(", 1, Token_Type::PARENTHESIS_OPEN});
		    result.push_back(Token{")", 1, Token_Type::PARENTHESIS_CLOSE});
		    encountered_operands = 1;
		    offset += 2;	    
		}
	    }
	    if(!parenthesis_index_history.empty())
	    {
		parenthesis_index = 0;
	    }
	    else
	    {
		parenthesis_index = parenthesis_index_history.top();
		parenthesis_index_history.pop();
	    }
	    result.push_back(token);
	}
	else
	{
	    result.push_back(token);
	}
    }

    return result;
}

std::unordered_set<Token_Type> allowed_operators{Token_Type::PLUS, Token_Type::MINUS, Token_Type::ASTERISK, Token_Type::SLASH};

std::vector<Token> to_polish_notation(const std::vector<Token> expression)
{
    std::vector<Token> result;

    std::stack<Token> encountered_operators;
    for(auto& token : expression)
    {
	if(token.type == Token_Type::NUMERIC)
	{
	    result.push_back(token);
	}
	else if(token.type == Token_Type::PARENTHESIS_CLOSE)
	{
	    if(!encountered_operators.empty())
	    {
		result.push_back(encountered_operators.top());
		encountered_operators.pop();
	    }
	    // @Todo Error handling
	}
	else if(allowed_operators.find(token.type) != allowed_operators.end())
	{
	    encountered_operators.push(token);
	}
	// @Todo What if the met symbol is useless/unknown
    }

    return result;
}

long my_atoi(const char* str, size_t length)
{
    long result = 0;
    for(int i = 0; i < length; i++)
    {
	int digit = str[i] - 0x30;
	result += digit * pow(10, length - i - 1);
    }

    return result;
}

int eval(const std::vector<Token> postfix_expression)
{
    std::vector<long> operands;
    for(auto& token : postfix_expression)
    {
	if(token.type == Token_Type::NUMERIC)
	{
	    auto operand = my_atoi(token.text, token.length);
	    operands.push_back(operand);
	}
    }

    std::stack<long> encountered_operands;
    
    int operands_number = 0;
    for(int i = 0; i < postfix_expression.size(); i++)
    {
	auto& token = postfix_expression[i];
	if(token.type == Token_Type::NUMERIC)
	{
	    encountered_operands.push(operands[operands_number++]);
	}
	else if(token.type == Token_Type::PLUS)
	{
	    auto b = encountered_operands.top(); encountered_operands.pop();
	    auto a = encountered_operands.top(); encountered_operands.pop();
	    auto result = a + b;
	    encountered_operands.push(result);
	}
	else if(token.type == Token_Type::MINUS)
	{
	    auto b = encountered_operands.top(); encountered_operands.pop();
	    auto a = encountered_operands.top(); encountered_operands.pop();
	    auto result = a - b;
	    encountered_operands.push(result);
	}
	else if(token.type == Token_Type::ASTERISK)
	{
	    auto b = encountered_operands.top(); encountered_operands.pop();
	    auto a = encountered_operands.top(); encountered_operands.pop();
	    auto result = a * b;
	    encountered_operands.push(result);
	}
	else if(token.type == Token_Type::SLASH)
	{
	    auto b = encountered_operands.top(); encountered_operands.pop();
	    auto a = encountered_operands.top(); encountered_operands.pop();
	    auto result = a / b;
	    encountered_operands.push(result);
	}
    }

    return encountered_operands.top();
}

int main(int arg_count, char** args)
{
    Tokenizer parser{args[1]};

    std::vector<Token> expression;
    
    for(;;)
    {
	Token token = parser.next();
	if(token.type == Token_Type::EOS)
	{
	    break;
	}

	expression.push_back(token);
    }

    for(auto& token : expression)
    {
	printf("Token {type: %s, text: \"%.*s\", length: %zu}\n", get_enum_name(token.type), token.length, token.text, token.length);
    }

    expression = priorityfy_expression(expression);

    for(auto& token : expression)
    {
	printf("%.*s", token.length, token.text);
    }

    printf("\n");
    
    expression = parenthesify_expression(expression);

    for(auto& token : expression)
    {
	printf("%.*s", token.length, token.text);
    }

    printf("\n");

    auto postfix_expression = to_polish_notation(expression);

    for(auto& token : postfix_expression)
    {
	printf("%.*s", token.length, token.text);
    }

    printf("\n");

    auto result = eval(postfix_expression);

    printf("Result: %ld\n", result);
    
    return 0;
}
