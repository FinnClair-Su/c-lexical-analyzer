#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token_types.h"
#include "symbol_table.h"

namespace lexer {

class LexicalError {
public:
    LexicalError(const std::string& message, int line, int column);
    
    std::string getMessage() const;
    int getLine() const;
    int getColumn() const;
    std::string toString() const;
    
private:
    std::string message_;
    int line_;
    int column_;
};

class Lexer {
public:
    explicit Lexer(const std::string& sourceCode);
    
    std::vector<Token> tokenize();
    bool hasErrors() const;
    const std::vector<LexicalError>& getErrors() const;
    const SymbolTable& getSymbolTable() const;
    
    void writeTokens(const std::string& filepath) const;
    void writeSymbolTable(const std::string& filepath) const;
    void writeErrors(const std::string& filepath) const;
    
private:
    std::string source_;
    size_t pos_;
    int line_;
    int column_;
    char currentChar_;
    
    std::vector<Token> tokens_;
    std::vector<LexicalError> errors_;
    SymbolTable symbolTable_;
    std::unordered_map<std::string, TokenType> keywords_;
    
    void advance();
    char peek(int offset = 1) const;
    void error(const std::string& message);
    void skipWhitespace();
    void skipComment();
    Token readIdentifier();
    Token readNumber();
    Token readOperator();
    void initKeywords();
};

} // namespace lexer

#endif
