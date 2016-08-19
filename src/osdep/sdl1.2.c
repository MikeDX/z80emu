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


uint8_t OSD_Keys[2048];

void OSD_KeyInit(void) {
	OSD_Keys[SDLK_LSHIFT]=ZX_KEY_SHIFT;
	OSD_Keys[SDLK_z]=ZX_KEY_Z;
	OSD_Keys[SDLK_x]=ZX_KEY_X;
	OSD_Keys[SDLK_c]=ZX_KEY_C;
	OSD_Keys[SDLK_v]=ZX_KEY_V;

	OSD_Keys[SDLK_a]=ZX_KEY_A;
	OSD_Keys[SDLK_s]=ZX_KEY_S;
	OSD_Keys[SDLK_d]=ZX_KEY_D;
	OSD_Keys[SDLK_f]=ZX_KEY_F;
	OSD_Keys[SDLK_g]=ZX_KEY_G;

	OSD_Keys[SDLK_q]=ZX_KEY_Q;
	OSD_Keys[SDLK_w]=ZX_KEY_W;
	OSD_Keys[SDLK_e]=ZX_KEY_E;
	OSD_Keys[SDLK_r]=ZX_KEY_R;
	OSD_Keys[SDLK_t]=ZX_KEY_T;

	OSD_Keys[SDLK_1]=ZX_KEY_1;
	OSD_Keys[SDLK_2]=ZX_KEY_2;
	OSD_Keys[SDLK_3]=ZX_KEY_3;
	OSD_Keys[SDLK_4]=ZX_KEY_4;
	OSD_Keys[SDLK_5]=ZX_KEY_5;

	OSD_Keys[SDLK_0]=ZX_KEY_0;
	OSD_Keys[SDLK_9]=ZX_KEY_9;
	OSD_Keys[SDLK_8]=ZX_KEY_8;
	OSD_Keys[SDLK_7]=ZX_KEY_7;
	OSD_Keys[SDLK_6]=ZX_KEY_6;

	OSD_Keys[SDLK_p]=ZX_KEY_P;
	OSD_Keys[SDLK_o]=ZX_KEY_O;
	OSD_Keys[SDLK_i]=ZX_KEY_I;
	OSD_Keys[SDLK_u]=ZX_KEY_U;
	OSD_Keys[SDLK_y]=ZX_KEY_Y;

	OSD_Keys[SDLK_RETURN]=ZX_KEY_ENTER;
	OSD_Keys[SDLK_l]=ZX_KEY_L;
	OSD_Keys[SDLK_k]=ZX_KEY_K;
	OSD_Keys[SDLK_j]=ZX_KEY_J;
	OSD_Keys[SDLK_h]=ZX_KEY_H;

	OSD_Keys[SDLK_SPACE]=ZX_KEY_SPACE;
	OSD_Keys[SDLK_LCTRL]=ZX_KEY_SYM;
	OSD_Keys[SDLK_m]=ZX_KEY_M;
	OSD_Keys[SDLK_n]=ZX_KEY_N;
	OSD_Keys[SDLK_b]=ZX_KEY_B;

}

SDL_Surface *screen;

// Define program entry point
// Just calling ZX_main is enough for most

int main(int argc, char *argv[])
{
	ZX_main(argc, argv);
	return 0;
}

void OSD_Init(void) {
	// Initialise SDL 1.2
	SDL_Init(SDL_INIT_EVERYTHING);

	// Setup keybindings
	OSD_KeyInit();
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
	int keysym;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				keysym = event.key.keysym.sym;
				indata[keyaddr[OSD_Keys[keysym]]]&=~keybuf[OSD_Keys[keysym]];

				if(event.key.keysym.sym == SDLK_ESCAPE) {
					debug = 1;
				}
				if(event.key.keysym.sym == SDLK_TAB) {
					debug = 0;
				}

				break;

			case SDL_KEYUP:
				keysym = event.key.keysym.sym;
				indata[keyaddr[OSD_Keys[keysym]]]|=keybuf[OSD_Keys[keysym]];
				break;

			case SDL_QUIT:
				running=0;
		}
	}
	return;

}
