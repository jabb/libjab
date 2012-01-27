
all:
	gcc -Wall -Wextra -pedantic -std=c89 *.c -lSDL

clean:
	rm a.out
	
