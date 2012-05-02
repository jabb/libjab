
all:
	gcc -Wall -Wextra -pedantic -std=c89 *.c -lm -lSDL

mac:
	gcc -Wall -Wextra -pedantic -std=c89 *.c -lm `sdl-config --cflags --libs`  -include SDL_main.h

clean:
	rm a.out

