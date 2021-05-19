CC = gcc
CFLAGS = -I. -O3

examples random_string: example/random_string.c install
	$(CC) example/random_string.c $(CFLAGS) -o random_string -lchadstr

install: chadstr.h chadstr.c
	$(CC) -fPIC -shared chadstr.c -o libchadstr.so $(CFLAGS)
	mv libchadstr.so /usr/lib/libchadstr.so
	chmod 755 /usr/lib/libchadstr.so
	cp chadstr.h /usr/include/chadstr.h

uninstall: /usr/include/chadstr.h
	rm /usr/lib/libchadstr.so
	rm /usr/include/chadstr.h

clean:
	rm chadstr.o | true
	rm libchadstr.so 2>/dev/null | true
	rm random_string 2>/dev/null | true

default: examples

all: install examples

