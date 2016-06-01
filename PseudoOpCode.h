#pragma once

#include <string>

class PseudoOpCode
{
 
public:
   
    static  bool isPseudoOpCode(const char* token);
    static  bool isConstant(const std::string* token);

};
