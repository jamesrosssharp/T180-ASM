
#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::addSymbol(const std::string* name)
{
    m_symbols.push_back(new Symbol(name));
}

void SymbolTable::print()
{
    for (std::vector<Symbol*>::iterator iter = m_symbols.begin(); iter != m_symbols.end(); iter++)
        (*iter)->print(); 
}
