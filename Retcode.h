#pragma once

enum AssemblerResult 
{
    ASSEMBLER_OK = 0,
    ASSEMBLER_ERROR,

    // Error codes
    ASSEMBLER_ERROR_FILENOTFOUND = 100,
    ASSEMBLER_BUFFER_OVERFLOW,
    ASSEMBLER_INVALID_TOKEN,

};
