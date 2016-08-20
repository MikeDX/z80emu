/* zxvid.h
 *
 * ZX Video Header file
 *
 */

#ifndef _ZXVID_H_
#define _ZXVID_H_

// Spectrum colours
typedef struct _ZX_Pen {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ZX_Pen;

GLOBAL ZX_Pen colours[16];

int ZX_SetPalette(void);
void ZX_Draw(void);

#endif
