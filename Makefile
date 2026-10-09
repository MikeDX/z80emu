# ZXEM MAKEFILE
#
# make               native build (SDL 1.2, z80core CPU)
# make SDL=2         build against SDL2
# make SDL=0         build with the dummy (no video) backend
# make DEBUG=1       debug build (-O0 -g)
# make PLAT=HTML     Emscripten build (zxem.html)
# make test          build and run the CPU test suite (z80emu core)

## Config

# CPU core: z80core (default), z80emu, mz80 (mz80 has issues on 64-bit)
CPU ?= z80core
SDL ?= 1
PLAT ?= NATIVE

TARGET = zxem
TEST_TARGET = tests/zxtest

CPPFLAGS += -Isrc/zxem -Isrc/osdep -Isrc/cpu
WARNINGS = -Wall -pedantic
CFLAGS += $(WARNINGS) -fomit-frame-pointer
CXXFLAGS += $(WARNINGS) -std=c++11 -fomit-frame-pointer

ifdef DEBUG
OLEVEL = -O0 -g
else
OLEVEL = -O2
endif
CFLAGS += $(OLEVEL)
CXXFLAGS += $(OLEVEL)

## Platform specific config

ifeq ($(PLAT), HTML)
CC = emcc
CXX = em++
AR = emar
OBJDIR = objhtml
SDLFLAGS = -s USE_SDL=$(SDL)
OSDLIBS =
LINKFLAGS += $(SDLFLAGS) --preload-file roms --preload-file scr --emrun
TARGET := $(TARGET).html
else
OBJDIR = obj
endif

## OS dependent (video/input) backend

ifeq ($(SDL), 1)
OSD_SOURCE = src/osdep/sdl1.2.c
ifneq ($(PLAT), HTML)
SDLFLAGS = $(shell sdl-config --cflags)
OSDLIBS = $(shell sdl-config --libs)
endif
else ifeq ($(SDL), 2)
OSD_SOURCE = src/osdep/sdl2.c
ifneq ($(PLAT), HTML)
SDLFLAGS = $(shell sdl2-config --cflags)
OSDLIBS = $(shell sdl2-config --libs)
endif
else
OSD_SOURCE = src/osdep/dummy.c
endif

## Sources: every .c/.cpp in the selected CPU directory plus src/zxem.
## Helper programs (maketables, zextest) live in src/cpu/z80emu/util so
## they are not picked up here.

CPUSRC  := $(wildcard src/cpu/$(CPU)/*.c) $(wildcard src/cpu/$(CPU)/*.cpp)
ZXEMSRC := $(wildcard src/zxem/*.c)

obj_of = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(subst /,_,$(patsubst src/%,%,$(basename $(1))))))

CPUOBJ  := $(call obj_of,$(CPUSRC))
ZXEMOBJ := $(call obj_of,$(ZXEMSRC))
OSDOBJ  := $(OBJDIR)/osdep.o
OBJECTS := $(ZXEMOBJ) $(CPUOBJ) $(OSDOBJ)

HEADERS := $(wildcard src/*/*.h src/cpu/*/*.h)

## Rules

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

define compile_c
$(call obj_of,$(1)): $(1) $(HEADERS) | $(OBJDIR)
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -c $$< -o $$@
endef
define compile_cxx
$(call obj_of,$(1)): $(1) $(HEADERS) | $(OBJDIR)
	$$(CXX) $$(CPPFLAGS) $$(CXXFLAGS) -c $$< -o $$@
endef

$(foreach src,$(filter %.c,$(CPUSRC)) $(ZXEMSRC),$(eval $(call compile_c,$(src))))
$(foreach src,$(filter %.cpp,$(CPUSRC)),$(eval $(call compile_cxx,$(src))))

$(OSDOBJ): $(OSD_SOURCE) $(HEADERS) | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SDLFLAGS) -c $< -o $@

# Link with the C++ driver so the C++ runtime is pulled in for z80core.
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(OSDLIBS) $(LINKFLAGS) -o $@

## CPU test suite (exercises the z80emu core, whichever CPU zxem uses)

$(OBJDIR)/test_z80emu.o: src/cpu/z80emu/z80emu.c $(wildcard src/cpu/z80emu/*.h) | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): tests/cputest.c src/cpu/z80emu/z80emu.h $(OBJDIR)/test_z80emu.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(OBJDIR)/test_z80emu.o -o $@

# The suite stops at the first failure. The z80emu core currently passes the
# first $(KNOWN_PASSES) tests and fails at cb46 (BIT 0,(HL): undocumented
# flag bits 3/5 come from MEMPTR, which z80emu does not model). Fail the
# build if that number goes down, so new regressions are caught.
KNOWN_PASSES ?= 286

test: $(TEST_TARGET)
	@(cd tests && ./zxtest > zxtest.log); \
	  passed=$$(grep -c PASSED tests/zxtest.log); \
	  echo "cputest: $$passed tests passed (expected at least $(KNOWN_PASSES))"; \
	  grep -B1 -A15 FAILED tests/zxtest.log | grep -v '^[ 0-9]*M[RW] ' ; \
	  test $$passed -ge $(KNOWN_PASSES)

clean:
	rm -rf obj objhtml $(TARGET) zxem.html zxem.js zxem.data $(TEST_TARGET) tests/zxtest.log

.PHONY: all clean test
