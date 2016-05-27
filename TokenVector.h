#pragma once

#include <vector>
#include <string>

#include "Retcode.h"

class TokenVector
{

public:

    TokenVector();
    ~TokenVector();
    
    AssemblerResult tokenize(std::string line);

private:

    std::vector <std::string*> tokens; 

};
