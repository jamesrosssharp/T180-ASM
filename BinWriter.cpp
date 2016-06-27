
#include "BinWriter.h"
#include <stdlib.h>
#include <string.h>
#include "Assembler.h"
#include "TokenVector.h"
#include <iostream>
#include <stdio.h>

BinWriter::BinWriter()
{
}

BinWriter::~BinWriter()
{
}

AssemblerResult BinWriter::write(const char* ofile, Assembler& assembler)
{

    AssemblerResult result = ASSEMBLER_OK;

    // Preallocate the maximum memory available for T180 platform (32k)

    const char* fileName;
    const int memorySize = 32768;
    unsigned char* programMemory = (unsigned char*) malloc(memorySize);
    FILE* f; 
    int bytesWritten;
    unsigned short address;
    unsigned short maxAddress = 0;

    if (programMemory == NULL)
    {
        result = ASSEMBLER_MEMORY_ERROR;
        goto bail;
    }

    for (std::vector<TokenVector*>::iterator iter = assembler.lineTokensBegin(); iter != assembler.lineTokensEnd(); ++iter)
    {
   
        TokenVector* tv = *iter;

        if (tv == NULL)
            continue;

        address = tv->getAssembledAddress();
          
        for (std::vector<unsigned char>::iterator byteIterator = tv->beginAssembly(); byteIterator != tv->endAssembly(); ++byteIterator)
        {
            if (address < memorySize)
                programMemory[address++] = *byteIterator;

            if (address > maxAddress)
                maxAddress = address;
        }

    }


    if (ofile == NULL)
        fileName = "a.bin";
    else
        fileName = ofile;

    // open file for writing and write out bytes

    f = fopen(fileName, "wb");

    if (f == NULL)
    {
        std::cout << "Error opening file" << std::endl;
        result = ASSEMBLER_ERROR_OPENINGFILE;
        goto bail;
    }

    bytesWritten = fwrite(programMemory, 1, maxAddress, f);

    if (bytesWritten < maxAddress)
    {
        std::cout << "Error writing to file" << std::endl;
        result = ASSEMBLER_ERROR_WRITINGTOFILE;
    }

    fclose(f);

bail:

    return result;

}
