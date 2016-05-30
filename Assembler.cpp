//
//  T180 ASM
//
//  Assembler.cpp   : assembler class
//
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <string.h>

#include "Assembler.h"

Assembler::Assembler()
{
}

Assembler::~Assembler()
{
    // iterate over m_lineTokens and destroy all the token vectors
}

AssemblerResult Assembler::tokenize(const char* infile)
{
    std::ifstream asmfile(infile);

    if (asmfile.is_open())
    {
        int lineNo = 1;
        std::string line;
        while (std::getline(asmfile, line))
        {
      
            // check if the line contains an include statement
            
            char* writeable = strdup(line.c_str()); 

            char* token = strtok(writeable, " \t");

            std::string tok;

            if (token != NULL)
            {
        
                for (int i = 0; i < strlen(token) ; i ++)
                    tok.push_back(std::toupper(token[i]));

                if (tok == std::string("#INCLUDE"))
                {  

                    char *fname = strtok(NULL, "\"\'");

                    //std::cout << fname << std::endl;                 

                    // Recurse
                    AssemblerResult res = tokenize(fname);

                    free(writeable);

                    if (res != ASSEMBLER_OK)
                        return res;

                    continue;

                }

            }

            free(writeable);

            // Else, tokenise the line
 
            TokenVector*    tv = new TokenVector();

            AssemblerResult res = tv->tokenize(std::string(infile), lineNo, line);

            if (res != ASSEMBLER_OK)
            {
                return ASSEMBLER_ERROR;
            }
    
            m_lineTokens.push_back(tv); 
            lineNo ++;
        }

    } 
    else
    {
        std::cout << "Error opening file: " << infile << std::endl;
        return ASSEMBLER_ERROR_FILENOTFOUND;
    }    

    return ASSEMBLER_OK; 
}

AssemblerResult Assembler::assemble(const char* infile, bool compileOnly, bool hexOutput, const char* outfile)
{

    // open infile and tokenize everything, creating a std::vector of lines
    
    AssemblerResult res = tokenize(infile);  

    if (res != ASSEMBLER_OK)
        return res;
 
    // print out token vector

    //printTokens();
    
    // build symbol table
    
    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
        if ((*iter)->getToken(0) != NULL)
            if ((*iter)->getToken(0)->getType() == Token::LABEL)
                m_symbolTable.addSymbol((*iter)->getToken(0)->getToken());

    // print out symbol table
    
    printSymbols();    

    // assemble

}

void Assembler::printTokens()
{

    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
        (*iter)->print();
    
}

void Assembler::printSymbols()
{
    m_symbolTable.print();
}
