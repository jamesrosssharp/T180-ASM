
#include "Writer.h"

class BinWriter  : 
        public Writer
{


public:
    BinWriter();
    ~BinWriter();

    AssemblerResult write(const char* ofile, Assembler& assembler);

};

