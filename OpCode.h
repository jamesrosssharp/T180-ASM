#include "Retcode.h"
#include "TokenVector.h"

class Assembler;

class OpCode
{
 
public:   

    enum AddressingMode 
    {
        ADDRESSINGMODE_NONE,
        ADDRESSINGMODE_IMMEDIATE,
        ADDRESSINGMODE_ABSOLUTE,
        ADDRESSINGMODE_INDEXED,
        ADDRESSINGMODE_INDEXED_WITH_OPERAND,
        ADDRESSINGMODE_INDIRECT,
        ADDRESSINGMODE_INDIRECT_WITH_INDEX, 
    };

    enum Register
    {
        REG_NONE,
        REG_A,
        REG_B,
        REG_C,
        REG_D,
        REG_S
    }; 

    static  bool IsOpCode(const char* token);
    static  AssemblerResult AssembleOpCode(Assembler* assem, int tokenIdx, TokenVector* tv);   

    static  AssemblerResult ParseExpression(Assembler* assem, const Token* operand, OpCode::Register& reg, unsigned short& value, OpCode::AddressingMode& add);

};
