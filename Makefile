all:	t180asm OpCodeUnitTest

OBJECTS = Assembler.o TokenVector.o PseudoOpCode.o Token.o \
            SymbolTable.o Symbol.o ConstantTable.o Constant.o \
            Writer.o BinWriter.o
HEADERS = Retcode.h Assembler.h TokenVector.h OpCode.h PseudoOpCode.h Token.h \
            SymbolTable.h Symbol.h ConstantTable.h Constant.h \
            Writer.h BinWriter.h

t180asm: main.o OpCode.o  $(OBJECTS) $(HEADERS)
	gcc main.o OpCode.o $(OBJECTS) -o t180asm -lstdc++ 

OpCodeUnitTest: $(OBJECTS) OpCode.cpp
	gcc -DUNIT_TEST -o OpCodeUnitTest OpCode.cpp $(OBJECTS) -lstdc++ 

%.o:    %.cpp $(HEADERS)
	gcc -c  $< -o $@

