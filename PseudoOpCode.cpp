#include <iostream>
#include <string>
#include <string.h>
#include "PseudoOpCode.h"

const char* psopcodes[] = 
    {
        "ORG",
        "DC.W",
        "DC.B",
        "EQU",
        NULL
    };


bool PseudoOpCode::isPseudoOpCode(const char* token)
{

    std::string tok;

    for (int i = 0; i < strlen(token) ; i ++)
        tok.push_back(std::toupper(token[i]));

    const char* psopcode;
    int idx = 0;
    
    for (psopcode = psopcodes[0]; psopcode != NULL; psopcode = psopcodes[idx++])
    {
       std::string psopc(psopcode);

       if (psopc == tok)
        return true;
    } 
 
    return false;

}

bool PseudoOpCode::isConstant(const std::string* token)
{
    return *token == std::string("EQU");
}
