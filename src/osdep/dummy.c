/* dummy.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 * OSDEP file - DUMMY. 
 */
#include "osdep.h"
#include "zxem.h"

int main(int argc, char *argv[])
{
	ZX_main(argc, argv);
	return 0;
}

void OSD_Init(void) {

}

void OSD_Quit(void) {

}

void OSD_SetVideoMode(int width, int height, int fullscreen) {

}

void OSD_SetPalette(void) {

}

void OSD_RenderScreen(void) {

}

void OSD_Input(void) {

}
