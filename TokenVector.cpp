#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "TokenVector.h"
#include "Assembler.h"
#include "OpCode.h"
#include "PseudoOpCode.h"

TokenVector::TokenVector()
{
}

TokenVector::~TokenVector()
{
}   

AssemblerResult TokenVector::tokenize(std::string line)
{
    //
    //  The asm file is structure like so:-
    //
    //  label:<whitespace>opcode<whitespace>operands
    //  
    //  or
    //
    //  <whitespace>opcode<whitespace>operands
    //
    //  we need to parse these formats.
    //
   
    char* writeable = strdup(line.c_str());
 
    char* token = strtok(writeable, " \t"); 

    if (token != NULL)
    {
        if (token[strlen(token)-1] == ':')
            std::cout << "Token is a label:" << token << std::endl;
        else if (OpCode::isOpCode(token))
            std::cout << "Token is an opcode:" << token << std::endl;
        else if (PseudoOpCode::isPseudoOpCode(token))
            std::cout << "Token is a pseudo op code:" << token << std::endl;
    }  

    free(writeable);

    return ASSEMBLER_OK; 
}


