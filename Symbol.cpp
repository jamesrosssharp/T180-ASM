#include <iostream>

#include "Symbol.h"

Symbol::Symbol(const std::string* name)   : 
m_name(name->c_str()),
m_address(0)
{
}

Symbol::~Symbol()
{
}

void Symbol::print()
{
   std::cout << "Symbol: " << m_name << " Address: " << m_address << std::endl; 
}
