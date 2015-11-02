CC = clang++
CFLAGS = -g -I/opt/local/include -std=c++11
LDLIBS = /opt/local/lib/libboost_system-mt.dylib

all:
	$(CC) main.cpp Commando.cpp $(LDLIBS) $(CFLAGS) -o Commando

test:
	./Commando 127.0.0.1 31400 31401

clean:
	rm -r ./Commando.dSYM ./Commando
