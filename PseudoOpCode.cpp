#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "PseudoOpCode.h"
#include "Constant.h"
#include "Assembler.h"

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


AssemblerResult PseudoOpCode::handlePseudoOpCode(Assembler* assm, int tokenIdx, TokenVector* tv)
{
    
    const Token* tok = tv->getToken(tokenIdx);

    std::string s = Assembler::ToUpper(tok->getToken());

    if (s == "ORG")
    {
        const std::string* operand = tv->getToken(tokenIdx + 1)->getToken();

        if (Constant::IsValidHexString(operand->c_str())) 
        {
            unsigned short address = Constant::ParseHexString(operand->c_str());
            assm->setAssemblerAddress(address);
    
            return ASSEMBLER_ASSEMBLY_COMPLETE;

        }
        else if (Constant::IsValidInteger(operand->c_str())) 
        {
       
            unsigned short address = atoi(operand->c_str());
            assm->setAssemblerAddress(address);
        
            return ASSEMBLER_ASSEMBLY_COMPLETE;
     
        }
        else        
        {
            Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx + 1), "Invalid address");
            return ASSEMBLER_INVALID_ADDRESS;    
        } 

    }
    else if (s == "DC.B")
    {

    
    }
    else if (s == "DC.W")
    {


    }
    
    return ASSEMBLER_ASSEMBLY_COMPLETE;

} 

