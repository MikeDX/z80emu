// ZXTEST

#include "../src/cpu/z80emu/z80emu.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint8_t *zxmem;

Z80_STATE zxcpu;
uint8_t writes[65536];
uint8_t reads[65536];

int total, running, debug;

uint8_t readbyte(uint16_t addr) {
//	printf("Reading 0x%X\n",addr);
	printf("%4d MR %04X %2X\n",zxcpu.elapsed_cycles, addr, zxmem[addr]);

		// printf("Reads:\n");
		// for(i=0;i<65535;i++) {
		// 	if(reads[i]>0) {
		// 		printf("MR %04X %d %02X\n",i,reads[i],zxmem[i]);
		// 	}
		// 	if(writes[i]>0) {
		// 		printf("MW %04X %d %02X\n",i,writes[i],zxmem[i]);
		// 	}
		// }

	reads[addr]++;
    return zxmem[addr];
}

uint16_t readword(uint16_t addr) {
//	printf("Reading 0x%X\n",addr);
	printf("%4d MR %04X %02X\n",zxcpu.elapsed_cycles, addr+1, zxmem[addr+1]);
	printf("%4d MR %04X %02X\n",zxcpu.elapsed_cycles, addr, zxmem[addr]);
	reads[addr]++;
	reads[addr+1]++;
    return zxmem[addr] | zxmem[addr+1]<<8;
}

void writebyte(uint16_t addr, uint8_t data) {

	printf("%4d MW %04X %02X\n",zxcpu.elapsed_cycles, addr, data);

	/* Don't allow writing to ROM */
//	if(addr>0x3FFF) {
		//printf("Writing 0x%X to 0x%X\n",data,addr);
	    zxmem[addr]=data;
	    writes[addr]++;
//	}
	//cached[addr]=1;
}


void writeword(uint16_t addr, uint16_t data) {
	printf("%4d MW %04X %02X\n",zxcpu.elapsed_cycles, addr+1, data>>8);
	printf("%4d MW %04X %02X\n",zxcpu.elapsed_cycles, addr, data&0x00FF);

//	if(addr>=0x4000) {
//		printf("Writing 0x%X to 0x%X\n",data,addr);
	    zxmem[addr]=data;
	    writes[addr]++;

	    zxmem[addr+1]=data>>8;
	    writes[addr+1]++;
//	}

//	cached[addr]=1;
//	cached[addr+1]=1;

}

uint8_t zx_data =0;
/* TODO: Fix IN routine */
uint8_t input(uint16_t port) {
/*	uint8_t regb = zxcpu.registers.byte[Z80_B];
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

	data |= (0xe0); // Set bits 5-7 - as reset above 
//	data &= ~0x40;
	// default

*/
	return 255;//data;//indata[regb];
	//zx_data;

}

void output(uint16_t port, uint8_t data) {
//	indata[zxcpu.registers.byte[Z80_B]]=data;
//	uint8_t data = 0xff;
	printf("OUT %02X %02X %02X\n",zxcpu.registers.byte[Z80_B],port,data);
//	zxcpu.r=data;
//	indata[zxcpu.registers.byte[Z80_B]]=data;
//	zx_data = data;

}
int main(int argc, char *argv[]) {
	FILE *in;
	FILE *out;
	char testid[255];//= 0;
	char testide[255];
	int AF,BC,DE,HL,AF2,BC2,DE2,HL2,IX,IY,SP,PC;
	int I, R, IFF1, IFF2, IM, halted, tstates, etstates;
	int addr;
	int data;
	int i;
//	00 00 0 0 0 0     1
	zxmem = (uint8_t*)malloc(65535);
	memset(zxmem,0,65535);

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

	in = fopen("tests.in","r");

	out = fopen("tests.expected","r");

	if(!in) {
		printf("Cannot open tests input file\n");
	}

	while(!feof(in)) {
//	if(1) {
		memset(testid,0,255);
		memset(testide,0,255);
		
		memset(zxmem,0,65535);
		fscanf(in,"%s\n",testid);
//		printf("TEST ID: %s\n",testid);

		fscanf(in,"%x %x %x %x %x %x %x %x %x %x %x %x\n",&AF,&BC,&DE,&HL,&AF2,&BC2,&DE2,&HL2,&IX,&IY,&SP,&PC);

		fscanf(in,"%x %x %x %x %x %x     %d\n",&I, &R, &IFF1, &IFF2, &IM, &halted, &tstates);

		while(1) {
			fscanf(in,"%x ", &addr);
			if(addr==-1) {
				break;
			}
			while(1) {
				fscanf(in,"%x ", &data);
				if(data==-1) {
					break;
				}
//				printf("%d %d\n",addr,data);
				zxmem[addr]=data;
				addr++;
			}
		}
		// RUN TEST

		Z80Reset(&zxcpu);

		zxcpu.registers.word[Z80_AF]=AF;
		zxcpu.alternates[Z80_AF]=AF2;
		zxcpu.registers.word[Z80_BC]=BC;
		zxcpu.alternates[Z80_BC]=BC2;
		zxcpu.registers.word[Z80_DE]=DE;
		zxcpu.alternates[Z80_DE]=DE2;
		zxcpu.registers.word[Z80_HL]=HL;
		zxcpu.alternates[Z80_HL]=HL2;

		zxcpu.registers.word[Z80_IX]=IX;
		zxcpu.registers.word[Z80_IY]=IY;

		zxcpu.registers.word[Z80_SP] = SP;
		zxcpu.pc = PC;
		zxcpu.elapsed_cycles = 0;

		zxcpu.i = I;
		memset(reads,0,65535);
		memset(writes,0,65535);
		
		tstates = Z80Emulate(&zxcpu, tstates);

		for(i=0;i<65535;i++) {
			// if(reads[i]>0) {
			// 	printf("MR %04X %d %02X\n",i,reads[i],zxmem[i]);
			// }
			if(writes[i]>0) {
				printf("%04X %02X\n",i,zxmem[i]);
			}
		}

		fscanf(out,"%s",testide);
		printf("TEST ID: [%s] [%s]\n",testid, testide);
		while(!feof(out)) {
			memset(testid,0,255);
			fgets(testid,255,out);
//			printf("%d, [%s]\n",testid[0],testid);
			if(testid[0]!=32 && testid[0]!=10) {
				fseek(out,-strlen(testid),SEEK_CUR);
				break;
			}
		}
//		fgets(testid,255,out);
//		printf("FIRST LINE: [%s]\n",testid);

		fscanf(out,"%x %x %x %x %x %x %x %x %x %x %x %x\n",&AF,&BC,&DE,&HL,&AF2,&BC2,&DE2,&HL2,&IX,&IY,&SP,&PC);

//		fscanf(out,"%x %x %x %x %x %x %x\n",&I, &R, &IFF1, &IFF2, &IM, &halted, &etstates);
		fscanf(out,"%x %x %x %x %x %d\n",&I, &R, &IFF1, &IFF2, &IM, &etstates);

		while(1) {
			fgets(testid,255,out);
			if(testid[0]=='\n') {
				break;
			}
		}
		if (
		zxcpu.registers.word[Z80_AF]==AF &&
		zxcpu.alternates[Z80_AF]==AF2 &&
		zxcpu.registers.word[Z80_BC]==BC &&
		zxcpu.alternates[Z80_BC]==BC2 &&
		zxcpu.registers.word[Z80_DE]==DE &&
		zxcpu.alternates[Z80_DE]==DE2 &&
		zxcpu.registers.word[Z80_HL]==HL &&
		zxcpu.alternates[Z80_HL]==HL2 &&

		zxcpu.registers.word[Z80_IX]==IX &&
		zxcpu.registers.word[Z80_IY]==IY &&

		zxcpu.registers.word[Z80_SP] == SP &&
		zxcpu.pc == PC
		) {
		printf("PASSED\n");
		} else {
			printf("FAILED\n");
			printf("AF: %04X : %04X\n", zxcpu.registers.word[Z80_AF],AF);
			printf("AF': %04X : %04X\n", zxcpu.alternates[Z80_AF],AF2);
			printf("BC: %04X : %04X\n", zxcpu.registers.word[Z80_BC],BC);
			printf("BC': %04X : %04X\n", zxcpu.alternates[Z80_BC],BC2);
			printf("DE: %04X : %04X\n", zxcpu.registers.word[Z80_DE],DE);
			printf("DE': %04X : %04X\n", zxcpu.alternates[Z80_DE],DE2);
			printf("HL: %04X : %04X\n", zxcpu.registers.word[Z80_HL],HL);
			printf("HL': %04X : %04X\n", zxcpu.alternates[Z80_HL],HL2);

			printf("IX: %04X : %04X\n", zxcpu.registers.word[Z80_IX],IX);
			printf("IY: %04X : %04X\n", zxcpu.registers.word[Z80_IY],IY);
			printf("SP: %04X : %04X\n", zxcpu.registers.word[Z80_SP],SP);

			printf("PC: %04X : %04X\n", zxcpu.pc, PC);

			printf("TSTATES: %d : %d\n", etstates, tstates);

			break;
		}
	}
	fclose(in);
	fclose(out);
	printf("DONE\n");

	free(zxmem);
	return 0;

}
