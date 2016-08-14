OLEVEL = -O3 
CFLAGS = -Wall -std=gnu99 -pedantic $(OLEVEL) -fomit-frame-pointer
SDLFLAGS = $(shell sdl-config --cflags)
SDLLIBS = $(shell sdl-config --libs)
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


ifeq ($(PLAT), HTML)
CC = emcc
SDLFLAGS = -s USE_SDL=1
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
	$(CC) $(CFLAGS) $(OLEVEL) -c $< -o $@

$(OBJDIR)/zxem.o: zxem.c zxem.h z80emu.h
	$(CC) $(CFLAGS) $(OLEVEL) $(SDLFLAGS) -Wall -c $< -o $@

$(OBJDIR)/zxvid.o: zxvid.c zxem.h
	$(CC) $(CFLAGS) $(OLEVEL) $(SDLFLAGS) -Wall -c $< -o $@

$(OBJDIR)/zxio.o: zxio.c zxem.h
	$(CC) $(CFLAGS) $(OLEVEL) $(SDLFLAGS) -Wall -c $< -o $@

OBJECT_FILES = $(OBJDIR)/zxem.o $(OBJDIR)/z80emu.o $(OBJDIR)/zxvid.o $(OBJDIR)/zxio.o

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(SDLLIBS) $(OBJECT_FILES) $(LINKFLAGS) -o $@

$(TEST_TARGET): tests/cputest.c z80emu.h $(OBJDIR)/z80emu.o
	$(CC) $(CFLAGS) $< $(OBJDIR)/z80emu.o  -o $@


clean: 
	rm *.o 
	rm $(TARGET)
