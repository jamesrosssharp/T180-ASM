#pragma once

#include <vector>

#include "Retcode.h"
#include "TokenVector.h"
#include "SymbolTable.h"
#include "ConstantTable.h"
#include "PseudoOpCode.h"

class Assembler 
{

public:
    Assembler();
    ~Assembler();

    AssemblerResult assemble(const char* infile, bool compileOnly = false, bool hexOutput = false, const char* outfile = NULL);  

    AssemblerResult tokenize(const char* infile);

    static void PrintError(const std::string* file, const std::string* line, const int lineNo, const Token* token, const char* errorMsg);  

    void printTokens();
    void printSymbols();
    void printConstants();

private:

    std::vector <TokenVector*> m_lineTokens;
   
    SymbolTable m_symbolTable; 
    ConstantTable m_constantTable;

};

