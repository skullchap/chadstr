CC = gcc
CFLAGS = -I. -O3

examples random_string: example/random_string.c
	$(CC) example/random_string.c $(CFLAGS) -o random_string

install: chadstr.h
	cp chadstr.h /usr/include/chadstr.h

uninstall:
	rm /usr/include/chadstr.h | true
	rm /usr/lib/libchadstr.so | true

clean: random_string
	rm random_string

shared_lib: dummy.c chadstr.h install
	$(CC) dummy.c -fPIC -shared -o libchadstr.so $(CFLAGS)
	mv libchadstr.so /usr/lib/libchadstr.so
	chmod 755 /usr/lib/libchadstr.so

default: examples

all: examples install

