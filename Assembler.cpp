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
    // iterate over m_lineTokens and destroy all the token vectors
}


AssemblerResult Assembler::assemble(const char* infile, bool compileOnly, bool hexOutput, const char* outfile)
{

    // open infile and tokenize everything, creating a std::vector of lines
   
    std::ifstream asmfile(infile);

    if (asmfile.is_open())
    {
        int lineNo = 0;
        std::string line;
        while (std::getline(asmfile, line))
        {
       
            TokenVector*    tv = new TokenVector();

            AssemblerResult res = tv->tokenize(line);

            if (res != ASSEMBLER_OK)
            {
                switch(res)
                {
                    case ASSEMBLER_INVALID_TOKEN:
                        std::cout << "Invalid token";
                        break;                
                    case ASSEMBLER_BUFFER_OVERFLOW:
                        std::cout << "Buffer overflow";
                        break;
                    default:
                        std::cout << "Unknown exception " << res;
                }

                std::cout << " at line " << lineNo << std::endl;
                return ASSEMBLER_ERROR;
            }
    
            m_lineTokens.push_back(tv); 
            lineNo ++;
        }

    }      

    // print out token vector

    printTokens();
    
    // build symbol table
    
    

    // assemble

}

void Assembler::printTokens()
{

    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
        (*iter)->print();
    
}
