/* zxem.c
 * 
 * ZX Spectrum emulator 
 * Copyright (c) 2016 MikeDX
 *
 * Cross platform ZX Spectrum emulator
 * 
 */

#define GLOBALS
#include "zxem.h"

double          total;
int stopped;

FILE *scrf;
int scri=0;

void scrloop(void) {
	if(!feof(scrf)) {
		fread(&zxmem[16384+scri],1,1,scrf);
		cached[16384+scri]=1;
		scri++;
		if(!(scri&0xF)) {
			ZX_Draw();
	//		return;
		}
	}
}

int ZX_LoadSCR(char *path) {
	FILE *f;
	int i = 0;
	int j = 16384;

	// fill screen with cross hatch
	memset(&zxmem[16384],0xAA,6144);
	for(i=0;i<12;i++) {
		memset(&zxmem[j],0xAA,256);
		memset(&zxmem[j+256],0x55,256);
		j+=512;
	}
	for (i=0;i<768;i++) {
		zxmem[j+i]=i&127;
	}
	while(running) {
		ZX_Input();
		ZX_Draw();
	}
	running = 1;
//	memset(&zxmem[j],199,768);
	memset(&cached[16384],1,6912);
	// for(i=0;i<10;i++) {
	// 	memset(&cached[16384],1,6912);
	// 	for(j=0;j<6912;j++) {
	// 		zxmem[16384+j]=rand()%256;
	// 	}
	// 	ZX_Draw();
	// }
	// memset(&zxmem[16384+6144],rand()%256/*64+7*/,32*24);
	if(!(f=fopen(path,"r"))) {
		printf("Failed to open screen\n");
		return -1;
	}
	scri=0;
	scrf=f;
#ifndef EMSCRIPTEN
	while(!feof(f) && running) {
		scrloop();
		ZX_Input();
	}
	fclose(f);
#else
	emscripten_set_main_loop(scrloop,0,1);
#endif	
	return 1;

}
int ZX_LoadROM(void) {
	/* Load ZX Spectrum ROM into lower 16K mem */
	FILE *f;

	if(!(f=fopen("roms/48k.rom","r"))) {
//	if(!(f=fopen("roms/JGH.ROM","r"))) {
		printf("Failed to open ROM\n");
		return -1;
	}

	fread(zxmem,1,16384,f);

	return 1;
}

void ZX_End(void) {
	free(zxmem);
	free(cached);
	free(screenbuf);
	OSD_Quit();
}

int oim  = 255;
/* Handle Interrupt - called before each screen render */
void ZX_Int(void) {
	if(oim!=zxcpu.im) {
		oim = zxcpu.im;
		printf("New interrupt: MODE: %d ",zxcpu.im);
		switch(oim) {
			case Z80_INTERRUPT_MODE_0:
				printf("mode 0\n");
				break;
			case Z80_INTERRUPT_MODE_1:
				printf("mode 1\n");
				break;
			case Z80_INTERRUPT_MODE_2:
				printf("mode 2\n");
				break;
			default:
				printf("????\n");
		}
						
	}

//	if(zxcpu.status==Z80_STATUS_FLAG_HALT) {
//		zxcpu.status&=~Z80_STATUS_FLAG_HALT;
//		zxcpu.pc++;
//	}

	Z80Emulate(&zxcpu, Z80Interrupt(&zxcpu,0xff));


////	Z80Emulate(&zxcpu, );
}

int next_total = 0;

void mainloop(void) {
	if(!debug) {
		total += Z80Emulate(&zxcpu, next_total-total);
	} else {
		total += Z80Emulate(&zxcpu, 1);
		printf("PC: 0x%04X\n",zxcpu.pc);
	}
	if(total>=next_total) {
		next_total +=CYCLES_PER_STEP;
		ZX_Input();
		ZX_Int();
		ZX_Draw();
	}
}

int ZX_main(int argc, char *argv[])
{
	int i =0;
	
	OSD_Init();
	ZX_KeyInit();
	
	/* Create 8 bit buffer */
	/* Spectrum screen is 32*8 by 24*8 */

	OSD_SetVideoMode(256,192,0);

	zxmem = (uint8_t *)malloc(65536);
	cached = (uint8_t *)malloc(65536);
	screenbuf = (uint8_t *)malloc(256*192);

	if(!zxmem || !cached) {
		printf("Failed to allocate RAM\n");
		ZX_End();
	}

	// Clear main memory
	memset(zxmem,0,65535);

	// Clear cached memory
	memset(cached,1,65535);

	// Attempt to load spectrum ROM
	if(!ZX_LoadROM()) {
		ZX_End();
	}

	ZX_SetPalette();

	if(argc>1) {
		running = 1;
		i = 0;
		while(++i<argc) {
			printf("Loading screen: %s\n",argv[i]);
			ZX_LoadSCR(argv[i]);
			// while(running) {
			// 	ZX_Draw();
			// 	ZX_Input();
			// }
		}
	}

	Z80Reset(&zxcpu);
	zxcpu.im = Z80_INTERRUPT_MODE_0;
	zxcpu.memory = zxmem;
	zxcpu.readbyte = readbyte;
	zxcpu.readword = readword;
	zxcpu.writeword = writeword;
	zxcpu.writebyte = writebyte;
	zxcpu.input = input;
	zxcpu.output = output;
	
	total = 0;
	running = 1;
	debug = 0;

#ifndef EMSCRIPTEN
	while(running) {
		mainloop();
	}
#else
	emscripten_set_main_loop(mainloop,0,1);
#endif
	ZX_End();
	return 0;
}

