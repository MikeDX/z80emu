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
#include "Z80Core.h"

#define MAX_CPU 10

CZ80Core *zxcpu[MAX_CPU];
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
		zxcpu[num_cpus]=new CZ80Core;
		zxcpu[num_cpus]->Initialise(coreRead,coreWrite,IORead,IOWrite,coreContention,coreContention,num_cpus);
		return num_cpus;
	} 

	return 255;
}

// reset a CPU context
void CPU_Reset(uint8_t cpuid) {
	zxcpu[num_cpus]->Reset();
}

// Destroy a CPU context
void CPU_Destroy(uint8_t cpuid) {
	delete(zxcpu[cpuid]);
	zxcpu[cpuid]=NULL;
}

// Emulatoe virtual CPU
int CPU_Emulate(uint8_t cpuid, int ticks) {
	return zxcpu[cpuid]->Execute(ticks);	
}


// Call interrupt on a CPU
void CPU_Interrupt(uint8_t cpuid) {

	zxcpu[cpuid]->SignalInterrupt();

	// do we have to do anything else??
	zxcpu[cpuid]->Execute(13);

}

// Call an NMI on a CPU
void CPU_NMI(uint8_t cpuid) {
	// TODO
}


uint16_t CPU_GetReg(uint8_t cpuid, char *reg) {

	int creg=reg[0]+reg[1]*256;
	switch(creg) {
		case 66: // B
			return zxcpu[cpuid]->GetRegister(CZ80Core::eREG_B);
			break;
		case 17232: // PC
			return zxcpu[cpuid]->GetRegister(CZ80Core::eREG_SP);
			break;
		default:
			printf("Unimplemented reg return %s\n",reg);
			return 0;
	}
	
	return 0;
}
