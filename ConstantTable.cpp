#include <iostream>
#include "ConstantTable.h"

ConstantTable::ConstantTable()
{
}

ConstantTable::~ConstantTable()
{
}

AssemblerResult ConstantTable::addConstant(TokenVector* tv)
{

    // Sanity check the token vector
    
    const std::string* str = tv->getToken(0)->getToken();

    int val;
    int ret = Constant::ParseExpression(this, NULL, tv->getToken(2)->getToken(), val); 

    if (ret != ASSEMBLER_OK)
    {

        return ASSEMBLER_BAD_EXPRESSION;
    } 

    //std::cout << *str << " " << val << std::endl; 

    // Create a new constant

    Constant* cons = new Constant(str, val);

    // Search through table of constants to see if the constant already exists
  
    if (hasConstant(str->c_str()))
    { 
        delete cons;
        return ASSEMBLER_CONSTANT_REDEFINED;
    }   

    m_constants.push_back(cons);

    return ASSEMBLER_OK; 
}

bool ConstantTable::hasConstant(const char* token)
{
    std::string s(token);

    //std::cout << "Searching for: " << s << std::endl;

     for (std::vector<Constant*>::iterator iter = m_constants.begin(); iter != m_constants.end(); iter++)
     {
        //std::cout << *(*iter)->getLabel() << std::endl;
        if (*(*iter)->getLabel() == s)
            return true; 
     }

    return false;
}

Constant* ConstantTable::getConstant(const char* token) 
{
    std::string s(token);

    //std::cout << "Searching for: " << s << std::endl;

     for (std::vector<Constant*>::iterator iter = m_constants.begin(); iter != m_constants.end(); iter++)
     {
        //std::cout << *(*iter)->getLabel() << std::endl;
        if (*(*iter)->getLabel() == s)
            return (*iter); 
     }

    return NULL;

}

void ConstantTable::print()
{
     for (std::vector<Constant*>::iterator iter = m_constants.begin(); iter != m_constants.end(); iter++)
     {
        (*iter)->print(); 
     }
}
