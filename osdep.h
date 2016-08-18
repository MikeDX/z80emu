/* osdep.h
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 * A new port must supply these functions in order to operate
 */

#ifndef _OSDEP_H_
#define _OSDEP_H_

void OSD_Init(void);
void OSD_Quit(void);

void OSD_SetVideoMode(int width, int height, int fullscreen);
void OSD_SetPalette(void);
void OSD_RenderScreen(void);

void OSD_Input(void);

#endif
