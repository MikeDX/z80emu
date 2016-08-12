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

// macro for global vars
#ifndef GLOBALS
#define GLOBAL extern
#else
#define GLOBAL
#endif

#define Z80_CPU_SPEED           4000000   /* In Hz. */
#define CYCLES_PER_STEP         (Z80_CPU_SPEED / 50)

GLOBAL uint8_t *zxmem;

#endif
