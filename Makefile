CC=gcc
CFLAGS=-Ofast -Wall -Wextra -std=c99 -pedantic
CFLAGS += -Wno-missing-braces -Wno-strict-aliasing -Wno-unused-variable # To make GCC shut up about olive.c
CFLAGS += $(shell sdl-config --cflags)

LDFLAGS = -flto
LDFLAGS += $(shell sdl-config --libs)

all: japan

japan:
	$(CC) $(CFLAGS) japan.c -o japan $(LDFLAGS)

clean:
	rm -fv japan
