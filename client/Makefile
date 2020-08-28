SRC = $(wildcard src/*.c)
SRC_OBJ = $(SRC:src/%.c=obj/%.o)

all: obj bin bin/dquickstat-client

bin/dquickstat-client: $(SRC_OBJ)
	gcc $(ADD_FLAGS) -o $@ $(SRC_OBJ)

obj/%.o: src/%.c
	gcc $(ADD_FLAGS) -I ../shared/include -Wall -O -c $< -o $@

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm -f obj/* bin/*
