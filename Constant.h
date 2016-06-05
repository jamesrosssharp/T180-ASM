#pragma once

#include <string>

#include "Retcode.h"
#include "TokenVector.h"
#include "SymbolTable.h"

class ConstantTable;

class Constant
{

public:
    
        Constant(const std::string* label, int value);
        ~Constant();

    const std::string*   getLabel();
    const int            getValue();

    static AssemblerResult  ParseExpression(ConstantTable* const_table, SymbolTable* sym_table, const std::string* token, int& val);  

    static bool IsValidHexString(const char* str);
    static bool IsValidInteger(const char* str);

    static unsigned short ParseHexString(const char* tok);

    void print();

private:
    
    std::string m_label;

    int         m_value;

};

bool operator == (Constant& a, Constant& b);
