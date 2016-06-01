#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "TokenVector.h"
#include "Assembler.h"
#include "OpCode.h"
#include "PseudoOpCode.h"
#include "Token.h"

TokenVector::TokenVector()
{
}

TokenVector::~TokenVector()
{
    // iterate over token list and destroy all tokens

    for (std::vector<Token*>::iterator iter = m_tokens.begin(); iter != m_tokens.end(); iter++)
        delete (*iter);

    // clean up m_file and m_line
    delete m_file;
    delete m_line;
}   

AssemblerResult TokenVector::tokenize(const std::string& file, int lineNo, const std::string& line)
{
    //
    //  The asm file is structure like so:-
    //
    //  label:<whitespace>opcode<whitespace>operands
    //  
    //  or
    //
    //  <whitespace>opcode<whitespace>operands
    //
    //  we need to parse these formats.
    //
    //  The operands field may have a comment in it, denoted by ;, so strip it out
  
    m_lineNo = lineNo;
    m_file = new std::string(file);
    m_line = new std::string(line);
 
    char* writeable = strdup(line.c_str());
 
    char* token = strtok(writeable, " \t"); 

    bool opCodeFound = false;

    if (token != NULL)
    {
        if (token[strlen(token)-1] == ':')
        {
            
            token[strlen(token)-1] = '\0'; // strip colon

            Token* tok_c = new Token(Token::LABEL, token);
            m_tokens.push_back(tok_c);            
        }
        else if (OpCode::isOpCode(token))
        {
            opCodeFound = true; 

            Token* tok_c = new Token(Token::OPCODE, token);
            m_tokens.push_back(tok_c);            
        }
        else if (PseudoOpCode::isPseudoOpCode(token))
        {
            opCodeFound = true;

            Token* tok_c = new Token(Token::PSEUDO_OPCODE, token);
            m_tokens.push_back(tok_c);            
        }
        else if (token[0] == ';')
        {
            // line is commented out. Return.
            return ASSEMBLER_OK;
        }
        else
        {
            Token* tok_c = new Token(Token::UNKNOWN, token);
            m_tokens.push_back(tok_c);            
        }

    }  


    // get second token (opcode after a label)
    
    if (! opCodeFound)
    {
        token = strtok(NULL, " \t");

        if (token)
        {
            if (token[strlen(token)-1] == ':')
            {
                int tokenPos = line.find(token);

                std::cout << line << std::endl;

                for(int i = 0; i < tokenPos; i ++)
                    std::cout << " ";
                std::cout << "^--- Label invalid here" << std::endl; 

                return ASSEMBLER_LABEL_INVALID;
   
            }
            else if (OpCode::isOpCode(token))
            {
                opCodeFound = true; 
 
                Token* tok_c = new Token(Token::OPCODE, token);
                m_tokens.push_back(tok_c);            
           
             }
            else if (PseudoOpCode::isPseudoOpCode(token))
            {
                opCodeFound = true;
           
                Token* tok_c = new Token(Token::PSEUDO_OPCODE, token);
                m_tokens.push_back(tok_c);      
             }
            else if (token[0] == ';')
            {
                // skip if commented
                return ASSEMBLER_OK;
            }
            else
            {
                // invalid token. find token in the line
               
                int tokenPos = line.find(token);

                std::cout << line << std::endl;

                for(int i = 0; i < tokenPos; i ++)
                    std::cout << " ";
                std::cout << "^--- Invalid token" << std::endl; 

                return ASSEMBLER_INVALID_TOKEN;
            }


        } 
    }

    // get operand field (strip comment)

    if (opCodeFound)
    {

        // string to store operand tokens in
       
        const int bufSize = 1024; 
        char operand[bufSize];
        
        int idx = 0;

        while (((token = strtok(NULL, "\t ")) != NULL) && idx < bufSize)
        {
            // strip comments        
            if (token[0] == ';')
                break;

            // check for overflow
            if (idx + strlen(token) >= bufSize)
            {
                idx = bufSize;
                break;
            }

            strcpy(&operand[idx],token);

            idx += strlen(token);

            operand[idx++] = ' '; 

        }
       
        if (idx >= bufSize)
        {
            return ASSEMBLER_BUFFER_OVERFLOW;
        }

        if (idx > 0)
        {
            operand[idx++] = '\0';
        
            Token* tok_c = new Token(Token::OPERAND, operand);
            m_tokens.push_back(tok_c);      
        }

    }

    free(writeable);

    return ASSEMBLER_OK; 
}

void TokenVector::print()
{
   for (std::vector<Token*>::iterator iter = m_tokens.begin(); iter != m_tokens.end(); iter++)
    (*iter)->print(); 
}

const Token* TokenVector::getToken(int idx) const 
{

    if (idx >= 0 && idx < m_tokens.size())
        return m_tokens[idx];
    else
        return NULL;
}

const std::string* TokenVector::getFile() const
{
    return m_file;
}

const std::string* TokenVector::getLine() const
{
    return m_line;
}

const int     TokenVector::getLineNo() const
{
    return m_lineNo;
}


