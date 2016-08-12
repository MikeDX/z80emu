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
#include <SDL.h>

SDL_Surface *screen;

Z80_STATE zxcpu;
double          total;
int stopped;
SDL_Color colours[16];

int ZX_SetPalette(void) {

/* Normal Colours */
	colours[0].r=0;
	colours[0].g=0;
	colours[0].b=0;

	colours[1].r=0;
	colours[1].g=0;
	colours[1].b=0xD7;
	
	colours[2].r=0xD7;
	colours[2].g=0;
	colours[2].b=0;

	colours[3].r=0xD7;
	colours[3].g=0;
	colours[3].b=0xD7;

	colours[4].r=0;
	colours[4].g=0xD7;
	colours[4].b=0;

	colours[5].r=0;
	colours[5].g=0xD7;
	colours[5].b=0xD7;

	colours[6].r=0xD7;
	colours[6].g=0xD7;
	colours[6].b=0;

	colours[7].r=0xD7;
	colours[7].g=0xD7;
	colours[7].b=0xD7;

/* Bright Colours */
	colours[8].r=0;
	colours[8].g=0;
	colours[8].b=0;

	colours[9].r=0;
	colours[9].g=0;
	colours[9].b=0xFF;
	
	colours[10].r=0xFF;
	colours[10].g=0;
	colours[10].b=0;

	colours[11].r=0xFF;
	colours[11].g=0;
	colours[11].b=0xFF;

	colours[12].r=0;
	colours[12].g=0xFF;
	colours[12].b=0;

	colours[13].r=0;
	colours[13].g=0xFF;
	colours[13].b=0xFF;

	colours[14].r=0xFF;
	colours[14].g=0xFF;
	colours[14].b=0;

	colours[15].r=0xFF;
	colours[15].g=0xFF;
	colours[15].b=0xFF;

	SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL,colours,0,16);
	
	return 1;

}
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

void ZX_Draw(void) {
	/* Draw the screen */
	int x=0,y=0;
	int i=0;
	int j=0;
	uint16_t bufaddr = 0;
	uint8_t yp=0;
	SDL_LockSurface(screen);
	uint8_t *pixels = screen->pixels;

	uint8_t ink = 0;
	uint8_t paper = 0;
	uint8_t flash = 0;
	uint8_t bright = 0;
	uint8_t attrib = 0;
	uint16_t pixel_addr = 0;
	uint16_t attrib_addr = 0;

	uint8_t pix;
	// Each byte in spectrum memory draws 8 pixels
	for(y=0; y<24;y++) {

		yp=y*8;

		// Pixel address calculated for each row
		pixel_addr = ((yp>>3)<<5)&0xFF;
		pixel_addr += ((yp&0x7)+(((yp>>6)&0x3)<<3))<<8;

		// attributes are easier to calculate
		attrib_addr = (16384+6144)+y*32;

		// Draw each 8x8 block
		for ( x=0;x<32;x++ ) {
			// Get Attribute
			attrib = zxmem[attrib_addr];

			// Lower 3 bits are ink
			ink = attrib & 0x7;

			// bits 5-3 are paper
			paper = (attrib >> 3) &0x7;

			// bit 6 is brightness
			bright = (attrib >>6) & 0x1;

			if(bright) {
				ink+=8;
				paper+=8;
			}

			// bit 7 is flash
			flash = (attrib >> 7);

			for(j=0;j<8;j++) {
//				printf("Pixel addr: %d X:%d Y:%d YP:%d YP7:%d YP78:%d \n",pixeladdr,x,y,yp,yp&0x7,(yp&0x7)<<8);

				// Read spectrum ram where pixel data lives
				pix = zxmem[pixel_addr+j*256+16384];

				// cache the SDL surface memory pointer
				bufaddr = (y*8+j)*screen->pitch + x*8;

				// plot each pixel as ink or paper
				for(i=7;i>=0;i--) {
					pixels[bufaddr]=(pix&1<<i)?ink:paper;
					bufaddr++;
				}

			}
			attrib_addr++;
			pixel_addr++;
		}
	}
	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
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
	// handle IN
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

