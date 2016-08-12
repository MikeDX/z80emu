#include "zxem.h"

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
