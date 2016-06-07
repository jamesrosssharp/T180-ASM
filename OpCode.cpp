#include <iostream>
#include <string>
#include <string.h>

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

AssemblerResult OpCode::AssembleOpCode(Assembler* assem, int tokenIdx, TokenVector* tv)
{

    const Token* tok = tv->getToken(tokenIdx);
    const Token* operand = tv->getToken(tokenIdx + 1);

    OpCode::Register reg;
    unsigned short value = 0;
    OpCode::AddressingMode add;
        
    AssemblerResult res = OpCode::ParseExpression(assem, operand, reg, value, add); 

    std::string s = Assembler::ToUpper(tok->getToken());

    if (s ==  "ADC")
    {

    } 
    else if (s == "ADD")
    {

        // generate assembly

        tv->resetAssembly();

        switch (add)
        {
            case ADDRESSINGMODE_IMMEDIATE:
                
                tv->pushAssembledByte(0x07);
                tv->pushAssembledByte(value & 0xff);
                tv->pushAssembledByte((value & 0xff00) >> 8);
             
                break;
            default:
                Assembler::PrintError(tv->getFile(), tv->getLine(), tv->getLineNo(), tv->getToken(tokenIdx + 1), "Invalid addressing mode");        
                return ASSEMBLER_INVALID_ADDRESSING_MODE;
                break;
        }

        if (res == ASSEMBLER_ASSEMBLY_DEFERRED)
        {

        } 
        else
        {

        }

    }
    else if (s == "AND")
    {
    

    }
    else if (s == "CALL")
    {

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
       
        tv->resetAssembly();
        tv->pushAssembledByte(0x0f);    // CLF 

        // 3. store address

        tv->setAssembledAddress(assem->getAssemblerAddress());

        // 4. Advance assembler address
        
        assem->advanceAssemblerAddress(1);

    }
    else if (s == "CMP")
    {

    }
    else if (s == "DEC")
    {

    }
    else if (s == "HLT")
    {

    }
    else if (s == "INC")
    {

    }
    else if (s == "LD")
    {

    }
    else if (s == "JMP")
    {

    }
    else if (s == "JNC")
    {

    }
    else if (s == "JNS")
    {

    }
    else if (s == "JNZ")
    {

    }
    else if (s == "MOV")
    {

    }
    else if (s == "POP")
    {

    }
    else if (s == "PUSH")
    {

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

    return ASSEMBLER_OK;

}  
 
AssemblerResult OpCode::ParseExpression(Assembler* assem, const Token* operand, OpCode::Register& reg, unsigned short& value, OpCode::AddressingMode& add)
{

    std::string s = Assembler::ToUpper(operand->getToken());

    const char* str = s.c_str();

    // try to determine addressing mode  

    // First, look for operands of the form "a,<op>"
    // For this case we have to set reg1 to REG_A and parse <op>.


    // indexed
    // string can be "B", "C" or "D" 

    std::cout << s << std::endl;

    if (s == "B")
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
        
    }

    // indexed with operand
    // of form "reg,op"
    


    // immediate 
    // of form "#value +/- #symbol +/- #constant"

    
    // absolute
    // of form "value +/- symbol +/- constant"


}


AssemblerResult OpCode::ParseConstantTerm(Assembler* assem, std::string constant, unsigned short& value)
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

    Token token(Token::OPERAND, "[$100 + SOME_CONSTANT + SOME_SYMBOL] , b");

    OpCode::Register reg = OpCode::REG_NONE;
    OpCode::AddressingMode add = OpCode::ADDRESSINGMODE_NONE;
    unsigned short value = 0;

    AssemblerResult res = OpCode::ParseExpression(&assem, &token, reg, value, add);

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
