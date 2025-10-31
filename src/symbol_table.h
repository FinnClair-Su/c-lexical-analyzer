#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <vector>

namespace lexer {

struct SymbolInfo {
    int id;
    std::string name;
};

class SymbolTable {
public:
    SymbolTable();
    int insert(const std::string& name);
    const SymbolInfo* lookup(const std::string& name) const;
    std::vector<SymbolInfo> getAllSymbols() const;
    size_t size() const;
    bool contains(const std::string& name) const;
    
private:
    std::unordered_map<std::string, SymbolInfo> symbols_;
    int nextId_;
};

}

#endif
