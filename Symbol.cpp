#include <iostream>

#include "Symbol.h"

Symbol::Symbol(const std::string* name)   : 
m_name(name->c_str()),
m_address(0),
m_address_set(false)
{
}

Symbol::~Symbol()
{
}

void Symbol::print()
{
   std::cout << "Symbol: " << m_name << " Address: " << m_address << std::endl; 
}

const std::string* Symbol::getLabel()
{
    return &m_name;
}

AssemblerResult Symbol::getAddress(unsigned short& address)
{
    if (m_address_set)
    {
        address = m_address;
        return ASSEMBLER_ASSEMBLY_COMPLETE;
    }

    return ASSEMBLER_ASSEMBLY_DEFERRED;

}

void Symbol::setAddress(unsigned short address)
{
    m_address = address;
    m_address_set = true;
}
