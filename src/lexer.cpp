#include "lexer.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>

namespace lexer {

LexicalError::LexicalError(const std::string& message, int line, int column)
    : message_(message), line_(line), column_(column) {
}

std::string LexicalError::getMessage() const {
    return message_;
}

int LexicalError::getLine() const {
    return line_;
}

int LexicalError::getColumn() const {
    return column_;
}

std::string LexicalError::toString() const {
    std::ostringstream oss;
    oss << "错误: [" << line_ << ":" << column_ << "] " << message_;
    return oss.str();
}

Lexer::Lexer(const std::string& sourceCode)
    : source_(sourceCode), pos_(0), line_(1), column_(1) {
    currentChar_ = pos_ < source_.length() ? source_[pos_] : '\0';
    initKeywords();
}

void Lexer::initKeywords() {
    keywords_["void"] = TokenType::VOID;
    keywords_["int"] = TokenType::INT;
    keywords_["float"] = TokenType::FLOAT;
    keywords_["double"] = TokenType::DOUBLE;
    keywords_["if"] = TokenType::IF;
    keywords_["else"] = TokenType::ELSE;
    keywords_["for"] = TokenType::FOR;
    keywords_["do"] = TokenType::DO;
    keywords_["while"] = TokenType::WHILE;
    keywords_["return"] = TokenType::RETURN;
}

void Lexer::advance() {
    if(pos_ < source_.length()) {
        if(currentChar_ == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        pos_++;
        currentChar_ = pos_ < source_.length() ? source_[pos_] : '\0';
    }
}

char Lexer::peek(int offset) const {
    size_t peekPos = pos_ + offset;
    if (peekPos < source_.length()) {
        return source_[peekPos];
    }
    return '\0';
}

void Lexer::error(const std::string& message) {
    errors_.emplace_back(message, line_, column_);
}

void Lexer::skipWhitespace() {
    while(currentChar_ == ' ' || currentChar_ == '\t' || 
          currentChar_ == '\n' || currentChar_ == '\r') {
        advance();
    }
}

void Lexer::skipComment() {
    if(currentChar_ == '/' && peek() == '/') {
        advance();
        advance();
        while(currentChar_ != '\0' && currentChar_ != '\n') {
            advance();
        }
    } else if(currentChar_ == '/' && peek() == '*') {
        int startLine = line_;
        int startColumn = column_;
        advance();
        advance();
        
        while(currentChar_ != '\0') {
            if(currentChar_ == '*' && peek() == '/') {
                advance();
                advance();
                return;
            }
            advance();
        }
        
        std::ostringstream oss;
        oss << "多行注释未闭合（从 " << startLine << ":" << startColumn << " 开始）";
        errors_.emplace_back(oss.str(), startLine, startColumn);
    }
}

Token Lexer::readIdentifier() {
    int startLine = line_;
    int startColumn = column_;
    std::string identifier;
    
    while(std::isalnum(currentChar_) || currentChar_ == '_') {
        identifier += currentChar_;
        advance();
    }
    
    if(identifier.length() > 32) {
        std::ostringstream oss;
        oss << "标识符 '" << identifier << "' 长度超过32个字符";
        error(oss.str());
        identifier = identifier.substr(0, 32);
    }
    
    auto it = keywords_.find(identifier);
    if(it != keywords_.end()) {
        return Token(it->second, identifier, startLine, startColumn);
    } else {
        symbolTable_.insert(identifier);
        return Token(TokenType::IDENTIFIER, identifier, startLine, startColumn);
    }
}

Token Lexer::readNumber() {
    int startLine = line_;
    int startColumn = column_;
    std::string number;
    
    while(std::isdigit(currentChar_)) {
        number += currentChar_;
        advance();
    }
    
    return Token(TokenType::INTEGER, number, startLine, startColumn);
}

Token Lexer::readOperator() {
    int startLine = line_;
    int startColumn = column_;
    char current = currentChar_;
    
    if(current == '+') {
        advance();
        if(currentChar_ == '+') {
            advance();
            return Token(TokenType::INCREMENT, "++", startLine, startColumn);
        } else if(currentChar_ == '=') {
            advance();
            return Token(TokenType::PLUS_ASSIGN, "+=", startLine, startColumn);
        }
        return Token(TokenType::PLUS, "+", startLine, startColumn);
    }
    
    if(current == '-') {
        advance();
        if(currentChar_ == '-') {
            advance();
            return Token(TokenType::DECREMENT, "--", startLine, startColumn);
        } else if(currentChar_ == '=') {
            advance();
            return Token(TokenType::MINUS_ASSIGN, "-=", startLine, startColumn);
        }
        return Token(TokenType::MINUS, "-", startLine, startColumn);
    }
    
    if(current == '*') {
        advance();
        if(currentChar_ == '=') {
            advance();
            return Token(TokenType::MULTIPLY_ASSIGN, "*=", startLine, startColumn);
        }
        return Token(TokenType::MULTIPLY, "*", startLine, startColumn);
    }
    
    if(current == '/') {
        advance();
        if(currentChar_ == '=') {
            advance();
            return Token(TokenType::DIVIDE_ASSIGN, "/=", startLine, startColumn);
        }
        return Token(TokenType::DIVIDE, "/", startLine, startColumn);
    }
    
    if(current == '=') {
        advance();
        if(currentChar_ == '=') {
            advance();
            return Token(TokenType::EQUAL, "==", startLine, startColumn);
        }
        return Token(TokenType::ASSIGN, "=", startLine, startColumn);
    }
    
    if(current == '<') {
        advance();
        if(currentChar_ == '=') {
            advance();
            return Token(TokenType::LE, "<=", startLine, startColumn);
        } else if(currentChar_ == '<') {
            advance();
            return Token(TokenType::LEFT_SHIFT, "<<", startLine, startColumn);
        }
        return Token(TokenType::LT, "<", startLine, startColumn);
    }
    
    if(current == '>') {
        advance();
        if(currentChar_ == '=') {
            advance();
            return Token(TokenType::GE, ">=", startLine, startColumn);
        } else if(currentChar_ == '>') {
            advance();
            return Token(TokenType::RIGHT_SHIFT, ">>", startLine, startColumn);
        }
        return Token(TokenType::GT, ">", startLine, startColumn);
    }
    
    if(current == '!') {
        advance();
        if(currentChar_ == '=') {
            advance();
            return Token(TokenType::NOT_EQUAL, "!=", startLine, startColumn);
        }
        return Token(TokenType::NOT, "!", startLine, startColumn);
    }
    
    if(current == '&') {
        advance();
        if(currentChar_ == '&') {
            advance();
            return Token(TokenType::AND, "&&", startLine, startColumn);
        }
        std::ostringstream oss;
        oss << "非法字符 '&'";
        error(oss.str());
        return Token(TokenType::ERROR, "&", startLine, startColumn);
    }
    
    if(current == '|') {
        advance();
        if(currentChar_ == '|') {
            advance();
            return Token(TokenType::OR, "||", startLine, startColumn);
        }
        std::ostringstream oss;
        oss << "非法字符 '|'";
        error(oss.str());
        return Token(TokenType::ERROR, "|", startLine, startColumn);
    }
    
    if(current == ';') {
        advance();
        return Token(TokenType::SEMICOLON, ";", startLine, startColumn);
    }
    if(current == ',') {
        advance();
        return Token(TokenType::COMMA, ",", startLine, startColumn);
    }
    if(current == '(') {
        advance();
        return Token(TokenType::LPAREN, "(", startLine, startColumn);
    }
    if(current == ')') {
        advance();
        return Token(TokenType::RPAREN, ")", startLine, startColumn);
    }
    if(current == '{') {
        advance();
        return Token(TokenType::LBRACE, "{", startLine, startColumn);
    }
    if(current == '}') {
        advance();
        return Token(TokenType::RBRACE, "}", startLine, startColumn);
    }
    
    std::ostringstream oss;
    oss << "非法字符 '" << current << "'";
    error(oss.str());
    advance();
    return Token(TokenType::ERROR, std::string(1, current), startLine, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    tokens_.clear();
    
    while(currentChar_ != '\0') {
        if(currentChar_ == ' ' || currentChar_ == '\t' || 
           currentChar_ == '\n' || currentChar_ == '\r') {
            skipWhitespace();
            continue;
        }
        
        if(currentChar_ == '/' && (peek() == '/' || peek() == '*')) {
            skipComment();
            continue;
        }
        
        if(std::isalpha(currentChar_) || currentChar_ == '_') {
            tokens_.push_back(readIdentifier());
            continue;
        }
        
        if(std::isdigit(currentChar_)) {
            tokens_.push_back(readNumber());
            continue;
        }
        
        if(currentChar_ == '+' || currentChar_ == '-' || currentChar_ == '*' || 
           currentChar_ == '/' || currentChar_ == '=' || currentChar_ == '<' || 
           currentChar_ == '>' || currentChar_ == '!' || currentChar_ == '&' || 
           currentChar_ == '|' || currentChar_ == ';' || currentChar_ == ',' || 
           currentChar_ == '(' || currentChar_ == ')' || currentChar_ == '{' || 
           currentChar_ == '}') {
            tokens_.push_back(readOperator());
            continue;
        }
        
        std::ostringstream oss;
        oss << "非法字符 '" << currentChar_ << "'";
        error(oss.str());
        advance();
    }
    
    tokens_.push_back(Token(TokenType::EOF_TOKEN, "", line_, column_));
    return tokens_;
}

bool Lexer::hasErrors() const {
    return !errors_.empty();
}

const std::vector<LexicalError>& Lexer::getErrors() const {
    return errors_;
}

const SymbolTable& Lexer::getSymbolTable() const {
    return symbolTable_;
}

void Lexer::writeTokens(const std::string& filepath) const {
    std::ofstream outFile(filepath);
    if(!outFile) {
        throw std::runtime_error("无法创建文件: " + filepath);
    }
    for(const auto& token : tokens_) {
        outFile << "(" << token.getCategoryCode() << ", " << token.getValue() << ")\n";
    }
}

void Lexer::writeSymbolTable(const std::string& filepath) const {
    std::ofstream outFile(filepath);
    if(!outFile) {
        throw std::runtime_error("无法创建文件: " + filepath);
    }
    auto symbols = symbolTable_.getAllSymbols();
    if(symbols.empty()) {
        outFile << "符号表为空\n";
        return;
    }
    outFile << "ID  | 标识符名\n";
    outFile << "----|----------\n";
    for(const auto& symbol : symbols) {
        outFile << std::left << std::setw(4) << symbol.id << "| " << symbol.name << "\n";
    }
}

void Lexer::writeErrors(const std::string& filepath) const {
    std::ofstream outFile(filepath);
    if(!outFile) {
        throw std::runtime_error("无法创建文件: " + filepath);
    }
    
    if(errors_.empty()) {
        outFile << "无错误\n";
    } else {
        for(const auto& error : errors_) {
            outFile << error.toString() << "\n";
        }
    }
}

} // namespace lexer
