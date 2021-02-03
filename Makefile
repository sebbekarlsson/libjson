exec = a.out
sources = $(filter-out src/main.c, $(wildcard src/*.c))
objects = $(sources:.c=.o)
flags = -g -fPIC
LPATH = $(HOME)/.local


#$(exec): $(objects)
#	gcc $(objects) $(flags) -o $(exec)

libjson.a: $(objects)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	make libjson.a
	mkdir -p $(LPATH)/include/json
	cp -r ./src/include/* $(LPATH)/include/json/.
	cp ./libjson.a $(LPATH)/lib/.
	cp ./json.out $(LPATH)/bin/json

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c src/include/*.h
