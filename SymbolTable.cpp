
#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
}

AssemblerResult SymbolTable::addSymbol(const std::string* name)
{

    // TODO: check for duplicate symbols

    m_symbols.push_back(new Symbol(name));

    return ASSEMBLER_OK;
}

AssemblerResult SymbolTable::addSymbol(const char* name)
{

    std::string s(name);

    return addSymbol(&s);
}

void SymbolTable::print()
{
    for (std::vector<Symbol*>::iterator iter = m_symbols.begin(); iter != m_symbols.end(); iter++)
        (*iter)->print(); 
}


Symbol* SymbolTable::getSymbol(const char* name)
{

    for (std::vector<Symbol*>::iterator iter = m_symbols.begin(); iter != m_symbols.end(); iter++)
        if (*(*iter)->getLabel() == name)
            return *iter;

    return NULL;
}

bool SymbolTable::hasSymbol(const char* name)
{

    for (std::vector<Symbol*>::iterator iter = m_symbols.begin(); iter != m_symbols.end(); iter++)
        if (*(*iter)->getLabel() == name)
            return true;

    return false;
}

AssemblerResult SymbolTable::setAddressForSymbol(const char* name, unsigned short address)
{

    for (std::vector<Symbol*>::iterator iter = m_symbols.begin(); iter != m_symbols.end(); iter++)
        if (*(*iter)->getLabel() == name)
        {
            (*iter)->setAddress(address);
            return ASSEMBLER_OK;
        }

    return ASSEMBLER_SYMBOL_NOT_FOUND;
}

