# ZXEM MAKEFILE
OLEVEL = -O3 
CFLAGS =  -Isrc/zxem -Isrc/osdep -Isrc/cpu -Wall -pedantic $(OLEVEL) -fomit-frame-pointer
SDL = 1
CC = g++
CPU = z80emu
#CPU = mz80

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
OSDFLAGS = $(shell sdl2-config --cflags)
OSDLIBS = $(shell sdl2-config --libs)
endif
endif


ifeq ($(PLAT), HTML)
CC = emcc
OSDFLAGS = -s USE_SDL=$(SDL)
OSDLIBS = 
OLEVEL += -s ASM_JS=1 
LINKFLAGS = $(OSDFLAGS) $(OLEVEL) --preload-files roms/ --preload-files scr/ --emrun
TARGET := $(TARGET).html
OBJDIR = objhtml
endif

ifeq ($(CPU), z80emu)
CPUOBJ = $(OBJDIR)/z80emu.o
CPUINTC = src/cpu/z80emu/cpuintf.c
endif

ifeq ($(CPU), mz80)
CPUOBJ = $(OBJDIR)/mz80.o
CPUINTC = src/cpu/mz80/cpuintf.c
endif


all: $(OBJDIR) $(TARGET) 

$(OBJDIR):
	mkdir $(OBJDIR)

tables.h: maketables.c
	$(CC) -Wall $< -o maketables
	./maketables > $@

$(OBJDIR)/z80emu.o: src/cpu/z80emu/z80emu.c src/cpu/z80emu/z80emu.h src/cpu/z80emu/instructions.h src/cpu/z80emu/macros.h src/cpu/z80emu/tables.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mz80.o: src/cpu/mz80/mz80.c src/cpu/mz80/mz80.h src/cpu/cpuintf.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/zxem.o: src/zxem/zxem.c src/zxem/zxem.h src/osdep/osdep.h
	$(CC) $(CFLAGS) $(OSDFLAGS) -c $< -o $@

$(OBJDIR)/zxvid.o: src/zxem/zxvid.c src/zxem/zxem.h src/osdep/osdep.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/zxio.o: src/zxem/zxio.c src/zxem/zxem.h src/osdep/osdep.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/osdep.o: $(OSD_SOURCE) src/zxem/zxem.h src/osdep/osdep.h
	$(CC) $(CFLAGS) $(OSDFLAGS) -c $(OSD_SOURCE) -o $@

$(OBJDIR)/cpuintf.o: $(CPUINTC) 
	$(CC) $(CFLAGS) -c $< -o $@

OBJECT_FILES = $(OBJDIR)/zxem.o $(CPUOBJ) $(OBJDIR)/zxvid.o $(OBJDIR)/zxio.o $(OBJDIR)/osdep.o $(OBJDIR)/cpuintf.o

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(OSDLIBS) $(LINKFLAGS) -o $@

$(TEST_TARGET): tests/cputest.c z80emu.h $(OBJDIR)/z80emu.o
	$(CC) $(CFLAGS) $< $(OBJDIR)/z80emu.o  -o $@


clean: 
	rm $(OBJDIR)/*.o 
	rm $(TARGET)
