
#pragma once

#include <string>

class Token 
{

public:

    enum TokenType {
        LABEL,
        OPCODE,
        PSEUDO_OPCODE,
        OPERAND
    };

    Token(TokenType type, const char* token);
    ~Token();

    void print();

private:

    std::string  m_token;
    TokenType    m_type;

};
