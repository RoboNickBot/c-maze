c-maze: src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/directionals.c src/maze_gen.c src/internals.h src/IOStuff.h src/mapper.h src/directionals.h src/maze_gen.h
	gcc -I /usr/include/SDL2 -I /usr/include/SDL2_image -lSDL2 -lSDL2_image -o c-maze src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/directionals.c src/maze_gen.c

gen: src/gen_test.c src/mapper.c src/maze_gen.c src/internals.c src/directionals.c src/IOStuff.c src/mapper.h src/maze_gen.h src/internals.h src/directionals.h src/IOStuff.h src/image_key.h
	gcc -I /usr/include/SDL2 -I /usr/include/SDL2_image -lSDL2 -lSDL2_image -o gen src/gen_test.c src/mapper.c src/maze_gen.c src/internals.c src/directionals.c src/IOStuff.c

windows: src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/directionals.c src/maze_gen.c src/internals.h src/IOStuff.h src/mapper.h src/directionals.h src/maze_gen.h
	x86_64-w64-mingw32-gcc -I ./include/SDL2 -L ./lib -o c-maze.exe src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/directionals.c src/maze_gen.c -lSDL2 -lSDL2_image
