#include "zxem.h"


// Keyboard routines
// Verbose for no reason.


uint8_t readbyte(uint16_t addr) {
    return zxmem[addr];
}

uint16_t readword(uint16_t addr) {
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
	uint8_t regb = CPU_GetReg(CPU_Handle,"B");
//	printf("Reading %02X\n",regb);
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
//			printf("Indata: %d\n",indata[regb]);
			return data;
			break;
		default: 
			printf("Reading port 0x%02x:%02x\n",regb,port);
			printf("PC: %04X %d\n",CPU_GetReg(CPU_Handle, "PC"), CPU_GetReg(CPU_Handle,"PC"));
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
	printf("OUT %02X %02X %02X\n",CPU_GetReg(CPU_Handle,"B"),port,data);
//	zxcpu.r=data;
//	indata[zxcpu.registers.byte[Z80_B]]=data;
	zx_data = data;

}
// Setup keyboard array

void ZX_KeyInit(void) {

	// 0xFEFE
	keyaddr[ZX_KEY_SHIFT]=0xFE;
	keyaddr[ZX_KEY_Z]=0xFE;
	keyaddr[ZX_KEY_X]=0xFE;
	keyaddr[ZX_KEY_C]=0xFE;
	keyaddr[ZX_KEY_V]=0xFE;

	keybuf[ZX_KEY_SHIFT]=1;
	keybuf[ZX_KEY_Z]=2;
	keybuf[ZX_KEY_X]=4;
	keybuf[ZX_KEY_C]=8;
	keybuf[ZX_KEY_V]=16;

	// 0xFDFE
	keyaddr[ZX_KEY_A]=0xFD;
	keyaddr[ZX_KEY_S]=0xFD;
	keyaddr[ZX_KEY_D]=0xFD;
	keyaddr[ZX_KEY_F]=0xFD;
	keyaddr[ZX_KEY_G]=0xFD;

	keybuf[ZX_KEY_A]=1;
	keybuf[ZX_KEY_S]=2;
	keybuf[ZX_KEY_D]=4;
	keybuf[ZX_KEY_F]=8;
	keybuf[ZX_KEY_G]=16;

	// 0xFBFE
	keyaddr[ZX_KEY_Q]=0xFB;
	keyaddr[ZX_KEY_W]=0xFB;
	keyaddr[ZX_KEY_E]=0xFB;
	keyaddr[ZX_KEY_R]=0xFB;
	keyaddr[ZX_KEY_T]=0xFB;

	keybuf[ZX_KEY_Q]=1;
	keybuf[ZX_KEY_W]=2;
	keybuf[ZX_KEY_E]=4;
	keybuf[ZX_KEY_R]=8;
	keybuf[ZX_KEY_T]=16;

	// 0xF7FE
	keyaddr[ZX_KEY_1]=0xF7;
	keyaddr[ZX_KEY_2]=0xF7;
	keyaddr[ZX_KEY_3]=0xF7;
	keyaddr[ZX_KEY_4]=0xF7;
	keyaddr[ZX_KEY_5]=0xF7;

	keybuf[ZX_KEY_1]=1;
	keybuf[ZX_KEY_2]=2;
	keybuf[ZX_KEY_3]=4;
	keybuf[ZX_KEY_4]=8;
	keybuf[ZX_KEY_5]=16;

	// 0xEFFE
	keyaddr[ZX_KEY_0]=0xEF;
	keyaddr[ZX_KEY_9]=0xEF;
	keyaddr[ZX_KEY_8]=0xEF;
	keyaddr[ZX_KEY_7]=0xEF;
	keyaddr[ZX_KEY_6]=0xEF;

	keybuf[ZX_KEY_0]=1;
	keybuf[ZX_KEY_9]=2;
	keybuf[ZX_KEY_8]=4;
	keybuf[ZX_KEY_7]=8;
	keybuf[ZX_KEY_6]=16;

	// 0xDFFE
	keyaddr[ZX_KEY_P]=0xDF;
	keyaddr[ZX_KEY_O]=0xDF;
	keyaddr[ZX_KEY_I]=0xDF;
	keyaddr[ZX_KEY_U]=0xDF;
	keyaddr[ZX_KEY_Y]=0xDF;

	keybuf[ZX_KEY_P]=1;
	keybuf[ZX_KEY_O]=2;
	keybuf[ZX_KEY_I]=4;
	keybuf[ZX_KEY_U]=8;
	keybuf[ZX_KEY_Y]=16;

	// 0xBFFE
	keyaddr[ZX_KEY_ENTER]=0xBF;
	keyaddr[ZX_KEY_L]=0xBF;
	keyaddr[ZX_KEY_K]=0xBF;
	keyaddr[ZX_KEY_J]=0xBF;
	keyaddr[ZX_KEY_H]=0xBF;

	keybuf[ZX_KEY_ENTER]=1;
	keybuf[ZX_KEY_L]=2;
	keybuf[ZX_KEY_K]=4;
	keybuf[ZX_KEY_J]=8;
	keybuf[ZX_KEY_H]=16;

	// 0x7FFE
	keyaddr[ZX_KEY_SPACE]=0x7F;
	keyaddr[ZX_KEY_SYM]=0x7F;
	keyaddr[ZX_KEY_M]=0x7F;
	keyaddr[ZX_KEY_N]=0x7F;
	keyaddr[ZX_KEY_B]=0x7F;

	keybuf[ZX_KEY_SPACE]=1;
	keybuf[ZX_KEY_SYM]=2;
	keybuf[ZX_KEY_M]=4;
	keybuf[ZX_KEY_N]=8;
	keybuf[ZX_KEY_B]=16;

	memset(indata,255,255);
}


void ZX_Input(void) {
	OSD_Input();
}

