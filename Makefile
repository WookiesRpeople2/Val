# Set the compiler to gcc
CC = gcc

exec = val.out
sources = $(wildcard src/*.c)
objects = $(sources:src/%.c=src/%.o)
flags = -g


$(exec): $(objects)
	$(CC) $(objects) $(flags) -o $(exec)


src/%.o: src/%.c include/%.h
	$(CC) -c $(flags) $< -o $@

install: $(exec)
	cp ./$(exec) /usr/local/bin/hello

clean:
	-rm -f $(exec)
	-rm -f $(objects)
