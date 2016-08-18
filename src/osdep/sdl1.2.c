/* sdl1.2.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 * OSDEP file for SDL1.2
 */
#include <SDL.h>
#include "osdep.h"
#include "zxem.h"


SDL_Surface *screen;

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
	// TODO - Destroy stuff
	SDL_FreeSurface(screen);
	SDL_Quit();
}

void OSD_SetVideoMode(int width, int height, int fullscreen) {
	screen = SDL_SetVideoMode(width, height, 8, SDL_SWSURFACE);
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

	SDL_SetPalette(screen,SDL_LOGPAL|SDL_PHYSPAL,osd_colours,0,16);
}

void OSD_RenderScreen(void) {
	int y;
	SDL_LockSurface(screen);
	uint8_t *pixels = screen->pixels;
	for(y=0;y<192;y++) {
		memcpy(pixels,&screenbuf[y*256],256);
		pixels+=screen->pitch;
	}
	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
}

void OSD_Input(void) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				indata[keyaddr[event.key.keysym.sym]]&=~keybuf[event.key.keysym.sym];
//				printf("%d %d\n",event.key.keysym.sym,indata[keyaddr[event.key.keysym.sym]]);
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					debug = 1;
				}
				if(event.key.keysym.sym == SDLK_TAB) {
					debug = 0;
				}

				break;
			case SDL_KEYUP:
				indata[keyaddr[event.key.keysym.sym]]|=keybuf[event.key.keysym.sym];
				break;

			case SDL_QUIT:
				running=0;
		}
	}
	return;

}
