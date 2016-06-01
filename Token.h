#pragma once

#include <string>

class Token 
{

public:

    enum TokenType {
        LABEL,
        OPCODE,
        PSEUDO_OPCODE,
        OPERAND,
        UNKNOWN
    };

    Token(TokenType type, const char* token);
    ~Token();

    void print();

    const TokenType    getType() const;
    const std::string* getToken() const;

private:

    std::string  m_token;
    TokenType    m_type;

};
