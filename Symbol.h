#pragma once

#include <string>

class Symbol 
{

public:

    Symbol(const std::string* name);
    ~Symbol();

    void print();

private:

    std::string m_name;  

    int m_address;

};
