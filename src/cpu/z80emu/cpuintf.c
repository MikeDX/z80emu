/* cpuintf.c
 * 
 * CPU Interface (mz80)
 *
 * Allows ZXEM to be used with different cpu cores
 *
 */

#include "zxem.h"
#include "z80emu.h"

#define MAX_CPU 10

Z80_STATE *zxcpu[MAX_CPU];
uint8_t num_cpus = -1;

// create CPU context and return a handle
uint8_t CPU_Create(void) {
	num_cpus++;
	if(num_cpus<MAX_CPU) {
		zxcpu[num_cpus]=(Z80_STATE*)malloc(sizeof(Z80_STATE));
		zxcpu[num_cpus]->memory = zxmem;
		zxcpu[num_cpus]->readbyte = readbyte;
		zxcpu[num_cpus]->readword = readword;
		zxcpu[num_cpus]->writeword = writeword;
		zxcpu[num_cpus]->writebyte = writebyte;
		zxcpu[num_cpus]->input = input;
		zxcpu[num_cpus]->output = output;
		return num_cpus;
	} 

	return 255;
}

// reset a CPU context
void CPU_Reset(uint8_t cpuid) {

	Z80Reset(zxcpu[cpuid]);
	zxcpu[cpuid]->im = Z80_INTERRUPT_MODE_0;
}


// Assign a block of memory as read/write
void CPU_MEMRW(int addr, int length) {

}

// Assign a block of memory as read only
void CPU_MEMR(int addr, int length) {

}

// Destroy a CPU context
void CPU_Destroy(uint8_t cpuid) {
	free(zxcpu[cpuid]);
	zxcpu[cpuid]=NULL;
}

// Emulatoe virtual CPU
int CPU_Emulate(uint8_t cpuid, int ticks) {
	return Z80Emulate(zxcpu[cpuid], ticks);
}

int oim;

// Call interrupt on a CPU
void CPU_Interrupt(uint8_t cpuid) {
	if(oim!=zxcpu[cpuid]->im) {
		oim = zxcpu[cpuid]->im;
		printf("New interrupt: MODE: %d ",oim);
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

	Z80Emulate(zxcpu[cpuid], Z80Interrupt(zxcpu[cpuid],0xff));

}

// Call an NMI on a CPU
void CPU_NMI(uint8_t cpuid) {
	// TODO
}

void CPU_SetReg(uint8_t cpuid, char *reg, uint16_t value) {

	int creg = reg[0]+reg[1]*256;

	switch(creg) {
		case 66:
			zxcpu[cpuid]->registers.byte[Z80_B]=value;
			return;
			break;
		case 17232:
			zxcpu[cpuid]->pc = value;
			return;
			break;

		default: 
			printf("Unimplemented reg [%s] (%d)\n",reg, creg);
			return 0;

	}

}

uint16_t CPU_GetReg(uint8_t cpuid, char *reg) {

	int creg=reg[0]+reg[1]*256;
//	printf("REG: %s %d\n",reg,creg);
	switch(creg) {
		case 66: // B
			return zxcpu[cpuid]->registers.byte[Z80_B];
			break;
		case 17232: // PC
			return zxcpu[cpuid]->pc;
			break;
		default:
			printf("Unimplemented reg [%s] (%d)\n",reg, creg);
			return 0;
	}
}
