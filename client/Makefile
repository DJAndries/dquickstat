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

install: bin/dquickstat-client
	useradd -M -s /usr/bin/nologin dquickstat || true
	usermod -L dquickstat
	install -m 755 ./bin/dquickstat-client /usr/local/bin/
	install -m 644 ./src/dquickstat-client.service /etc/systemd/system
	systemctl enable dquickstat-client
	systemctl start dquickstat-client
