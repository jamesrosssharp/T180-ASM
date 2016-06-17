#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "OpCode.h"
#include "Assembler.h"

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

 
bool OpCode::IsOpCode(const char* token)
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

bool OpCode::IsRegister(const std::string& str)
{
    if (str == "A")
        return true;
    else if (str == "B")
        return true;
    else if (str == "C")
        return true;
    else if (str == "D")
        return true;
    else if (str == "S")
        return true;

    return false;
}


AssemblerResult OpCode::AssembleOpCode(Assembler* assem, int tokenIdx, TokenVector* tv)
{

    const Token* tok = tv->getToken(tokenIdx);
    const Token* operand = tv->getToken(tokenIdx + 1);

    OpCode::Register reg;
    OpCode::Register reg2;
    unsigned short value = 0;
    OpCode::AddressingMode add;
        
    AssemblerResult res = OpCode::ParseExpression(assem, operand, reg, reg2, value, add); 

    AssemblerResult error = ASSEMBLER_OK;

    switch (res)
    {
        case ASSEMBLER_ASSEMBLY_COMPLETE:
        case ASSEMBLER_ASSEMBLY_DEFERRED:
        case ASSEMBLER_OK:
            break;
        default:
           Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx + 1), Assembler::ReturnCodeToString(res)); 
           return res;       
    }


    std::string s = Assembler::ToUpper(tok->getToken());

    tv->resetAssembly();
    tv->setAssembledAddress(assem->getAssemblerAddress());

    if (s ==  "ADC")
    {

        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_IMMEDIATE:

                tv->pushAssembledByte(0x19);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_ABSOLUTE:

                tv->pushAssembledByte(0x1e);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_INDEXED:

                switch (reg)
                {
                    case OpCode::REG_B:                              
                        tv->pushAssembledByte(0x43);
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x6b);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x93);
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                } 

                assem->advanceAssemblerAddress(1);                

                break;
            case ADDRESSINGMODE_INDEXED_WITH_OPERAND:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x4d); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x75);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x9d);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x57); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x7f);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xa7);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT_WITH_INDEX: 

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x61); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x89);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xb1);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }
 

    } 
    else if (s == "ADD")
    {

        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_IMMEDIATE:

                tv->pushAssembledByte(0x07);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_ABSOLUTE:

                tv->pushAssembledByte(0x1d);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_INDEXED:

                switch (reg)
                {
                    case OpCode::REG_B:                              
                        tv->pushAssembledByte(0x42);
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x6a);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x92);
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                } 

                assem->advanceAssemblerAddress(1);                

                break;
            case ADDRESSINGMODE_INDEXED_WITH_OPERAND:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x4c); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x74);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x9c);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x56); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x7e);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xa6);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT_WITH_INDEX: 

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x60); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x88);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xb0);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }
 
    }
    else if (s == "AND")
    {

        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_IMMEDIATE:

                tv->pushAssembledByte(0x05);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_ABSOLUTE:

                tv->pushAssembledByte(0x1b);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_INDEXED:

                switch (reg)
                {
                    case OpCode::REG_B:                              
                        tv->pushAssembledByte(0x40);
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x68);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x90);
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                } 

                assem->advanceAssemblerAddress(1);                

                break;
            case ADDRESSINGMODE_INDEXED_WITH_OPERAND:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x4a); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x72);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x9a);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x54); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x7c);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xa4);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT_WITH_INDEX: 

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x5e); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x86);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xae);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }
    

    }
    else if (s == "CALL")
    {
       
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_ABSOLUTE:        

                tv->pushAssembledByte(0x13);

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }

 
    }
    else if (s == "CLF")
    {

        // CLF is a single byte opcode without operand
        
        // 1. validate operand - should be empty token
       
        if (operand != NULL)
        {
            Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx + 1), "Operand invalid here");
            return ASSEMBLER_INVALID_OPERAND;    
        } 
        
        // 2. generate assembly
       
        tv->pushAssembledByte(0x0f);    // CLF 

        // 3. Advance assembler address
        
        assem->advanceAssemblerAddress(1);

    }
    else if (s == "CMP")
    {

        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_IMMEDIATE:

                tv->pushAssembledByte(0x0a);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_ABSOLUTE:

                tv->pushAssembledByte(0x21);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            case ADDRESSINGMODE_INDEXED:

                switch (reg)
                {
                    case OpCode::REG_B:                              
                        tv->pushAssembledByte(0x46);
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x6e);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x96);
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                } 

                assem->advanceAssemblerAddress(1);                

                break;
            case ADDRESSINGMODE_INDEXED_WITH_OPERAND:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x50); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x78);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xa0);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT:

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x5a); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x82);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xaa);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);

                break;
            case ADDRESSINGMODE_INDIRECT_WITH_INDEX: 

                switch (reg)
                {
                   case OpCode::REG_B:
                        tv->pushAssembledByte(0x64); 
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x8c);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0xb4);
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
        
                assem->advanceAssemblerAddress(3);
 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }
    }
    else if (s == "DEC")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_INDEXED:
    
                switch (reg)
                {
                    case OpCode::REG_A:
                        tv->pushAssembledByte(0x16);
                        break; 
                    case OpCode::REG_B:
                        tv->pushAssembledByte(0x35);
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x39);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x3c);
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }

                assem->advanceAssemblerAddress(1);

                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }
      
    }
    else if (s == "HLT")
    {
        if (operand != NULL)
        {
            Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx + 1), "Operand invalid here");
            return ASSEMBLER_INVALID_OPERAND;    
        } 
        
        // 2. generate assembly
       
        tv->pushAssembledByte(0x00);    // halt 

        // 3. Advance assembler address
        
        assem->advanceAssemblerAddress(1);
    }
    else if (s == "INC")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_INDEXED:
    
                switch (reg)
                {
                    case OpCode::REG_A:
                        tv->pushAssembledByte(0x15);
                        break; 
                    case OpCode::REG_B:
                        tv->pushAssembledByte(0x34);
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x38);
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x3b);
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;

                }

                assem->advanceAssemblerAddress(1);

                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
                break;
        }
    
    }
    else if (s == "LD")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        if (reg == OpCode::REG_A)
        {
            switch (add)
            {
                case ADDRESSINGMODE_IMMEDIATE:
                    tv->pushAssembledByte(0x18);
                    tv->pushAssembledByte(value & 0xff);
                    tv->pushAssembledByte((value & 0xff00) >> 8);
                    assem->advanceAssemblerAddress(3);
                    break;
                case ADDRESSINGMODE_ABSOLUTE:
                    tv->pushAssembledByte(0x01);
                    tv->pushAssembledByte(value & 0xff);
                    tv->pushAssembledByte((value & 0xff00) >> 8); 
                    assem->advanceAssemblerAddress(3);
                    break;
                case ADDRESSINGMODE_INDEXED:
                    switch (reg2)
                    {
                        case OpCode::REG_B:
                            tv->pushAssembledByte(0x3e);
                            break;
                        case OpCode::REG_C:
                            tv->pushAssembledByte(0x66);
                            break;
                        case OpCode::REG_D:
                            tv->pushAssembledByte(0x8e);
                            break;
                        default:
                            error = ASSEMBLER_INVALID_REGISTER;
                            goto bail; 
                    }
                    assem->advanceAssemblerAddress(1);
                    break;
                case ADDRESSINGMODE_INDEXED_WITH_OPERAND:
                    switch (reg2)
                    {
                        case OpCode::REG_B:
                            tv->pushAssembledByte(0x48);
                            break;
                        case OpCode::REG_C:
                            tv->pushAssembledByte(0x70);
                            break;
                        case OpCode::REG_D:
                            tv->pushAssembledByte(0x98);
                            break;
                        default:
                            error = ASSEMBLER_INVALID_REGISTER;
                            goto bail;
                    }
                    
                    tv->pushAssembledByte(value & 0xff);
                    tv->pushAssembledByte((value & 0xff00) >> 8);
                    assem->advanceAssemblerAddress(3);

                    break;
                case ADDRESSINGMODE_INDIRECT:

                    switch (reg2)
                    {
                        case OpCode::REG_B:
                            tv->pushAssembledByte(0x52);
                            break;
                        case OpCode::REG_C:
                            tv->pushAssembledByte(0x7a);
                            break;
                        case OpCode::REG_D:
                            tv->pushAssembledByte(0xa2);
                            break;
                        default:
                            error = ASSEMBLER_INVALID_REGISTER;
                            goto bail;
                    }

                    tv->pushAssembledByte(value & 0xff);
                    tv->pushAssembledByte((value & 0xff00) >> 8);
                    assem->advanceAssemblerAddress(3);

                    break;
                case ADDRESSINGMODE_INDIRECT_WITH_INDEX:

                    switch (reg2)
                    {
                        case OpCode::REG_B:
                            tv->pushAssembledByte(0x5c);
                            break;
                        case OpCode::REG_C:
                            tv->pushAssembledByte(0x84);
                            break;
                        case OpCode::REG_D:
                            tv->pushAssembledByte(0xac);
                            break;
                        default:
                            error = ASSEMBLER_INVALID_REGISTER;
                            goto bail;
                    }

                    tv->pushAssembledByte(value & 0xff);
                    tv->pushAssembledByte((value & 0xff00) >> 8);
                    assem->advanceAssemblerAddress(3);

                    break;
                default:
                    error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                    goto bail;                    
            }
        }
        else
        {
            switch (reg)
            {
                case OpCode::REG_B:
                    switch (add)
                    {
                        case ADDRESSINGMODE_ABSOLUTE:
                            tv->pushAssembledByte(0x23);
                            tv->pushAssembledByte(value & 0xff);
                            tv->pushAssembledByte((value & 0xff00) >> 8);
                            assem->advanceAssemblerAddress(3);
                            break;
                        case ADDRESSINGMODE_IMMEDIATE:
                            tv->pushAssembledByte(0x27);
                            tv->pushAssembledByte(value & 0xff);
                            tv->pushAssembledByte((value & 0xff00) >> 8);
                            assem->advanceAssemblerAddress(3);
                            break;
                        default:
                            error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                            goto bail;
                    }
  
                    break;
                case OpCode::REG_C:
                    switch (add)
                    {
                        case ADDRESSINGMODE_ABSOLUTE:
                            tv->pushAssembledByte(0x28);
                            tv->pushAssembledByte(value & 0xff);
                            tv->pushAssembledByte((value & 0xff00) >> 8);
                            assem->advanceAssemblerAddress(3);
                            break;
                        case ADDRESSINGMODE_IMMEDIATE:
                            tv->pushAssembledByte(0x2C);
                            tv->pushAssembledByte(value & 0xff);
                            tv->pushAssembledByte((value & 0xff00) >> 8);
                            assem->advanceAssemblerAddress(3);
                            break;
                        default:
                            error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                            goto bail;
                    }
                    break;
                case OpCode::REG_D:
                    switch (add)
                    {
                        case ADDRESSINGMODE_ABSOLUTE:
                            tv->pushAssembledByte(0x2d);
                            tv->pushAssembledByte(value & 0xff);
                            tv->pushAssembledByte((value & 0xff00) >> 8);
                            assem->advanceAssemblerAddress(3);
                            break;
                        case ADDRESSINGMODE_IMMEDIATE:
                            tv->pushAssembledByte(0x31);
                            tv->pushAssembledByte(value & 0xff);
                            tv->pushAssembledByte((value & 0xff00) >> 8);
                            assem->advanceAssemblerAddress(3);
                            break;

                        default:
                            error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                            goto bail;
                    }
                    break;
                default:
                    error = ASSEMBLER_INVALID_REGISTER;
                    goto bail;
            }
        }
    
    }
    else if (s == "JMP")
    {
        
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_ABSOLUTE:
                tv->pushAssembledByte(0x03);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
                assem->advanceAssemblerAddress(3); 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
        } 

    }
    else if (s == "JNC")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_ABSOLUTE:
                tv->pushAssembledByte(0x0c);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
                assem->advanceAssemblerAddress(3); 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
        } 


    }
    else if (s == "JNS")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_ABSOLUTE:
                tv->pushAssembledByte(0x0b);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
                assem->advanceAssemblerAddress(3); 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
        } 


    }
    else if (s == "JNZ")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_ABSOLUTE:
                tv->pushAssembledByte(0x09);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
                assem->advanceAssemblerAddress(3); 
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
        } 


    }
    else if (s == "MOV")
    {

        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_REGISTER_TO_REGISTER:

                switch (reg)
                {
                    case OpCode::REG_A:
       
                        switch (reg2)
                        {
                            case OpCode::REG_B:
                                tv->pushAssembledByte(0x24);
                                assem->advanceAssemblerAddress(1); 
                                break;
                            case OpCode::REG_C:
                                tv->pushAssembledByte(0x29);
                                assem->advanceAssemblerAddress(1);
                                break;
                            case OpCode::REG_D:
                                tv->pushAssembledByte(0x2e);
                                assem->advanceAssemblerAddress(1); 
                                break;
                            default:
                                error = ASSEMBLER_INVALID_REGISTER;
                                goto bail;

                        }
 
                        break;
                    case OpCode::REG_B:
                        switch (reg2)
                        {
                            case OpCode::REG_A:
                                tv->pushAssembledByte(0x25);
                                assem->advanceAssemblerAddress(1); 
                                break;
                            default:
                                error = ASSEMBLER_INVALID_REGISTER;
                                goto bail;

                        }
                        break;
                    case OpCode::REG_C:
                        switch (reg2)
                        {
                            case OpCode::REG_A:
                                tv->pushAssembledByte(0x2a);
                                assem->advanceAssemblerAddress(1); 
                                break;
                            default:
                                error = ASSEMBLER_INVALID_REGISTER;
                                goto bail;

                        }
                        break;
                    case OpCode::REG_D:
                        switch (reg2)
                        {
                            case OpCode::REG_A:
                                tv->pushAssembledByte(0x2f);
                                assem->advanceAssemblerAddress(1); 
                                break;
                            default:
                                error = ASSEMBLER_INVALID_REGISTER;
                                goto bail;

                        }
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;

                }

                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;

        }

    }
    else if (s == "POP")
    {

        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_INDEXED:
                switch (reg)
                {
                    case OpCode::REG_A:
                        tv->pushAssembledByte(0x12);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_B:
                        tv->pushAssembledByte(0x33);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x37);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x3b);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_S:
                        tv->pushAssembledByte(0xb7);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
        } 

    }
    else if (s == "PUSH")
    {
        if (operand == NULL)
        {
            error = ASSEMBLER_ARGUMENT_REQUIRED;
            goto bail;
        }

        switch (add)
        {
            case ADDRESSINGMODE_INDEXED:
                switch (reg)
                {
                    case OpCode::REG_A:
                        tv->pushAssembledByte(0x11);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_B:
                        tv->pushAssembledByte(0x32);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_C:
                        tv->pushAssembledByte(0x36);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_D:
                        tv->pushAssembledByte(0x3a);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    case OpCode::REG_S:
                        tv->pushAssembledByte(0xb6);
                        assem->advanceAssemblerAddress(1);      
                        break;
                    default:
                        error = ASSEMBLER_INVALID_REGISTER;
                        goto bail;
                }
                break;
            default:
                error = ASSEMBLER_INVALID_ADDRESSING_MODE;
                goto bail;
        } 
    }
    else if (s == "RET")
    {

    }
    else if (s == "ROL")
    {

    }
    else if (s == "ROR")
    {

    }
    else if (s == "SBB")
    {

    }
    else if (s == "ST")
    {

    }
    else if (s == "SUB")
    {

    }
    else if (s == "XOR")
    {

    }
    else 
    {
        Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx), "Invalid opcode");
        return ASSEMBLER_INVALID_OPCODE;    
    }   


bail:

    if (error != ASSEMBLER_OK)
    {
        Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx + 1), Assembler::ReturnCodeToString(error)); 
        return error;
    }


    return ASSEMBLER_OK;

}  
 
AssemblerResult OpCode::ParseExpression(Assembler* assem, const Token* operand, OpCode::Register& reg, OpCode::Register& reg2, unsigned short& value, OpCode::AddressingMode& add)
{


    if (operand->getToken() == NULL)
        return ASSEMBLER_ASSEMBLY_COMPLETE;

    std::string s = Assembler::Trim(Assembler::ToUpper(operand->getToken()));

    const char* str = s.c_str();

    // try to determine addressing mode  

    reg2 = OpCode::REG_NONE;

    // indexed / register
    // string can be "A", "B", "C", "D" or "S" 

    //std::cout << s << std::endl;

    if (s == "A")
    {
        reg = OpCode::REG_A;
        value = 0;
        add = OpCode::ADDRESSINGMODE_INDEXED; // misnomer, but we don't have much choice

        return ASSEMBLER_ASSEMBLY_COMPLETE;

    }
    else if (s == "B")
    {
        reg = OpCode::REG_B;
        value = 0;
        add = OpCode::ADDRESSINGMODE_INDEXED;

        return ASSEMBLER_ASSEMBLY_COMPLETE; 
    } 
    else if (s == "C")
    {
        reg = OpCode::REG_C;
        value = 0;
        add = OpCode::ADDRESSINGMODE_INDEXED;

        return ASSEMBLER_ASSEMBLY_COMPLETE;
    }
    else if (s == "D")
    {
        reg = OpCode::REG_D;
        value = 0;
        add = OpCode::ADDRESSINGMODE_INDEXED;  
    
        return ASSEMBLER_ASSEMBLY_COMPLETE; 
    }

    // indirect
    // can be of form "[reg, op]" 

    else if (str[0] == '[' && str[strlen(str) - 1] == ']')
    {
        add = OpCode::ADDRESSINGMODE_INDIRECT;
        
        std::string subs = s.substr(1,s.length() - 2);
    
        int commapos = subs.find(",");

        std::string regstr = subs.substr(0, commapos);
        std::string constant = subs.substr(commapos+1, subs.length());

        if (regstr == "B")
        {
            reg = OpCode::REG_B;
        }
        else if (regstr == "C")
        {
            reg = OpCode::REG_C;
        } 
        else if (regstr == "D")
        {
            reg = OpCode::REG_D;
        }
        else
            return ASSEMBLER_INVALID_REGISTER;

        // parse the constant part of the expression

        AssemblerResult res = OpCode::ParseConstantTerm(assem, constant, value);
        
        return res;

    }

    // indirect with index
    // of form "[op],reg" 

    else if (str[0] == '[')
    {
        
        add = OpCode::ADDRESSINGMODE_INDIRECT_WITH_INDEX;
        
        int commapos = s.find(",");

        if (commapos == -1)
        {
            return ASSEMBLER_BAD_EXPRESSION;
        }

        std::string regstr = s.substr(commapos + 1, s.length());

        std::string regstrClean = Assembler::Trim(regstr);

        if (regstrClean == "B")
        {
            reg = OpCode::REG_B;
        }
        else if (regstrClean == "C")
        {
            reg = OpCode::REG_C;
        } 
        else if (regstrClean == "D")
        {
            reg = OpCode::REG_D;
        }
        else
            return ASSEMBLER_INVALID_REGISTER;

        // fish out operand string

        std::string opstr = s.substr(0, commapos);
    
        int openBracketPos = opstr.find("[");
        int closeBracketPos = opstr.find("]");

        std::string opstrClean = opstr.substr(openBracketPos + 1, closeBracketPos - 1);

        AssemblerResult res = OpCode::ParseConstantTerm(assem, opstrClean, value);
        
        return res;

    }


    int commaPos = s.find(",");


   
    if (commaPos > 0)
    {
 
        std::string s1 = Assembler::Trim(s.substr(0, commaPos));
        std::string s2 = Assembler::Trim(s.substr(commaPos+1, s.length()));

 
             // search for register to register operations
    
        if (OpCode::IsRegister(s1), OpCode::IsRegister(s2))
        {
            add = OpCode::ADDRESSINGMODE_REGISTER_TO_REGISTER;

            if (s1 == "A")
            {
                reg = OpCode::REG_A;
            }
            else if (s1 == "B")
            {
                reg = OpCode::REG_B;
            }
            else if (s1 == "C")
            {
                reg = OpCode::REG_C;
            }
            else if (s1 == "D")
            {
                reg = OpCode::REG_D;
            }
            else if (s1 == "S")
            {
                reg = OpCode::REG_S;
            }

            if (s2 == "A")
            {
                reg2 = OpCode::REG_A;
            }
            else if (s2 == "B")
            {
                reg2 = OpCode::REG_B;
            }
            else if (s2 == "C")
            {
                reg2 = OpCode::REG_C;
            }
            else if (s2 == "D")
            {
                reg2 = OpCode::REG_D;
            }
            else if (s2 == "S")
            {
                reg2 = OpCode::REG_S;
            }

            return ASSEMBLER_ASSEMBLY_COMPLETE;

        }


        // Look for operands of the form "a,<op>"
        // For this case we have to set reg1 to REG_A and parse <op>.

        else if (s1 == "A")
        {

            reg = OpCode::REG_A;

            OpCode::Register reg3;

            Token tok(Token::OPERAND, s2.c_str());

            return  OpCode::ParseExpression(assem, &tok, reg2, reg3, value, add);


        } 

        // indexed with operand
        // of form "reg,op"
    
        else 
        {
            add = OpCode::ADDRESSINGMODE_INDEXED_WITH_OPERAND;
        
            std::string regStr = Assembler::Trim(s.substr(0, commaPos));

            if (regStr == "B")
            {
                reg = OpCode::REG_B;
            }
            else if (regStr == "C")
            {
                reg = OpCode::REG_C;
            } 
            else if (regStr == "D")
            {
                reg = OpCode::REG_D;
            }
            else
                return ASSEMBLER_INVALID_REGISTER;

            std::string opStr = Assembler::Trim(s.substr(commaPos+1, s.length())); 

            AssemblerResult res = OpCode::ParseConstantTerm(assem, opStr, value);
            
            return res;
        }

    }

    // immediate 
    // of form "#value +/- #symbol +/- #constant"

    else if (str[0] == '#')
    {

        add = OpCode::ADDRESSINGMODE_IMMEDIATE;
        
        char newStr[1024]; 
        int idx = 0;

        // TODO: watch out for buffer overflows here

        char *writeable = strdup(str);
        char *token = strtok(writeable, "\t ");

        while (token != NULL)
        {
           
            if (token[0] == '#')
            {
                strcpy(&newStr[idx], &token[1]);
                idx += strlen(token); 
                newStr[idx - 1] = ' ';
            }
            else
            {
                strcpy(&newStr[idx], token);
                idx += strlen(token) + 1; 
                newStr[idx - 1] = ' ';
            } 

            token = strtok(NULL, "\t ");

        }  

        newStr[idx] = '\0';

        std::string newS(newStr);

        free (writeable);
  
        //std::cout << newS << std::endl;
         
        AssemblerResult res = OpCode::ParseConstantTerm(assem, newS, value);
        
        return res;
    }
 
    // absolute
    // of form "value +/- symbol +/- constant"

    else
    {
        add = OpCode::ADDRESSINGMODE_ABSOLUTE;
        
        AssemblerResult res = OpCode::ParseConstantTerm(assem, s, value);
        
        return res;
    }


}


AssemblerResult OpCode::ParseConstantTerm(Assembler* assem, const std::string& constant, unsigned short& value)
{
    
    int val;
    AssemblerResult res = Constant::ParseExpression(assem->getConstantTable(), assem->getSymbolTable(), &constant, val); 

    value = (unsigned short)val;

    return res;

}

#ifdef UNIT_TEST

int main(int argc, char *argv[])
{

    Assembler assem;

    assem.addConstant("SOME_CONSTANT",0xaa);

    assem.addSymbol("SOME_SYMBOL");

    assem.setAddressForSymbol("SOME_SYMBOL", 0xfe00);

    Token token(Token::OPERAND, "a, [b, $100 + SOME_CONSTANT + SOME_SYMBOL]");
    //Token token(Token::OPERAND, "a, d");

    OpCode::Register reg = OpCode::REG_NONE;
    OpCode::Register reg2 = OpCode::REG_NONE;
    OpCode::AddressingMode add = OpCode::ADDRESSINGMODE_NONE;
    unsigned short value = 0;

    AssemblerResult res = OpCode::ParseExpression(&assem, &token, reg, reg2, value, add);

    std::cout << "Register: ";
    
    switch (reg)
    {
        case OpCode::REG_NONE:
            std::cout << "None";
            break;
        case OpCode::REG_A:
            std::cout << "A";
            break;
        case OpCode::REG_B:
            std::cout << "B";
            break;
        case OpCode::REG_C:
            std::cout << "C";
            break;
        case OpCode::REG_D:
            std::cout << "D"; 
            break;
    }

    std::cout << " Register 2: ";

    switch (reg2)
    {
        case OpCode::REG_NONE:
            std::cout << "None";
            break;
        case OpCode::REG_A:
            std::cout << "A";
            break;
        case OpCode::REG_B:
            std::cout << "B";
            break;
        case OpCode::REG_C:
            std::cout << "C";
            break;
        case OpCode::REG_D:
            std::cout << "D"; 
            break;
    }



    std::cout << " Addressing mode: ";

    switch (add)
    {
        case OpCode::ADDRESSINGMODE_NONE:
            std::cout << "None";
            break;
        case OpCode::ADDRESSINGMODE_IMMEDIATE:
            std::cout << "Immediate";
            break;
        case OpCode::ADDRESSINGMODE_ABSOLUTE:
            std::cout << "Absolute";
            break;
        case OpCode::ADDRESSINGMODE_INDEXED:
            std::cout << "Indexed";
            break;
        case OpCode::ADDRESSINGMODE_INDEXED_WITH_OPERAND:
            std::cout << "Indexed with operand";
            break;
        case OpCode::ADDRESSINGMODE_INDIRECT:
            std::cout << "Indirect";
            break;
        case OpCode::ADDRESSINGMODE_INDIRECT_WITH_INDEX:
            std::cout << "Indirect with index";
            break;
        case OpCode::ADDRESSINGMODE_REGISTER_TO_REGISTER:
            std::cout << "Register to register";
    }

    std::cout << " Value: ";

    std::cout << value;

    std::cout << " Result: "; 

    switch (res)
    {
        case ASSEMBLER_ASSEMBLY_COMPLETE:
            std::cout << "Assembly complete";
            break; 
        case ASSEMBLER_INVALID_REGISTER:
            std::cout << "Invalid register";
            break;
        case ASSEMBLER_BAD_EXPRESSION:
            std::cout << "Bad expression";
            break;
        case ASSEMBLER_ASSEMBLY_DEFERRED:
            std::cout << "Assembly deferred";
            break;
        case ASSEMBLER_OK:
            std::cout << "Ok";
            break;
        default:
            std::cout << res;
    }

    std::cout << std::endl;

}

#endif
