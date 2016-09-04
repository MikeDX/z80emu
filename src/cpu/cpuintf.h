/* cpuintf.h
 * 
 * CPU Interface
 *
 * Allows ZXEM to be used with different cpu cores
 *
 * Works the same way as OSDEP functions. Add the calls as you need
 *
 */

#include "zxem.h"

uint8_t CPU_Create(void);
void CPU_Reset(uint8_t cpuid);
void CPU_Destroy(uint8_t cpuid);
void CPU_Interrupt(uint8_t cpuid);
void CPU_NMI(uint8_t cpuid);
int CPU_Emulate(uint8_t cpuid, int ticks);
void CPU_SetIMode(uint8_t cpuid, uint8_t mode);
void CPU_SetReg(uint8_t cpuid, char *reg, uint16_t value);
uint16_t CPU_GetReg(uint8_t cpuid, char *reg);


