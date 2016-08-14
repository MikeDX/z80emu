#include "zxem.h"

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

/* TODO: Fix IN routine */
uint8_t input(uint16_t port) {
	return 0;
}


void ZX_Input(void) {
	return;
}

