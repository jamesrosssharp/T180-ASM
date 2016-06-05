#pragma once

#include <string>

#include "Retcode.h"
#include "TokenVector.h"

class Assembler;

class PseudoOpCode
{
 
public:
   
    static  bool isPseudoOpCode(const char* token);
    static  bool isConstant(const std::string* token);

    static  AssemblerResult handlePseudoOpCode(Assembler* assm, int tokenIdx, TokenVector* tv); 

};
