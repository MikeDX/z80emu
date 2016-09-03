/* cpuintf.c
 * 
 * CPU Interface (mz80)
 *
 * Allows ZXEM to be used with different cpu cores
 *
 */

#include "zxem.h"
#include "mz80.h"

// create CPU context and return a handle
uint8_t CPU_Create(void) {
	Z80_STATE zxcpu;
}

// reset a CPU context
void CPU_Reset(void) {

}

// Assign a block of memory as read/write
void CPU_MEMRW(void) {

}

// Assign a block of memory as read only
void CPU_MEMR(void) {

}

// Destroy a CPU context
void CPU_Destroy(void) {

}

// Call interrupt on a CPU
void CPU_Interrupt(void) {

}

// Call an NMI on a CPU
void CPU_NMI(void) {
	
}