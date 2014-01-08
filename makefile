c-maze: src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/directionals.c src/internals.h src/IOStuff.h src/mapper.h src/directionals.h
	gcc -I /usr/include/SDL2 -I /usr/include/SDL2_image -lSDL2 -lSDL2_image -o c-maze src/gametest1.c src/internals.c src/IOStuff.c src/mapper.c src/directionals.c

maze_gen_test: src/gen_test.c src/mapper.c src/maze_gen.c src/internals.c src/directionals.c
	gcc -o maze_gen_test src/gen_test.c src/mapper.c src/maze_gen.c src/internals.c src/directionals.c
