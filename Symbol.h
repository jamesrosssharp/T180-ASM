#pragma once

#include <string>
#include "Retcode.h"

class Symbol 
{

public:

    Symbol(const std::string* name);
    ~Symbol();

    const std::string* getLabel();
    AssemblerResult getAddress(unsigned short& address);

    void print();

    void setAddress(unsigned short address);

private:

    std::string m_name;  

    unsigned short m_address;
    bool    m_address_set;

};
