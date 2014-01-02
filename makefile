c-maze: src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/internals.h src/IOStuff.h src/mapper.h
	gcc -I /usr/include/SDL2 -I /usr/include/SDL2_image -lSDL2 -lSDL2_image -o src/c-maze src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c
