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

    const Token* getToken(int idx) const;
    const std::string* getFile() const;
    const std::string* getLine() const;
    const int     getLineNo() const;

private:

    std::vector <Token*> m_tokens; 
    
    std::string* m_file;
    std::string* m_line;
    int          m_lineNo;

};
