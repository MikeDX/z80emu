OLEVEL = -O3 
CFLAGS = -Wall -ansi -pedantic $(OLEVEL) -fomit-frame-pointer
CC = gcc
TARGET = zextest
OBJDIR = obj
ifndef PLAT
PLAT = NATIVE
endif

ifdef DEBUG
OLEVEL = -O0 -g
endif


ifeq ($(PLAT), HTML)
CC = emcc
OLEVEL += -s ASM_JS=1 
LINKFLAGS = $(OLEVEL) --preload-files testfiles/ --emrun
TARGET = zextest.html
OBJDIR = objhtml
endif

ifeq ($(PLAT), JS)
CC = emcc
OLEVEL += -s ASM_JS=1 
LINKFLAGS = $(OLEVEL) --embed-files testfiles/ --emrun
TARGET = zextest.js
OBJDIR = objjs
endif


all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)

tables.h: maketables.c
	$(CC) -Wall $< -o maketables
	./maketables > $@

$(OBJDIR)/z80emu.o: z80emu.c z80emu.h instructions.h macros.h tables.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/zextest.o: zextest.c z80emu.h
	$(CC) $(CFLAGS) -Wall -c $< -o $@

OBJECT_FILES = $(OBJDIR)/zextest.o $(OBJDIR)/z80emu.o

$(TARGET): $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LINKFLAGS) -o $@

clean: 
	rm *.o 
	rm $(TARGET)
