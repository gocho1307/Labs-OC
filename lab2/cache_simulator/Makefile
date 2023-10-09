#       Makefile for lab2
#
# Authors: Gonçalo Bárias (ist1103124)
#          Miguel Costa (ist1103969)
#          Raquel Braunschweig (ist1102624)

CC = gcc
CFLAGS = -Wall -Wextra

SOURCES := $(shell find . -name "*.c")
HEADERS := $(shell find . -name "*.h")
OBJECTS := $(SOURCES:.c=.o)
TARGETS := $(patsubst %.c,%,$(wildcard tests/*.c))

.PHONY: all clean fmt test

all:
	$(CC) $(CFLAGS) tests/SimpleProgram.c src/3/L2Cache_two_way.c -o tests/SimpleProgram
	$(CC) $(CFLAGS) tests/SimpleProgram_L1.c src/1/L1Cache.c -o tests/SimpleProgram_L1
	$(CC) $(CFLAGS) tests/SimpleProgram_L2.c src/2/L2Cache.c -o tests/SimpleProgram_L2
	$(CC) $(CFLAGS) tests/Test0_L2_2W.c src/3/L2Cache_two_way.c -o tests/Test0_L2_2W
	$(CC) $(CFLAGS) tests/Test1_L2_2W.c src/3/L2Cache_two_way.c -o tests/Test1_L2_2W

clean:
	rm -f $(OBJECTS) $(TARGETS)

fmt: $(SOURCES) $(HEADERS)
	clang-format -i $^
