/* zxem.h
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 */

#ifndef _ZXEM_H_
#define _ZXEM_H_
#include <stdio.h>
#include <stdlib.h>
#include "z80emu.h"
#include <SDL.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdint.h>
#include <string.h>
// macro for global vars
#ifndef GLOBALS
#define GLOBAL extern
#else
#define GLOBAL
#endif

#define Z80_CPU_SPEED           4000000   /* In Hz. */
#define CYCLES_PER_STEP         (Z80_CPU_SPEED / 50)

GLOBAL uint8_t *zxmem;
GLOBAL uint8_t *cached;
GLOBAL uint8_t *screenbuf;

GLOBAL SDL_Surface *screen;
GLOBAL uint8_t running;
GLOBAL Z80_STATE zxcpu;
GLOBAL uint8_t debug;
// ZX VIDEO functions (zxvid.c)
extern int ZX_SetPalette(void);
extern void ZX_Draw(void);

// ZX IO functions (zxio.c)
// memory calls
extern uint8_t readbyte(uint16_t addr);
extern uint16_t readword(uint16_t addr);
extern void writebyte(uint16_t addr, uint8_t data);
extern void writeword(uint16_t addr, uint16_t data);
// IN call
extern uint8_t input(uint16_t port);

// OUT call
extern void output(uint16_t port, uint8_t data);

// User input routine
extern void ZX_Input(void);
#endif
