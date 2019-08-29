exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -fPIC


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

libjson.a: $(objects)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	make libjson.a
	mkdir -p /usr/local/include/json
	cp -r ./src/include/* /usr/local/include/json/.
	cp ./libjson.a /usr/local/lib/.
	cp ./json.out /usr/local/bin/json

clean:
	-rm *.out
	-rm *.o
	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c src/include/*.h
