/* sdl1.2.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 * OSDEP file for SDL1.2
 */
#include <SDL2/SDL.h>
#include "osdep.h"
#include "zxem.h"


SDL_Renderer* renderer=NULL;
SDL_Window* window=NULL;
SDL_Surface* buffer8=NULL;
SDL_Surface* buffer32=NULL;
SDL_Texture* texture=NULL;
char windowtitle[1024];

// Define program entry point
// Just calling ZX_main is enough for most
int main(int argc, char *argv[])
{
	ZX_main(argc, argv);
	return 0;
}

void OSD_Init(void) {
	SDL_Init(SDL_INIT_EVERYTHING);
	// TODO - Setup keybindings
}

void OSD_Quit(void) {
	// free resources
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(buffer32);
	SDL_FreeSurface(buffer8); 		

	SDL_Quit();
}

void OSD_SetVideoMode(int width, int height, int fullscreen) {
	uint32_t rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, 
    	&window, &renderer);

	buffer8 = SDL_CreateRGBSurface(0, width, height, 8,
		0,0,0,0);
	
	buffer32 = SDL_CreateRGBSurface(0, width, height, 32,
		0,0,0,0);

	texture = SDL_CreateTextureFromSurface(renderer, buffer32);

	return;

}

void OSD_SetPalette(void) {
	// convert colours to SDL colours
	SDL_Colour osd_colours[16];
	int x=0;
	while(x<16) {
		osd_colours[x].r = colours[x].r;
		osd_colours[x].g = colours[x].g;
		osd_colours[x].b = colours[x].b;
		x++;
	}

	SDL_SetPaletteColors(buffer8->format->palette, osd_colours, 0, 16);

}

void OSD_RenderScreen(void) {
	int y;
	SDL_LockSurface(buffer8);
	uint8_t *pixels = buffer8->pixels;
	for(y=0;y<192;y++) {
		memcpy(pixels,&screenbuf[y*256],256);
		pixels+=buffer8->pitch;
	}
	SDL_UnlockSurface(buffer8);

	SDL_BlitSurface(buffer8, NULL, buffer32, NULL);

	/* Modify the texture's pixels */
	SDL_UpdateTexture(texture, NULL, buffer32->pixels, buffer32->pitch);

	/* Make the modified texture visible by rendering it */
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);
}

void OSD_Input(void) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
//				indata[keyaddr[event.key.keysym.sym]]&=~keybuf[event.key.keysym.sym];
//				printf("%d %d\n",event.key.keysym.sym,indata[keyaddr[event.key.keysym.sym]]);
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					debug = 1;
				}
				if(event.key.keysym.sym == SDLK_TAB) {
					debug = 0;
				}

				break;
			case SDL_KEYUP:
//				indata[keyaddr[event.key.keysym.sym]]|=keybuf[event.key.keysym.sym];
				break;

			case SDL_QUIT:
				running=0;
		}
	}
	return;

}

