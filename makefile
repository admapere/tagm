CC=gcc -std=gnu11 -Wall
OUT=-o tagm.o
FILES=./src/tagm.c
LIB=-lid3v2 -lncurses

syntax: $(FILES)
	$(CC) -fsyntax-only $(OUT) $(FILES) $(LIB)
debug: $(FILES)
	$(CC) -g $(OUT) $(FILES) $(LIB)
build: $(FILES)
	$(CC) $(OUT) $(FILES) $(LIB)
