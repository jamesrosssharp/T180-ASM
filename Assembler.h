#pragma once

#include <vector>
#include <string>

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
    AssemblerResult assembleLine(TokenVector* tv);

    void setAssemblerAddress(unsigned short address);
    unsigned short getAssemblerAddress();
   
    void advanceAssemblerAddress(unsigned short num_bytes);
 
    static void PrintError(const std::string* file, const std::string* line, const int lineNo, const Token* token, const char* errorMsg);  

    static std::string ToUpper(const std::string* s);

    void printTokens();
    void printSymbols();
    void printConstants();

private:

    std::vector <TokenVector*> m_lineTokens;
  
    unsigned short m_address;
 
    SymbolTable m_symbolTable; 
    ConstantTable m_constantTable;

};

