CC=gcc
CFLAGS=-O3 -Wall -Wextra -std=c99 -pedantic # Ofast was causing segfaults
CFLAGS += -Wno-missing-braces -Wno-strict-aliasing -Wno-unused-variable # To make GCC shut up about olive.c
CFLAGS += $(shell sdl-config --cflags)

LDFLAGS = -flto
LDFLAGS += $(shell sdl-config --libs)

all: japan continuous

japan:
	$(CC) $(CFLAGS) japan.c -o japan $(LDFLAGS)

continuous:
	$(CC) $(CFLAGS) continuous.c -o continuous $(LDFLAGS)

clean:
	rm -fv japan continuous
