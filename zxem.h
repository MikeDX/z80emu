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
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdint.h>
#include <string.h>

// Platform specific
#include "osdep.h"

// macro for global vars
#ifndef GLOBALS
#define GLOBAL extern
#else
#define GLOBAL
#endif

#include "zxio.h"

#define Z80_CPU_SPEED           3500000   /* In Hz. */
#define CYCLES_PER_STEP         69888
 //(Z80_CPU_SPEED / 50)

// Spectrum colours
typedef struct _ZX_Pen {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ZX_Pen;

GLOBAL ZX_Pen colours[16];

GLOBAL uint8_t *zxmem;
GLOBAL uint8_t *cached;
GLOBAL uint8_t *screenbuf;

GLOBAL uint8_t running;
GLOBAL Z80_STATE zxcpu;
GLOBAL uint8_t debug;
// ZX VIDEO functions (zxvid.c)
extern int ZX_SetPalette(void);
extern void ZX_Draw(void);

// ZX IO functions (zxio.c)
// memory calls
#endif // include once
