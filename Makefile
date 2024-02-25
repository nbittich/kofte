P=kofte
CFLAGS = -Wall -O3 -gdwarf-4
LDLIBS =
CC = clang
OBJECTS =

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

all:
	$(CC) -o $(P) $(CFLAGS) $(P).c

clean:
	rm -f $(P)

install: all
	install -D -m 755 -s $(P) $(BINDIR)/$(P)

uninstall:
	rm -rf $(BINDIR)/$(P)
