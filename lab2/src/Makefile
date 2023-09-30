#       Makefile for lab2
#
# Authors: Gonçalo Bárias (ist1103124)
#          Miguel Costa (ist1103969)
#          Raquel Braunschweig (ist1102624)

CC = gcc
CFLAGS = -Wall -Wextra

SOURCES  := $(wildcard */*.c)
HEADERS  := $(wildcard */*.h)
OBJECTS  := $(SOURCES:.c=.o)
TARGETS := $(patsubst %.c,%,$(wildcard tests/*.c))

.PHONY: all clean fmt test

all:
	$(CC) $(CFLAGS) tests/SimpleProgram.c 1/L1Cache.c -o $(TARGETS)

clean:
	rm -f $(OBJECTS) $(TARGETS)

fmt: $(SOURCES) $(HEADERS)
	clang-format -i $^
