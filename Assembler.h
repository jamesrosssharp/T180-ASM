
#pragma once
#include <vector>

#include "Retcode.h"
#include "TokenVector.h"
#include "SymbolTable.h"

class Assembler 
{

public:
    Assembler();
    ~Assembler();

    AssemblerResult assemble(const char* infile, bool compileOnly = false, bool hexOutput = false, const char* outfile = NULL);  

    AssemblerResult tokenize(const char* infile);

    void printTokens();
    void printSymbols();

private:

    std::vector <TokenVector*> m_lineTokens;
   
    SymbolTable m_symbolTable; 

};

