#pragma once

#include <vector>

#include "Constant.h"
#include "TokenVector.h"

class ConstantTable
{

public:

    ConstantTable();
    ~ConstantTable();

    AssemblerResult addConstant(TokenVector* tv);
    AssemblerResult addConstant(const char* token, int value);

    bool    hasConstant(const char* token);
    Constant*  getConstant(const char* token);

    void print();

private:

    std::vector<Constant*> m_constants;

};
