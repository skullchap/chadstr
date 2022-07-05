CC = gcc
CFLAGS = -I. -O3 -Werror -Wpedantic

examples random_string: example/random_string.c
	$(CC) example/random_string.c $(CFLAGS) -o random_string

install: chadstr.h
	cp chadstr.h $(DESTDIR)/usr/include/chadstr.h
	chmod 755 $(DESTDIR)/usr/include/chadstr.h

uninstall: /usr/include/chadstr.h
	rm $(DESTDIR)/usr/include/chadstr.h

clean: random_string
	rm random_string

shared: chadstr.h
	echo '#include "chadstr.h"' > dummy.c
	gcc dummy.c -fPIC -shared -o libchadstr.so -O3 -I.
	rm dummy.c
	mv libchadstr.so $(DESTDIR)/usr/lib/libchadstr.so
	chmod 755 $(DESTDIR)/usr/lib/libchadstr.so

default: examples

all: examples install

