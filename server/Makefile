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

install: bin/dquickstat-server
	useradd -G video -M -s /usr/bin/nologin dquickstat || true
	usermod -L dquickstat
	install -m 755 -d /usr/local/share/dquickstat/res
	install -m 644 ./res/* /usr/local/share/dquickstat/res/
	install -m 644 ./bin/libfbmagic.so /usr/lib/
	install -m 755 ./bin/dquickstat-server /usr/local/bin/
	install -m 644 ./src/dquickstat-server-prep.service /etc/systemd/system
	install -m 644 ./src/dquickstat-server.service /etc/systemd/system
	systemctl enable dquickstat-server-prep
	systemctl start dquickstat-server-prep
	systemctl enable dquickstat-server
	systemctl start dquickstat-server
