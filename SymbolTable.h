#pragma once

#include <vector>
#include <string>
#include "Symbol.h"

class SymbolTable
{

public:

    SymbolTable();
    ~SymbolTable();

    void addSymbol(const std::string* name);

    void print();

private:
    std::vector<Symbol*> m_symbols;

};
