/* zxem.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 */

#define GLOBALS
#include "zxem.h"

// Include SDL Main header

Z80_STATE zxcpu;
double          total;
int stopped;


int ZX_LoadROM(void) {
	/* Load ZX Spectrum ROM into lower 16K mem */
	FILE *f;

	if(!(f=fopen("roms/48k.rom","r"))) {
		printf("Failed to open ROM\n");
		return -1;
	}

	fread(zxmem,1,16384,f);

	return 1;
}

void ZX_End(void) {
	free(zxmem);
	SDL_Quit();
}


uint8_t readbyte(uint16_t addr) {
    return zxmem[addr];
}

uint16_t readword(uint16_t addr) {
    return zxmem[addr] | zxmem[addr+1]<<8;
}

void writebyte(uint16_t addr, uint8_t data) {

	/* Don't allow writing to ROM */
	if(addr>=0x4000) {
	    zxmem[addr]=data;
	}
}

void input(Z80_STATE *state) {
	// handle IN()
}

void writeword(uint16_t addr, uint16_t data) {
	if(addr>=0x4000) {
	    zxmem[addr]=data;
	    zxmem[addr+1]=data>>8;
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	/* Create 8 bit buffer */
	/* Spectrum screen is 32*8 by 24*8 */
	screen = SDL_SetVideoMode(256, 192, 8, SDL_SWSURFACE);
	zxmem = (uint8_t *)malloc(65536);
	if(!zxmem) {
		printf("Failed to allocate RAM\n");
		ZX_End();
	}

	memset(zxmem,0,65535);

	if(!ZX_LoadROM()) {
		ZX_End();
	}

	ZX_SetPalette();

	zxcpu.memory = zxmem;
	zxcpu.readbyte = readbyte;
	zxcpu.readword = readword;
	zxcpu.writeword = writeword;
	zxcpu.writebyte = writebyte;
	zxcpu.input = input;
	total = 0;

	for (;;) {
		total += Z80Emulate(&zxcpu, CYCLES_PER_STEP);
		ZX_Draw();
	}

	ZX_End();
	return 0;
}

