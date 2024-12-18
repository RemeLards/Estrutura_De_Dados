CC = gcc

SRC = main.c vector.c stack.c
OUT = vector_stack

#CHARACTER_HEADERS_PATH = ../../include/character
#SYSTEM_HEADERS_PATH = ../../include/system
#MAP_HEADERS_PATH = ../../include/map

IFLAGS = #-I$(CHARACTER_HEADERS_PATH) -I$(SYSTEM_HEADERS_PATH) -I$(MAP_HEADERS_PATH)

CFLAGS = -pedantic -Wall -Wextra -Wwrite-strings -g3

all :
	$(CC) $(SRC) -o $(OUT) $(IFLAGS) $(CFLAGS)
#	move character_functions.o ..\..\bin\ 
	

clean :
	rm *.o