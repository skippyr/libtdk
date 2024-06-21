VERSION:=v13.0.0
CC:=cc
CFLAGS:=-std=c99 -Wpedantic -Wall -Wextra -Wno-unused-result -Os
CPATH:=~/.local/share/include
LIBPATH:=~/.local/share/lib
MANPATH:=~/.local/share/man
SHELL:=bash

.PHONY: all clean install uninstall tools

all: build/lib/libtmk.so

clean:
	rm -rf build;

install: build/lib/libtmk.so src/tmk.h
	mkdir -p ${LIBPATH} ${CPATH} ${MANPATH}/man3;
	cp ${<} ${LIBPATH};
	cp $(word 2, ${^}) ${CPATH};

uninstall:
	rm -f ${LIBPATH}/libtmk.so ${CPATH}/tmk.h

tools: build/tools/key-dump

build/obj/tmk.o: src/tmk.c src/tmk.h
	mkdir -p build/obj;
	${CC} ${CFLAGS} -fPIC -c -o ${@} ${<};

build/lib/libtmk.so: build/obj/tmk.o
	mkdir -p build/lib;
	${CC} ${CFLAGS} -shared -o ${@} ${<};
