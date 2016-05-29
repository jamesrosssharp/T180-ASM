#include <iostream>
#include <string>
#include <string.h>
#include "OpCode.h"


const char* opcodes[] = {
        "ADC",
        "ADD",
        "AND",
        "CALL",
        "CLF",
        "CMP",
        "DEC",
        "HLT",
        "INC",
        "LD",
        "JMP",
        "JNC",
        "JNS",
        "JNZ",
        "MOV",
        "POP",
        "PUSH",
        "RET",
        "ROL",
        "ROR",
        "SBB",
        "ST",
        "SUB",
        "XOR",
        NULL
    };

 
bool OpCode::isOpCode(const char* token)
{
    std::string tok;

    for (int i = 0; i < strlen(token) ; i ++)
        tok.push_back(std::toupper(token[i]));

    const char* opcode;
    int idx = 0;
    
    for (opcode = opcodes[0]; opcode != NULL; opcode = opcodes[idx++])
    {
       std::string opc(opcode);

       if (opc == tok)
        return true;
    } 
 
    return false;
}

