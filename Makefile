.PHONY: all clean

CFLAGS = -Wall -Wextra -std=c89 -pedantic -g -Os
LDFLAGS =

all: demo

demo: demo.c nez-m3u-parser.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f demo
