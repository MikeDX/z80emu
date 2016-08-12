OLEVEL = -O3 
CFLAGS = -Wall -std=gnu99 -pedantic $(OLEVEL) -fomit-frame-pointer
SDLFLAGS = $(shell sdl-config --cflags)
SDLLIBS = $(shell sdl-config --libs)
CC = gcc
TARGET = zxem
OBJDIR = obj
ifndef PLAT
PLAT = NATIVE
endif

ifdef DEBUG
OLEVEL = -O0 -g
endif


ifeq ($(PLAT), HTML)
CC = emcc
SDLFLAGS = -s USE_SDL=1
OLEVEL += -s ASM_JS=1 
LINKFLAGS = $(OLEVEL) --preload-files roms/ --emrun
TARGET := $(TARGET).html
OBJDIR = objhtml
endif

ifeq ($(PLAT), JS)
CC = emcc
SDLFLAGS = -s USE_SDL=1
OLEVEL += -s ASM_JS=1 
LINKFLAGS = $(OLEVEL) --embed-files roms/ --emrun
TARGET := $(TARGET).js
OBJDIR = objjs
endif


all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

tables.h: maketables.c
	$(CC) -Wall $< -o maketables
	./maketables > $@

$(OBJDIR)/z80emu.o: z80emu.c z80emu.h instructions.h macros.h tables.h
	$(CC) $(CFLAGS) $(OLEVEL) -c $< -o $@

$(OBJDIR)/zxem.o: zxem.c zxem.h z80emu.h
	$(CC) $(CFLAGS) $(OLEVEL) $(SDLFLAGS) -Wall -c $< -o $@

OBJECT_FILES = $(OBJDIR)/zxem.o $(OBJDIR)/z80emu.o

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(SDLLIBS) $(OBJECT_FILES) $(LINKFLAGS) -o $@

clean: 
	rm *.o 
	rm $(TARGET)
