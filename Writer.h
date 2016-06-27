
#pragma once

#include "Retcode.h"

class Assembler;

class Writer
{

public:
    Writer();
    virtual ~Writer();

    virtual AssemblerResult write(const char* ofile, Assembler& assembler) = 0;


};

