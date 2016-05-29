
#pragma once
#include <vector>

#include "Retcode.h"
#include "TokenVector.h"

class Assembler 
{

public:
    Assembler();
    ~Assembler();

    AssemblerResult assemble(const char* infile, bool compileOnly = false, bool hexOutput = false, const char* outfile = NULL);  

    void printTokens();

private:

    std::vector <TokenVector*> m_lineTokens;

};

