OLEVEL = -O3 
CFLAGS = -I. -Wall -std=gnu99 -pedantic $(OLEVEL) -fomit-frame-pointer
SDL = 1
CC = gcc
TARGET = zxem
TEST_TARGET = tests/zxtest
OBJDIR = obj
ifndef PLAT
PLAT = NATIVE
endif

ifdef DEBUG
OLEVEL = -O0 -g
endif

OSD_SOURCE = src/osdep/dummy.c

ifdef SDL
ifeq ($(SDL), 1)
OSD_SOURCE = src/osdep/sdl1.2.c
OSDFLAGS = $(shell sdl-config --cflags)
OSDLIBS = $(shell sdl-config --libs)
endif
ifeq ($(SDL), 2)
OSD_SOURCE = src/osdep/sdl2.c
SDLFLAGS = $(shell sdl2-config --cflags)
SDLLIBS = $(shell sdl2-config --libs)
endif
endif


ifeq ($(PLAT), HTML)
CC = emcc
SDLFLAGS = -s USE_SDL=$(SDL)
SDLLIBS = -lSDL
OLEVEL += -s ASM_JS=1 
LINKFLAGS = $(OLEVEL) --preload-files roms/ --preload-files scr/ --emrun
TARGET := $(TARGET).html
OBJDIR = objhtml
endif

all: $(OBJDIR) $(TARGET) $(TEST_TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

tables.h: maketables.c
	$(CC) -Wall $< -o maketables
	./maketables > $@

$(OBJDIR)/z80emu.o: z80emu.c z80emu.h instructions.h macros.h tables.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/zxem.o: zxem.c zxem.h z80emu.h osdep.h
	$(CC) $(CFLAGS) $(OSDFLAGS) -c $< -o $@

$(OBJDIR)/zxvid.o: zxvid.c zxem.h osdep.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/zxio.o: zxio.c zxem.h osdep.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/osdep.o: $(OSD_SOURCE) zxem.h osdep.h
	$(CC) $(CFLAGS) $(OSDFLAGS) -c $(OSD_SOURCE) -o $@

OBJECT_FILES = $(OBJDIR)/zxem.o $(OBJDIR)/z80emu.o $(OBJDIR)/zxvid.o $(OBJDIR)/zxio.o $(OBJDIR)/osdep.o

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(OSDLIBS) $(LINKFLAGS) -o $@

$(TEST_TARGET): tests/cputest.c z80emu.h $(OBJDIR)/z80emu.o
	$(CC) $(CFLAGS) $< $(OBJDIR)/z80emu.o  -o $@


clean: 
	rm $(OBJDIR)/*.o 
	rm $(TARGET)
