#include "symbol_table.h"
#include <algorithm>

namespace lexer {

SymbolTable::SymbolTable() : nextId_(0) {}

int SymbolTable::insert(const std::string& name) {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return it->second.id;
    }
    SymbolInfo info{nextId_, name};
    symbols_[name] = info;
    return nextId_++;
}

const SymbolInfo* SymbolTable::lookup(const std::string& name) const {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<SymbolInfo> SymbolTable::getAllSymbols() const {
    std::vector<SymbolInfo> result;
    result.reserve(symbols_.size());
    for (const auto& pair : symbols_) {
        result.push_back(pair.second);
    }
    std::sort(result.begin(), result.end(), 
              [](const SymbolInfo& a, const SymbolInfo& b) {
                  return a.id < b.id;
              });
    return result;
}

size_t SymbolTable::size() const {
    return symbols_.size();
}

bool SymbolTable::contains(const std::string& name) const {
    return symbols_.find(name) != symbols_.end();
}

}
