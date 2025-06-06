CC=gcc
CFLAGS=-Iinclude -Wall -g

all: demo

demo: src/text_utils.c examples/demo.c
	$(CC) $(CFLAGS) -o demo examples/demo.c src/text_utils.c

clean:
	rm -f demo
