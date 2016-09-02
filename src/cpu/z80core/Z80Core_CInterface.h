//
// TZT ZX Spectrum Emulator
//

#ifndef __Z80CORE_CINTERFACE_H__
#define __Z80CORE_CINTERFACE_H__

//-----------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

//-----------------------------------------------------------------------------------------

typedef unsigned char(*Z80CoreRead)(unsigned short address, int param);
typedef void(*Z80CoreWrite)(unsigned short address, unsigned char data, int param);
typedef void(*Z80CoreContention)(unsigned short address, unsigned int tstates, int param);

//-----------------------------------------------------------------------------------------

typedef enum
{
	eREG_A,
	eREG_F,
	eREG_B,
	eREG_C,
	eREG_D,
	eREG_E,
	eREG_H,
	eREG_L,

	eREG_ALT_A,
	eREG_ALT_F,
	eREG_ALT_B,
	eREG_ALT_C,
	eREG_ALT_D,
	eREG_ALT_E,
	eREG_ALT_H,
	eREG_ALT_L,

	eREG_I,
	eREG_R,
} eZ80BYTEREGISTERS;

typedef enum
{
	eREG_AF,
	eREG_HL,
	eREG_BC,
	eREG_DE,
	eREG_ALT_AF,
	eREG_ALT_HL,
	eREG_ALT_BC,
	eREG_ALT_DE,

	eREG_IX,
	eREG_IY,
	eREG_SP,
	eREG_PC,

} eZ80WORDREGISTERS;

//-----------------------------------------------------------------------------------------

void *Z80CORE_Create();
void Z80CORE_Destroy(void *core);
void Z80CORE_Initialise(void *core, Z80CoreRead mem_read, Z80CoreWrite mem_write, Z80CoreRead io_read, Z80CoreWrite io_write, Z80CoreContention mem_contention_handling, Z80CoreContention io_contention_handling, int member_class);
void Z80CORE_Reset(void *core);
int Z80CORE_Execute(void *core, int num_tstates);
void Z80CORE_SignalInterrupt(void *core);
unsigned char Z80CORE_GetByteRegister(void *core, eZ80BYTEREGISTERS r);
unsigned short Z80CORE_GetWordRegister(void *core, eZ80WORDREGISTERS r);

//-----------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//-----------------------------------------------------------------------------------------

#endif
