# ZXEM MAKEFILE

## config
CFLAGS =  -Isrc/zxem -Isrc/osdep -Isrc/cpu -Wall -Wextra -pedantic -fomit-frame-pointer
CXXFLAGS = -fpermissive

ifdef DEBUG
CFLAGS += -g -O0
else
CFLAGS += -O2
endif

# MULTI CPU CORE!
#CPU = z80emu

# MZ80 - Issues on 64bit :(
# MZ80 BROKEN!
#CPU = mz80

# Z80CORE
CPU = z80core

TARGET = zxem
TEST_TARGET = tests/zxtest


## Platform specific config

PLAT ?= NATIVE
ifeq ($(PLAT), NATIVE)
CC ?= gcc
CXX ?= g++
OBJDIR = obj
endif
ifeq ($(PLAT), HTML)
CXX = em++
CC = emcc
OBJDIR = objhtml

AR = emar
CFLAGS += -s ASM_JS=1 -s USE_SDL=$(SDL)
LINKFLAGS += --preload-file ./roms --emrun
OSDLIBS = 
TARGET := $(TARGET).html
endif


## make Rules

all: $(OBJDIR) $(TARGET)

clean: 
	rm -rf $(OBJDIR)/* $(TARGET) $(TEST_TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)


### SDL

SDL ?= 1
ifdef SDL
ifeq ($(SDL), 1)
OSDLIBS ?= $(shell sdl-config --libs)
$(OBJDIR)/osdep.o: src/osdep/sdl1.2.c
	$(CC) $(CFLAGS) $(shell sdl-config --cflags) -c $< -o $@
endif
ifeq ($(SDL), 2)
OSDLIBS ?= $(shell sdl2-config --libs)
$(OBJDIR)/osdep.o: src/osdep/sdl2.c
	$(CC) $(CFLAGS) $(shell sdl2-config --cflags) -c $< -o $@
endif
else
$(OBJDIR)/osdep.o: src/osdep/dummy.c
	$(CC) $(CFLAGS) -c $< -o $@
endif

CPUSRC := $(wildcard ./src/cpu/$(CPU)/*.c) $(wildcard ./src/cpu/$(CPU)/*.cpp)
CPUOBJ := $(CPUSRC:.cpp=.o)
CPUOBJ := $(CPUOBJ:.c=.o)

ZXEMSRC := $(wildcard ./src/zxem/*.c)
ZXEMOBJ := $(ZXEMSRC:.c=.o)

$(TARGET): $(OBJDIR)/zxem.a($(ZXEMOBJ)) $(OBJDIR)/$(CPU).a($(CPUOBJ)) $(OBJDIR)/osdep.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) $^ $(OSDLIBS) $(LINKFLAGS) -o $@

#TODO test is z80emu specific
$(TEST_TARGET): tests/cputest.c src/cpu/z80emu/z80emu.h $(OBJDIR)/z80emu.o
	$(CC) $(CFLAGS) $< $(OBJDIR)/z80emu.o  -o $@

.PHONY: clean all
