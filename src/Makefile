COMPILER=gcc
OPTIONS=-g -std=c11 -pedantic -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code -pthread -lopenal -lsndfile
COMPILE=$(COMPILER) $(OPTIONS)
BUILD=../bin

all: main

main: *.c
	$(COMPILE) $^ -o $(BUILD)/$@

clean:
	rm -f $(BUILD)/*
