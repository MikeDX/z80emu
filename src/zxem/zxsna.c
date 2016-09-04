/* zxsna.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 */

#include "zxem.h"

int ZX_LoadSNA(char *filename) {

	uint8_t *snablock;
	uint16_t ival;
	int flen;
	FILE *f;
	f=fopen(filename,"r");
	if(!f) {
		printf("Failed to open %s\n",filename);
		return 0;
	}

	fseek(f,0,SEEK_END);
	flen = ftell(f);
	printf("SNAP LENGTH: %d\n",flen);

	snablock = (uint8_t*) malloc(flen);

	fseek(f,0,SEEK_SET);
	fread(snablock,1,flen,f);
/*
	 Offset   Size   Description
   ------------------------------------------------------------------------
   0        1      byte   I
   1        8      word   HL',DE',BC',AF'
   9        10     word   HL,DE,BC,IY,IX
   19       1      byte   Interrupt (bit 2 contains IFF2, 1=EI/0=DI)
   20       1      byte   R
   21       4      words  AF,SP
   25       1      byte   IntMode (0=IM0/1=IM1/2=IM2)
   26       1      byte   BorderColor (0..7, not used by Spectrum 1.7)
   27       49152  bytes  RAM dump 16384..65535
   ------------------------------------------------------------------------
   Total: 49179 bytes
*/

	ival = snablock[0];
	CPU_SetReg(CPU_Handle, (char *)"I", ival);

	ival = snablock[1]+snablock[2]*256;
	CPU_SetReg(CPU_Handle, (char *)"hl", ival);

	ival = snablock[3]+snablock[4]*256;
	CPU_SetReg(CPU_Handle, (char *)"de", ival);

	ival = snablock[5]+snablock[6]*256;
	CPU_SetReg(CPU_Handle, (char *)"bc", ival);

	ival = snablock[8]+snablock[7]*256;
	CPU_SetReg(CPU_Handle, (char *)"af", ival);

	ival = snablock[9]+snablock[10]*256;
	CPU_SetReg(CPU_Handle, (char *)"HL", ival);

	ival = snablock[11]+snablock[12]*256;
	CPU_SetReg(CPU_Handle, (char *)"DE", ival);

	ival = snablock[13]+snablock[14]*256;
	CPU_SetReg(CPU_Handle, (char *)"BC", ival);

	ival = snablock[15]+snablock[16]*256;
	CPU_SetReg(CPU_Handle, (char *)"IY", ival);

	ival = snablock[17]+snablock[18]*256;
	CPU_SetReg(CPU_Handle, (char *)"IX", ival);

	// byte 19 is interrupt bit - TODO

	printf("Interrupt BIT: %02x\n",snablock[19]);

	//if(snablock[19]&4)


	ival = snablock[20];
	CPU_SetReg(CPU_Handle, (char *)"R", ival);

	ival = snablock[21]+snablock[22]*256;
	CPU_SetReg(CPU_Handle, (char *)"AF", ival);

	ival = snablock[23]+snablock[24]*256;
	CPU_SetReg(CPU_Handle, (char *)"SP", ival);

	// byte 25 is interrupt mode - TODO

	printf("Interrupt MODE: %02x\n",snablock[25]);

	CPU_SetIMode(CPU_Handle, snablock[25]);

	//if(snablock[19]&4)

	if(flen>49179) {
		ival = snablock[49179]+snablock[49180]*256;
		CPU_SetReg(CPU_Handle, (char *)"PC", ival);
	} else {
		// 48k SNA.. TO DO
	}

	memcpy(&zxmem[16384],&snablock[27],49152);


	// byte 25 - interupt mode

	// byte 26 - border colour

	// 

	free(snablock);

	return 1;

}

int ZX_SaveSNA(char *filename) {
	return 0;
}
