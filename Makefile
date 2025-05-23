P=kofte
CFLAGS = -Wall -O3 -gdwarf-4
LDLIBS =
CC = gcc
OBJECTS =

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

build:
	$(CC) -o $(P) $(CFLAGS) $(P).c

clean:
	rm -f $(P)

install: build
	install -D -m 755 -s $(P) $(BINDIR)/$(P)

uninstall:
	rm -rf $(BINDIR)/$(P)
