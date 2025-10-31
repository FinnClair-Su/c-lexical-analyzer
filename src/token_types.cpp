#include "token_types.h"
#include <sstream>

namespace lexer {

Token::Token(TokenType type, const std::string& value, int line, int column)
    : type_(type), value_(value), line_(line), column_(column) {
}

TokenType Token::getType() const {
    return type_;
}

std::string Token::getValue() const {
    return value_;
}

int Token::getLine() const {
    return line_;
}

int Token::getColumn() const {
    return column_;
}

int Token::getCategoryCode() const {
    return static_cast<int>(type_);
}

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token(" << getCategoryCode() << ", '" << value_ 
        << "', " << line_ << ":" << column_ << ")";
    return oss.str();
}

}
