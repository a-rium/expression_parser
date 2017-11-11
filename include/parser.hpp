#pragma once

#include <string>

#define printable

struct IDONTCARE {};

printable enum class Token_Type
{
    LITERAL,
    NUMERIC,

    COMMENT,

    EQUALS,

    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    BACKSLASH,
    PERCENTAGE,
    DOLLAR,

    TILDE,
    EXCLAMATION,
    QUESTION_MARK,
    HASH,
    POWER,

    AND,
    OR,

    DOT,
    COMMA,
    SEMICOLON,
    COLON,

    UNDERSCORE,
    AT,

    QUOTE,
    DOUBLE_QUOTES,

    ANGULAR_OPEN,
    ANGULAR_CLOSE,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    BRACES_OPEN,
    BRACES_CLOSE,

    ESCAPED,

    EOS,
    UNKNOWN
};

struct Token
{
    const char* text;
    size_t length;
    Token_Type type;
};

struct Tokenizer
{
    const char* str;
    size_t at{0};

    Tokenizer(const char* str);
    Tokenizer(std::string std_str);

    Token next();

private:
    void eat_whitespace();

};

inline bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

inline bool is_literal(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

inline bool is_numeric(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}
