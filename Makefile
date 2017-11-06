CC=gcc


all: minesweeper

minesweeper:
	$(CC) -o minesweeper minesweeper.c

debug:
	$(CC) -o minesweeper -D DEBUG minesweeper.c

clean:
	rm minesweeper
