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
	printf("%s\n",__FUNCTION__);
	printf("Nothing to do\n");
}

void OSD_Quit(void) {
	printf("%s\n",__FUNCTION__);
}

void OSD_SetVideoMode(int width, int height, int fullscreen) {
	printf("%s\n",__FUNCTION__);
}

void OSD_SetPalette(void) {
	printf("%s\n",__FUNCTION__);
}

void OSD_RenderScreen(void) {
//	printf("%s\n",__FUNCTION__);
}

void OSD_Input(void) {
//	printf("%s\n",__FUNCTION__);
}
