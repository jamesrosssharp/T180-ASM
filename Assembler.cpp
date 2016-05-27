//
//  T180 ASM
//
//  Assembler.cpp   : assembler class
//
//

#include <iostream>
#include <fstream>
#include <string>

#include "Assembler.h"

Assembler::Assembler()
{
}

Assembler::~Assembler()
{
}


AssemblerResult Assembler::assemble(const char* infile, bool compileOnly, bool hexOutput, const char* outfile)
{

    // open infile and tokenize everything, creating a std::vector of lines
   
    std::ifstream asmfile(infile);

    if (asmfile.is_open())
    {
        
        std::string line;
        while (std::getline(asmfile, line))
        {
       
            TokenVector*    tv = new TokenVector();

            AssemblerResult res = tv->tokenize(line);

            m_lineTokens.push_back(tv); 

        }

    }      

}
