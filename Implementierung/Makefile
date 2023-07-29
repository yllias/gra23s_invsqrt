# Add additional compiler flags here
CC = gcc
CFLAGS = -O2 -std=c17 -Wall -Wextra -g -lm -fsanitize=address -fsanitize=undefined #-ffast-math #-march=native

.PHONY: clean

all: main
main: src/main.c src/inverse_sqrt.c src/parser.c src/tests.c src/magicnumber.c
	  $(CC) $(CFLAGS) -o $@ $^ -lm 

clean:
	  rm -f main
