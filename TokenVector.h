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
    
    AssemblerResult tokenize(const std::string& file, int lineNo, const std::string& line);

    void print();
    void printAssembly();

    const Token* getToken(int idx) const;
    const std::string* getFile() const;
    const std::string* getLine() const;
    const int     getLineNo() const;

    void resetAssembly();
    void pushAssembledByte(unsigned char byte); 
    
    void setAssembledAddress(unsigned short address);

private:

    std::vector <Token*> m_tokens; 
    
    std::string* m_file;
    std::string* m_line;
    int          m_lineNo;

    unsigned short   m_assembled_address;
    std::vector <unsigned char> m_assembled_bytes; 

};
