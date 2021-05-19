CC = gcc
CFLAGS = -I. -O3

examples random_string: example/random_string.c
	$(CC) example/random_string.c $(CFLAGS) -o random_string

install: chadstr.h
	cp chadstr.h /usr/include/chadstr.h

uninstall: /usr/lib/libchadstr.so
	rm /usr/lib/libchadstr.so

clean: random_string
	rm random_string

default: examples

all: examples install

