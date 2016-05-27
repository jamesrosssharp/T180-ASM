//
//  T180 ASM : assembler for T180 soft core CPU
//

#include <cstdlib>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

#include "Assembler.h"

void printUsage(char *arg0)
{
    std::cout << "Usage: " << arg0 << " [-cH] [-o <output.o>] <file.asm>" << std::endl;
    std::cout << std::endl;
    std::cout << "      options: " << std::endl;
    std::cout << "          -c: compile only " << std::endl;
    std::cout << "          -H: compile and link to Intel Hex format output" 
                << std::endl;
}

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }

    bool compileOnly = false;
    bool hexOutput = false;
    int c;   
    char *outputFile = NULL; 

    while ((c = getopt (argc, argv, "cHo:")) != -1)
        switch (c)
        {
            case 'c':
                compileOnly = true;
                break;
            case 'H':
                hexOutput = true;
                break;
            case 'o':
                outputFile = strdup(optarg);
                break;
            default:
                printUsage(argv[0]);
                exit(EXIT_FAILURE);
        }

    if (compileOnly && hexOutput)
    {
        std::cout << "ERROR: cannot have HEX output if compiling only" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (compileOnly && outputFile == NULL)
    {
        std::cout << "ERROR: must specify output file if compiling only" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (optind == argc)
    {
        std::cout << "ERROR: no input files" << std::endl;
    }
    
    Assembler a;

    AssemblerResult result = a.assemble(argv[optind], compileOnly, hexOutput, outputFile);

    if (result == ASSEMBLER_OK)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE); 
    
}
