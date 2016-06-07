#pragma once

#include <vector>
#include <string>
#include "Symbol.h"
#include "Retcode.h"

class SymbolTable
{

public:

    SymbolTable();
    ~SymbolTable();

    AssemblerResult addSymbol(const std::string* name);
    AssemblerResult addSymbol(const char* name);

    Symbol* getSymbol(const char* name);

    bool hasSymbol(const char* name);

    AssemblerResult setAddressForSymbol(const char* name, unsigned short address);

    void print();

private:
    std::vector<Symbol*> m_symbols;

};
