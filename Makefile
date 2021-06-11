CFLAGS = -Wall -Wextra -Wpedantic -Werror -Ofast 
CC = clang
TARGET = brainfuck

all: brainfuck

brainfuck: ll.o stack.o map.o instruction_buffer.o tape.o
	$(CC) $(CFLAGS) brainfuck.c -o $(TARGET) *.o

clean:
	rm -f *.o $(TARGET)

scan-build: clean
	scan-build --use-cc=$(CC) make 
