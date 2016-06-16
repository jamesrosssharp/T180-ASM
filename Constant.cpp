#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "Constant.h"
#include "ConstantTable.h"

Constant::Constant(const std::string* label, int value) :
    m_label(label->c_str()),
    m_value(value)
{
}

Constant::~Constant()
{
}

const std::string* Constant::getLabel()
{
    return &m_label;
}

const int Constant::getValue()
{
    return m_value;
}

bool operator == (Constant& a, Constant& b)
{

    return *(a.getLabel()) == *(b.getLabel());

}

bool Constant::IsValidHexString(const char* str)
{

    if (! (str[0] == '$') && ! isdigit(str[0]))
        return false; 

    for (int i = 1; i < strlen(str); i++)
    {
        if (! isxdigit(str[i]))
            return false; 
    }

    return true;
}

unsigned short Constant::ParseHexString(const char* tok)
{
      
    char str[1024];

    strncpy(str, &tok[1], 1024);
        
    str[strlen(tok)-1] = '\0';            

    unsigned short hexval = strtol(str, NULL, 16);

    return hexval;
}

bool Constant::IsValidInteger(const char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if(!isdigit(str[i]) || (str[i] == '-' && i == 0))
            return false;
    }

    return true;
}

AssemblerResult  Constant::ParseExpression(ConstantTable* const_table, SymbolTable* symbol_table, const std::string* token, int& val)
{

    AssemblerResult result = ASSEMBLER_OK;

    char *writeable = strdup(token->c_str());    

    char *tok;

    tok = strtok(writeable, " \t");

    val = 0;

    bool    combine_add = true;
    bool    can_combine = true;

    if (tok != NULL)
    do
    {
        //std::cout << tok << std::endl;

        // Hex constant?
        if (tok[0] == '$')
        { 
            if (! can_combine)
            {
                result = ASSEMBLER_BAD_EXPRESSION;
                goto bail;
            }

            char str[1024];

            strncpy(str, &tok[1], 1024);
        
            if (!IsValidHexString(str))
            {
               result = ASSEMBLER_BAD_EXPRESSION;
               goto bail;            
            }

            str[strlen(tok)-1] = '\0';            

            int hexval = strtol(str, NULL, 16);
            
            if (combine_add)
                val += hexval;
            else
                val -= hexval;
    
            can_combine = false;

        }
        else if (IsValidInteger(tok))
        {
            int intval = strtol(tok, NULL, 10);

            if (combine_add)
                val += intval;
            else
                val -= intval;
        }
        else if (strcmp(tok,"+") == 0)
        {
            combine_add = true;
            can_combine = true;
        } 
        else if (strcmp(tok,"-") == 0)
        {
            combine_add = false;
            can_combine = true;
        }
        // check if the token is a symbol or another constant
        else if (const_table != NULL && const_table->hasConstant(tok))
        {

            // getConstant might return null value, check for this
            const int const_val = const_table->getConstant(tok)->getValue();
   
            if (combine_add)
                val += const_val;
            else
                val -= const_val;
 

        }
        else if (symbol_table != NULL && symbol_table->hasSymbol(tok))
        {

            AssemblerResult res;
            unsigned short symAddress;

            // getSymbol might return a null value, check for this
            res = symbol_table->getSymbol(tok)->getAddress(symAddress);

            if (res == ASSEMBLER_ASSEMBLY_COMPLETE)
            {
                if (combine_add)
                    val += symAddress;
                else
                    val -= symAddress;

            } 
            else if (res == ASSEMBLER_ASSEMBLY_DEFERRED)
            {
                result = ASSEMBLER_ASSEMBLY_DEFERRED;
                goto bail;
            }
            else
            {
                result = ASSEMBLER_BAD_EXPRESSION;
                goto bail;
            }
        }
        else
        {
            result = ASSEMBLER_BAD_EXPRESSION;
            goto bail;
        }


    } while (tok = strtok(NULL, " \t"));


bail:
    free (writeable);
    return result;
}

void Constant::print() 
{
    std::cout << m_label << " : " << m_value << std::endl;
}
