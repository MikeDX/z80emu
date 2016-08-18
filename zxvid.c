/* zxvid.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 * Video handling routines.
 */

#include "zxem.h"

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


	OSD_SetPalette();
	
	return 1;

}

// TODO:
// Updated video buffer on scaline beam

void ZX_Draw(void) {
	/* Draw the screen */
	static uint8_t flash_flip = 0;
	static uint8_t flash_count = 0;
	int x=0,y=0;
	int i=0;
	int j=0;
	uint8_t yp=0;

	uint8_t ink = 0;
	uint8_t paper = 0;
	uint8_t flash = 0;
	uint8_t bright = 0;
	uint8_t attrib = 0;

	uint16_t buf_addr = 0;
	uint16_t pixel_addr = 0;
	uint16_t attrib_addr = 0;

	uint8_t pix;

	attrib_addr = 16384+6144;
	// Each byte in spectrum memory draws 8 pixels
	// with an additional byte for attributes of each 8x8 pixel block.

	// Draw all rows 0 - 23
	for(y=0; y<24;y++) {

		// yp is y in pixels
		yp=y*8;

		// Calculate pixel address offset from the x/y 8x8 block we want to draw.
		pixel_addr = ((yp>>3)<<5)&0xFF;
		pixel_addr += ((yp&0x7)+(((yp>>6)&0x3)<<3))<<8;

		pixel_addr +=16384; // Start of screen memory

		// Draw each 8x8 block 0 - 31
		for ( x=0;x<32;x++ ) {

			// Buf Addr is the first pixel we want to draw on
			buf_addr = (y*8)*256+x*8;

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

			// TODO - which is faster?
			// flash = (attrib >> 7);
			// flash = (attrib > 63);
			flash = (attrib & 0x80);

			if(flash && flash_flip) {
				// XOR trick for swapping ink / paper
				paper = paper ^ ink;
				ink = paper ^ ink;
				paper = paper ^ ink;
			}

			for(j=0;j<8;j++) {

				if(flash || cached[attrib_addr] || cached[pixel_addr+j*256]) {

					// Read spectrum ram where pixel data lives
					pix = zxmem[pixel_addr+j*256];

					// plot each pixel as ink or paper
					for(i=7;i>=0;i--) {
						screenbuf[buf_addr]=(pix&1<<i)?ink:paper;
						buf_addr++;
					}
					// move buffer back 
					buf_addr-=8;
				}
				// move to next line
				buf_addr+=256;
			}
			
			// move attrib address along
			attrib_addr++;

			// move pixel byte along
			pixel_addr++;
		}

	}

	memset(&cached[16384],0,6912);

	// Flash flip changes every 16 frames
	flash_count++;
	if(flash_count>16) {
		flash_flip = !flash_flip;
		flash_count = 0;
	}

	// Draw the screen
	OSD_RenderScreen();
}

