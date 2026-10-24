exec = "sulfur.exe"
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install: $(exec)
	mkdir -p C:/Users/SimonSJ/bin
	cp ./sulfur.exe C:/Users/SimonSJ/bin/sulfur.exe

clean:
	-rm *.exe
	-rm *.o
	-rm src/*.o