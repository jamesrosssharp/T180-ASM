#pragma once

#include <vector>
#include <string>

#include "Token.h"
#include "Retcode.h"

class TokenVector
{

public:

    TokenVector();
    ~TokenVector();
    
    AssemblerResult tokenize(std::string line);

    void print();

private:

    std::vector <Token*> m_tokens; 

};
