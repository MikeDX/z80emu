/* sdl1.2.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 * OSDEP file for SDL2
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
uint8_t OSD_Keys[2048];

void OSD_KeyInit(void) {

	// TODO - use scancode instead of keycode?
	OSD_Keys[SDLK_LSHIFT-0x3FFFFD1A]=ZX_KEY_SHIFT;
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
	OSD_Keys[SDLK_LCTRL-0x3FFFFD1A]=ZX_KEY_SYM;
	OSD_Keys[SDLK_m]=ZX_KEY_M;
	OSD_Keys[SDLK_n]=ZX_KEY_N;
	OSD_Keys[SDLK_b]=ZX_KEY_B;

}
// Define program entry point
// Just calling ZX_main is enough for most
int main(int argc, char *argv[])
{
	ZX_main(argc, argv);
	return 0;
}

void OSD_Init(void) {
	// Initialise SDL2
	SDL_Init(SDL_INIT_EVERYTHING);
	
	// Setup keybindings
	OSD_KeyInit();
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
	int keysym;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				keysym = event.key.keysym.sym<2048?event.key.keysym.sym:event.key.keysym.sym-0x3FFFFD1A;
				indata[keyaddr[OSD_Keys[keysym]]]&=~keybuf[OSD_Keys[keysym]];

				if(event.key.keysym.sym == SDLK_ESCAPE) {
					debug = 1;
				}
				if(event.key.keysym.sym == SDLK_TAB) {
					debug = 0;
				}

				break;
			case SDL_KEYUP:
				keysym = event.key.keysym.sym<2048?event.key.keysym.sym:event.key.keysym.sym-0x3FFFFD1A;
				indata[keyaddr[OSD_Keys[keysym]]]|=keybuf[OSD_Keys[keysym]];

				break;

			case SDL_QUIT:
				running=0;
		}
	}
	return;

}

