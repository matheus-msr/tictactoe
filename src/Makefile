FILES=main.c error.c graphics.c mechanics.c input.c network.c
CC=gcc
CFLAGS=-Wall -Wextra
CLIBS=-I/usr/include/SDL2 -D_REENTRANT -L/usr/lib/i386-linux-gnu -lSDL2 -lSDL2_ttf -lSDL2_image
ELFNAME=tictactoe.elf

all: compile

compile:
	$(CC) $(FILES) -o $(ELFNAME) $(CFLAGS) $(CLIBS)
	mv $(ELFNAME) ..

clean:
	rm ../$(ELFNAME)
