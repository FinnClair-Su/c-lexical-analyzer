#ifndef TOKEN_TYPES_H
#define TOKEN_TYPES_H

#include <string>

namespace lexer {

enum class TokenType {
    // 保留字
    VOID = 1,
    INT = 2,
    FLOAT = 3,
    DOUBLE = 4,
    IF = 5,
    ELSE = 6,
    FOR = 7,
    DO = 8,
    WHILE = 9,
    RETURN = 10,
    
    IDENTIFIER = 21,
    INTEGER = 22,
    
    // 运算符
    PLUS = 30,
    MINUS = 31,
    MULTIPLY = 32,
    DIVIDE = 33,
    ASSIGN = 34,
    LT = 35,
    GT = 36,
    NOT = 37,
    INCREMENT = 38,
    DECREMENT = 39,
    PLUS_ASSIGN = 40,
    MINUS_ASSIGN = 41,
    MULTIPLY_ASSIGN = 42,
    DIVIDE_ASSIGN = 43,
    EQUAL = 44,
    NOT_EQUAL = 45,
    LE = 46,
    GE = 47,
    LEFT_SHIFT = 48,
    RIGHT_SHIFT = 49,
    AND = 50,
    OR = 51,
    
    SEMICOLON = 60,
    COMMA = 61,
    LPAREN = 62,
    RPAREN = 63,
    LBRACE = 64,
    RBRACE = 65,
    
    EOF_TOKEN = 99,
    ERROR = -1
};

class Token {
public:
    Token(TokenType type, const std::string& value, int line, int column);
    
    TokenType getType() const;
    std::string getValue() const;
    int getLine() const;
    int getColumn() const;
    int getCategoryCode() const;
    std::string toString() const;
    
private:
    TokenType type_;
    std::string value_;
    int line_;
    int column_;
};

}

#endif
