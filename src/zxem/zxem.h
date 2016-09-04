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
#include <stdint.h>
#include <string.h>

// macro for global vars
#ifndef GLOBALS
#define GLOBAL extern
#else
#define GLOBAL
#endif

// Platform specific
#include "osdep.h"

// CPU interface
#include "cpuintf.h"

// ZX IO interface
#include "zxio.h"

// ZX Video Hardware
#include "zxvid.h"

// SNA Snapshot support
#include "zxsna.h"

#define Z80_CPU_SPEED           3500000   /* In Hz. */
#define CYCLES_PER_STEP         69888
 //(Z80_CPU_SPEED / 50)


GLOBAL uint8_t *screenbuf;

GLOBAL uint8_t running;
GLOBAL uint8_t debug;
GLOBAL uint8_t CPU_Handle;

// Main entry point
int ZX_main(int argc, char *argv[]);

#endif // include once
