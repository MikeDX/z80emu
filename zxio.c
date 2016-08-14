#include "zxem.h"

// Keyboard routines
// Verbose for no reason.
uint8_t keyaddr[1024];
uint8_t keybuf[1024];

uint8_t indata[255];

uint8_t keysetup = 0;

uint8_t readbyte(uint16_t addr) {
//	printf("Reading 0x%X\n",addr);
    return zxmem[addr];
}

uint16_t readword(uint16_t addr) {
//	printf("Reading 0x%X\n",addr);
    return zxmem[addr] | zxmem[addr+1]<<8;
}

void writebyte(uint16_t addr, uint8_t data) {

	/* Don't allow writing to ROM */
	if(addr>0x3FFF) {
		//printf("Writing 0x%X to 0x%X\n",data,addr);
	    zxmem[addr]=data;
	}
	cached[addr]=1;
}


void writeword(uint16_t addr, uint16_t data) {
	if(addr>=0x4000) {
//		printf("Writing 0x%X to 0x%X\n",data,addr);
	    zxmem[addr]=data;
	    zxmem[addr+1]=data>>8;
	}
	cached[addr]=1;
	cached[addr+1]=1;

}

uint8_t zx_data =0;
/* TODO: Fix IN routine */
uint8_t input(uint16_t port) {
	uint8_t regb = zxcpu.registers.byte[Z80_B];
	uint8_t data = zx_data;
	switch(regb) {
		case 0xFE:
		case 0xFD:
		case 0xFB:
		case 0xF7:
		case 0xEF:
		case 0xDF:
		case 0xBF:
		case 0x7F:
			data = indata[regb];
			return data;
			break;
		default: 
			printf("Reading port 0x%02x:%02x\n",regb,port);
			printf("PC: %04X %d\n",zxcpu.pc, zxcpu.pc);
			break;
	}

	data |= (0xe0); /* Set bits 5-7 - as reset above */
//	data &= ~0x40;
	// default
	return 255;//data;//indata[regb];
	//zx_data;
}

void output(uint16_t port, uint8_t data) {
//	indata[zxcpu.registers.byte[Z80_B]]=data;
//	uint8_t data = 0xff;
	printf("OUT %02X %02X %02X\n",zxcpu.registers.byte[Z80_B],port,data);
//	zxcpu.r=data;
//	indata[zxcpu.registers.byte[Z80_B]]=data;
	zx_data = data;

}
// Setup keyboard array

void ZX_KeyInit(void) {

	// 0xFEFE
	keyaddr[SDLK_LSHIFT]=0xFE;
	keyaddr[SDLK_LSHIFT]=0xFE;
	keyaddr[SDLK_z]=0xFE;
	keyaddr[SDLK_x]=0xFE;
	keyaddr[SDLK_c]=0xFE;
	keyaddr[SDLK_v]=0xFE;

	keybuf[SDLK_RSHIFT]=1;
	keybuf[SDLK_LSHIFT]=1;
	keybuf[SDLK_z]=2;
	keybuf[SDLK_x]=4;
	keybuf[SDLK_c]=8;
	keybuf[SDLK_v]=16;

	// 0xFDFE
	keyaddr[SDLK_a]=0xFD;
	keyaddr[SDLK_s]=0xFD;
	keyaddr[SDLK_d]=0xFD;
	keyaddr[SDLK_f]=0xFD;
	keyaddr[SDLK_g]=0xFD;

	keybuf[SDLK_a]=1;
	keybuf[SDLK_s]=2;
	keybuf[SDLK_d]=4;
	keybuf[SDLK_f]=8;
	keybuf[SDLK_g]=16;

	// 0xFBFE
	keyaddr[SDLK_q]=0xFB;
	keyaddr[SDLK_w]=0xFB;
	keyaddr[SDLK_e]=0xFB;
	keyaddr[SDLK_r]=0xFB;
	keyaddr[SDLK_t]=0xFB;

	keybuf[SDLK_q]=1;
	keybuf[SDLK_w]=2;
	keybuf[SDLK_e]=4;
	keybuf[SDLK_r]=8;
	keybuf[SDLK_t]=16;

	// 0xF7FE
	keyaddr[SDLK_1]=0xF7;
	keyaddr[SDLK_2]=0xF7;
	keyaddr[SDLK_3]=0xF7;
	keyaddr[SDLK_4]=0xF7;
	keyaddr[SDLK_5]=0xF7;

	keybuf[SDLK_1]=1;
	keybuf[SDLK_2]=2;
	keybuf[SDLK_3]=4;
	keybuf[SDLK_4]=8;
	keybuf[SDLK_5]=16;

	// 0xEFFE
	keyaddr[SDLK_6]=0xEF;
	keyaddr[SDLK_7]=0xEF;
	keyaddr[SDLK_8]=0xEF;
	keyaddr[SDLK_9]=0xEF;
	keyaddr[SDLK_0]=0xEF;

	keybuf[SDLK_0]=1;
	keybuf[SDLK_9]=2;
	keybuf[SDLK_8]=4;
	keybuf[SDLK_7]=8;
	keybuf[SDLK_6]=16;

	// 0xDFFE
	keyaddr[SDLK_p]=0xDF;
	keyaddr[SDLK_o]=0xDF;
	keyaddr[SDLK_i]=0xDF;
	keyaddr[SDLK_u]=0xDF;
	keyaddr[SDLK_y]=0xDF;

	keybuf[SDLK_p]=1;
	keybuf[SDLK_o]=2;
	keybuf[SDLK_i]=4;
	keybuf[SDLK_u]=8;
	keybuf[SDLK_y]=16;

	// 0xBFFE
	keyaddr[SDLK_RETURN]=0xBF;
	keyaddr[SDLK_l]=0xBF;
	keyaddr[SDLK_k]=0xBF;
	keyaddr[SDLK_j]=0xBF;
	keyaddr[SDLK_h]=0xBF;

	keybuf[SDLK_RETURN]=1;
	keybuf[SDLK_l]=2;
	keybuf[SDLK_k]=4;
	keybuf[SDLK_j]=8;
	keybuf[SDLK_h]=16;

	// 0x7FFE
	keyaddr[SDLK_SPACE]=0x7F;
	keyaddr[SDLK_LCTRL]=0x7F;
	keyaddr[SDLK_RCTRL]=0x7F;
	keyaddr[SDLK_m]=0x7F;
	keyaddr[SDLK_n]=0x7F;
	keyaddr[SDLK_b]=0x7F;

	keybuf[SDLK_SPACE]=1;
	keybuf[SDLK_LCTRL]=2;
	keybuf[SDLK_RCTRL]=2;
	keybuf[SDLK_m]=4;
	keybuf[SDLK_n]=8;
	keybuf[SDLK_b]=16;

	keysetup = 1;

	memset(indata,255,255);
}


void ZX_Input(void) {
	// Handle SDL
	if(!keysetup)
		ZX_KeyInit();

	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				indata[keyaddr[event.key.keysym.sym]]&=~keybuf[event.key.keysym.sym];
				//printf("%d %d\n",event.key.keysym.sym,indata[keyaddr[event.key.keysym.sym]]);
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

