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
#include "PseudoOpCode.h"
#include "OpCode.h"

Assembler::Assembler()
{
}

Assembler::~Assembler()
{
    // iterate over m_lineTokens and destroy all the token vectors

    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
        delete (*iter);

}

void Assembler::PrintError(const std::string* file, const std::string* line, const int lineNo, const Token* token, const char* errorMsg)
{
    int tokenPos = line->find((token->getToken())->c_str());

    std::cout << "!!!" << token->getToken()->c_str() << "!!!";

    std::cout << std::endl << "In file " << *file << ", line " << lineNo << ":" << std::endl;

    std::cout << *line << std::endl;

    for(int i = 0; i < tokenPos; i ++)
        std::cout << " ";
    std::cout << "^--- " << errorMsg << std::endl; 
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


std::string Assembler::ToUpper(const std::string* s)
{
    
    std::string tok;

    for (int i = 0; i < s->length(); i ++)
        tok.push_back(std::toupper((*s)[i]));

    return tok;

}

void Assembler::setAssemblerAddress(unsigned short address)
{

    //std::cout << "Setting assembler address " << address << std::endl;

    m_address = address;
}

unsigned short Assembler::getAssemblerAddress()
{
    return m_address;
}

void    Assembler::advanceAssemblerAddress(unsigned short num_bytes)
{
    m_address += num_bytes;
}

AssemblerResult Assembler::assembleLine(TokenVector* tv)
{

    bool lineAssembled = false;

    const Token* tok1 = tv->getToken(0);

    if (tok1 == NULL)
        return ASSEMBLER_OK;

    switch (tok1->getType())
    {
        case Token::LABEL:
            //tok1->setLabelAddress();
            // find the symbol in the symbol table 
            break; 
        case Token::OPCODE:
        {
            AssemblerResult res = OpCode::AssembleOpCode(this, 0, tv);

            if (res == ASSEMBLER_ASSEMBLY_COMPLETE)
                lineAssembled = true;
            else if (res != ASSEMBLER_OK)
                return res;

            break;
        }
        case Token::PSEUDO_OPCODE:
        {
            AssemblerResult res = PseudoOpCode::handlePseudoOpCode(this, 0, tv);

            if (res == ASSEMBLER_ASSEMBLY_COMPLETE)
                lineAssembled = true;
            else if (res != ASSEMBLER_OK)
                return res;

            break;
        }
    }

    if (lineAssembled)
    {

    }
    else
    {


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

    // build table of constants

    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
        if ((*iter)->getToken(1) != NULL)
             if ((*iter)->getToken(1)->getType() == Token::PSEUDO_OPCODE && PseudoOpCode::isConstant((*iter)->getToken(1)->getToken()))
             {
                AssemblerResult result;  
                result = m_constantTable.addConstant(*iter);

                if (result != ASSEMBLER_OK)
                {
                    switch (result)
                    {
                        case ASSEMBLER_CONSTANT_REDEFINED:  
                            PrintError((*iter)->getFile(), (*iter)->getLine(), (*iter)->getLineNo(), (*iter)->getToken(0), 
                                        "Constant redefined");
                            break;
                        case ASSEMBLER_BAD_EXPRESSION:
                            PrintError((*iter)->getFile(), (*iter)->getLine(), (*iter)->getLineNo(), (*iter)->getToken(2),
                                        "Bad expression");
                            break;
                        default:
                            PrintError((*iter)->getFile(), (*iter)->getLine(), (*iter)->getLineNo(), (*iter)->getToken(0),
                                        "Unspecified error");
                            break;
                    }
                    return result;
                 }

             }
 
    // build symbol table
    
    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
        if ((*iter)->getToken(0) != NULL)
            if ((*iter)->getToken(0)->getType() == Token::LABEL)
                m_symbolTable.addSymbol((*iter)->getToken(0)->getToken());

    // print out symbol table
    
    //printSymbols();    
    //printConstants();

    // assemble
 
    setAssemblerAddress(0);

    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
    {
        AssemblerResult result;

        result = assembleLine(*iter);

        switch (result)
        {   
            case ASSEMBLER_OK:
                break;
            default:
                std::cout << "Error" << std::endl;
                return result; 
        }
    }

    // print assembly
    
    for (std::vector<TokenVector*>::iterator iter = m_lineTokens.begin(); iter != m_lineTokens.end(); iter++)
    {
        (*iter)->printAssembly();
    }
     
    std::cout << std::endl;
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

void Assembler::printConstants()
{
    m_constantTable.print();
}
