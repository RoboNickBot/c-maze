c-maze: gametest1.c internals.c IOStuff.c mapper.c internals.h IOStuff.h mapper.h
	gcc -I /usr/include/SDL2 -I /usr/include/SDL2_image -lSDL2 -lSDL2_image -o c-maze gametest1.c internals.c IOStuff.c mapper.c
