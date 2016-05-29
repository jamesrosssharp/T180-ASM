OBJECTS = main.o Assembler.o TokenVector.o OpCode.o PseudoOpCode.o Token.o
HEADERS = Retcode.h Assembler.h TokenVector.h OpCode.h PseudoOpCode.h Token.h

t180asm:    $(OBJECTS) $(HEADERS)
	gcc $(OBJECTS) -o t180asm -lstdc++ 

%.o:    %.cpp $(HEADERS)
	gcc -c  $< -o $@

