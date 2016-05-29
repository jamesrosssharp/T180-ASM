#include "Token.h"
#include <iostream>

Token::Token (TokenType type, const char* token) : 
    m_token(token),
    m_type(type)
{
}

Token::~Token ()
{
}

void Token::print()
{
    std::cout << m_token << " ";

    switch(m_type)
    {

        case LABEL:
            std::cout << "(LABEL)";
            break;
        case OPCODE:
            std::cout << "(OPCODE)";
            break;
        case PSEUDO_OPCODE:
            std::cout << "(PSEUDO_OPCODE)";
            break;
        case OPERAND:
            std::cout << "(OPERAND)";
            break;
    }

    std::cout << std::endl;
}
