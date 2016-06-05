#pragma once

enum AssemblerResult 
{
    ASSEMBLER_OK = 0,
    ASSEMBLER_ERROR,

    // Error codes
    ASSEMBLER_ERROR_FILENOTFOUND = 100,
    ASSEMBLER_BUFFER_OVERFLOW,
    ASSEMBLER_INVALID_TOKEN,
    ASSEMBLER_LABEL_INVALID,
    ASSEMBLER_BAD_EXPRESSION,
    ASSEMBLER_CONSTANT_REDEFINED,
    ASSEMBLER_INVALID_ADDRESS,
    ASSEMBLER_INVALID_OPCODE,
    ASSEMBLER_INVALID_OPERAND,
    ASSEMBLER_INVALID_ADDRESSING_MODE,
    ASSEMBLER_INVALID_REGISTER,

    // Successful return codes
    ASSEMBLER_ASSEMBLY_COMPLETE = 500,
    ASSEMBLER_ASSEMBLY_DEFERRED,

};
