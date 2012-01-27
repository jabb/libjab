
all:
	gcc -Wall -Wextra -pedantic -std=c89 *.c -lSDL

mac:
	gcc -Wall -Wextra -pedantic -std=c89 *.c `sdl-config --cflags --libs`  -include SDL_main.h

clean:
	rm a.out
	
