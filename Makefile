OBJECTS = main.o

t180asm:    $(OBJECTS)
	gcc $(OBJECTS) -o t180asm -lstdc++

%.o:    %.cpp
	gcc -c  $< -o $@

