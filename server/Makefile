SRC = $(wildcard src/*.c)
SRC_OBJ = $(SRC:src/%.c=obj/%.o)

FADD_FLAGS = $(ADD_FLAGS) -Wl,-rpath='$$ORIGIN'

all: obj bin bin/dquickstat-server

bin/dquickstat-server: $(SRC_OBJ) bin/libfbmagic.so
	gcc $(FADD_FLAGS) -o $@ $(SRC_OBJ) -L./bin -l:libfbmagic.so

bin/libfbmagic.so:
	cp ../../fbmagic/lib/libfbmagic.so $@

obj/%.o: src/%.c
	gcc $(FADD_FLAGS) -I ../../fbmagic/include -I ../shared/include -Wall -O -c $< -o $@

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm -f obj/* bin/*
