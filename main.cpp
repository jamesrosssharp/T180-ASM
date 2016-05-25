//
//  T180 ASM : assembler for T180 soft core CPU
//

#include <cstdlib>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

void printUsage(char *arg0)
{
    std::cout << "Usage: " << arg0 << " [-cH] <file.asm>" << std::endl;
    std::cout << std::endl;
    std::cout << "      options: " << std::endl;
    std::cout << "          -c: compile only " << std::endl;
    std::cout << "          -H: Intel Hex format output (defaults to gnu elf" 
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
 
    while ((c = getopt (argc, argv, "cH")) != -1)
        switch (c)
        {
            case 'c':
                compileOnly = true;
                break;
            case 'H':
                hexOutput = true;
                break;
            default:
                printUsage(argv[0]);
                exit(EXIT_FAILURE);
        }


}
