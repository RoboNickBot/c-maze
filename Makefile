# Makefile for cmaze

VPATH = src
BIN = bin
BUILD = build
DOC = doc
RES = res
CC = gcc -DTESTMACRO=TESTMACRA

# setting the "USRPREFIX" sends the installation dir into the
# source-files so that they can find data files like the game images
COMPILE = $(CC) \
  -DUSRPREFIX=\"$(out)\" \
  -c $(INCLUDES)

LINK = $(CC) $(LOADS)

INCLUDES = \
  -I$(SDL2)/include \
  -I$(SDL2_image)/include
LOADS = \
  -L$(SDL2)/lib       -lSDL2 \
  -L$(SDL2_image)/lib -lSDL2_image

GENERAL.o = \
  $(BUILD)/internals.o \
  $(BUILD)/IOStuff.o \
  $(BUILD)/mapper.o \
  $(BUILD)/directionals.o \
  $(BUILD)/maze_gen.o
CMAZE.o = \
  $(BUILD)/gametest1.o
CMAZETEST.o = \
  $(BUILD)/gen_test.o

OBJS = $(CMAZE.o) $(CMAZETEST.o) $(GENERAL.o)

CMAZE = $(BIN)/cmaze-game
CMAZETEST = $(BIN)/cmaze-mazegen

PRODUCTS = $(CMAZE) $(CMAZETEST)

REBUILDABLES = $(BUILD) $(BIN)

all : $(BUILD) $(BIN) $(PRODUCTS)

install :
	install -vD $(BIN)/* -t $(out)/usr/bin/
	install -vD $(RES)/* -t $(out)/usr/share/c-maze/res/
	install -vD $(DOC)/* -t $(out)/usr/share/c-maze/doc/

clean :
	rm -vr $(REBUILDABLES)
	echo All clean

$(CMAZE) : $(CMAZE.o) $(GENERAL.o)
	$(LINK) -o $@ $^

$(CMAZETEST) : $(CMAZETEST.o) $(GENERAL.o)
	$(LINK) -o $@ $^

$(BUILD)/%.o : %.c
	$(COMPILE) -o $@ $<

$(BIN) :
	mkdir $(BIN)

$(BUILD) :
	mkdir $(BUILD)

# Dependency rules
gametest1.o : internals.h directionals.h IOStuff.h mapper.h maze_gen.h
gen_test.o : maze_gen.h internals.h mapper.h image_key.h IOStuff.h

internals.o : internals.h directionals.h
directionals.o : directionals.h
mapper.o : mapper.h internals.h directionals.h
maze_gen.o : maze_gen.h internals.h directionals.h
IOStuff.o : IOStuff.h internals.h directionals.h image_key.h
