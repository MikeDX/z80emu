/* cpuintf.c
 * 
 * CPU Interface (z80core)
 *
 * z80Core by Adrian Brown (website / contact??)
 *
 * Allows ZXEM to be used with different cpu cores
 *
 */

#include "zxem.h"
#include "Z80Core_CInterface.h"

#define MAX_CPU 10

void *zxcpu[MAX_CPU];
uint8_t num_cpus = -1;

unsigned char coreRead(unsigned short addr, int param) {
	return readbyte(addr);
}

void coreWrite(unsigned short address, unsigned char data, int param) {
	writebyte(address, data);
}

unsigned char IORead(unsigned short addr, int param) {
	return input(addr);
}

void IOWrite(unsigned short address, unsigned char data,int param) {
	output(address, data);
}

// Just return, dont handle contention
void coreContention(unsigned short address, unsigned int tstates,int param) {
	return;
}

// create CPU context and return a handle
uint8_t CPU_Create(void) {
	num_cpus++;
	if(num_cpus<MAX_CPU) {
		zxcpu[num_cpus]=Z80CORE_Create();
		Z80CORE_Initialise(zxcpu[num_cpus], coreRead,coreWrite,IORead,IOWrite,coreContention,coreContention,num_cpus);
		return num_cpus;
	} 

	return 255;
}

// reset a CPU context
void CPU_Reset(uint8_t cpuid) {
	Z80CORE_Reset(zxcpu[num_cpus]);
}

// Destroy a CPU context
void CPU_Destroy(uint8_t cpuid) {
	Z80CORE_Destroy(zxcpu[cpuid]);
	zxcpu[cpuid]=NULL;
}

// Emulatoe virtual CPU
int CPU_Emulate(uint8_t cpuid, int ticks) {
//	int oticks = zxcpu[cpuid]->GetTStates();
//	printf("Executing: %d\n",ticks);
	return Z80CORE_Execute(zxcpu[cpuid], ticks);	
//	printf("Done!\n");
//	return zxcpu[cpuid]->GetTStates()-oticks;
}


// Call interrupt on a CPU
void CPU_Interrupt(uint8_t cpuid) {

//	printf("Interrupt!\n");
	Z80CORE_SignalInterrupt(zxcpu[cpuid]);

	// do we have to do anything else??
	Z80CORE_Execute(zxcpu[cpuid], 13);

//	printf("Finished Interrupt!\n");
}

// Call an NMI on a CPU
void CPU_NMI(uint8_t cpuid) {
	// TODO
}


void CPU_SetReg(uint8_t cpuid, char *reg, uint16_t value) {

	int creg = reg[0]+reg[1]*256;

	printf("SETREG: %s %d\n",reg, value);

	switch(creg) {
		case 66: // B
			Z80CORE_SetByteRegister(zxcpu[cpuid], eREG_B, value);
			return;
			break;

		case 82: // R
			Z80CORE_SetByteRegister(zxcpu[cpuid], eREG_R, value);
			return;
			break;

		case 73: // I
			Z80CORE_SetByteRegister(zxcpu[cpuid], eREG_I, value);
			return;
			break;
		case 17232: // PC
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_PC, value);
			return;
			break;

		case 27752: // HL' (hl)
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_ALT_HL, value);
			return;
			break;

		case 25956: // DE' (de)
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_ALT_DE, value);
			return;
			break;

		case 25442: // BC' (bc)
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_ALT_BC, value);
			return;
			break;

		case 26209: // AF' (af)
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_ALT_AF, value);
			return;
			break;

		case 19528: // HL
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_HL, value);
			return;
			break;

		case 17732: // DE
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_DE, value);
			return;
			break;

		case 17218: // BC
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_BC, value);
			return;
			break;

		case 22857: // IX
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_IX, value);
			return;
			break;

		case 22601: // IY
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_IY, value);
			return;
			break;

		case 17985: // AF
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_AF, value);
			return;
			break;

		case 20563: // SP
			Z80CORE_SetWordRegister(zxcpu[cpuid], eREG_SP, value);
			return;
			break;


		default: 
			printf("Unimplemented reg [%s] (%d)\n",reg, creg);
			return;

	}

}

uint16_t CPU_GetReg(uint8_t cpuid, char *reg) {

	int creg=reg[0]+reg[1]*256;
//	printf("REG: %s %d\n",reg,creg);
	switch(creg) {
		case 66: // B
			return Z80CORE_GetByteRegister(zxcpu[cpuid], eREG_B);
			break;
		case 17232: // PC
			return Z80CORE_GetWordRegister(zxcpu[cpuid], eREG_PC);
			break;
		default:
			printf("Unimplemented reg return %s\n",reg);
			return 0;
	}
	
	return 0;
}
