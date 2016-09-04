//
// TZT ZX Spectrum Emulator
//

#include <iostream>
#include "Z80Core_CInterface.h"
#include "Z80Core.h"

//-----------------------------------------------------------------------------------------

extern "C"
{

//-----------------------------------------------------------------------------------------

void *Z80CORE_Create()
{
	return reinterpret_cast<void *>(new CZ80Core());
}

//-----------------------------------------------------------------------------------------

void Z80CORE_Destroy(void *core)
{
	delete reinterpret_cast<CZ80Core *>(core);
}

//-----------------------------------------------------------------------------------------

void Z80CORE_Initialise(void *core, Z80CoreRead mem_read, Z80CoreWrite mem_write, Z80CoreRead io_read, Z80CoreWrite io_write, Z80CoreContention mem_contention_handling, Z80CoreContention io_contention_handling, int member_class)
{
	reinterpret_cast<CZ80Core *>(core)->Initialise(mem_read, mem_write, io_read, io_write, mem_contention_handling, io_contention_handling, member_class);
}

//-----------------------------------------------------------------------------------------

void Z80CORE_Reset(void *core)
{
	reinterpret_cast<CZ80Core *>(core)->Reset();
}

//-----------------------------------------------------------------------------------------

int Z80CORE_Execute(void *core, int num_tstates)
{
	return reinterpret_cast<CZ80Core *>(core)->Execute(num_tstates);
}

//-----------------------------------------------------------------------------------------

void Z80CORE_SignalInterrupt(void *core)
{
	reinterpret_cast<CZ80Core *>(core)->SignalInterrupt();
}

//-----------------------------------------------------------------------------------------

unsigned char Z80CORE_GetByteRegister(void *core, eZ80BYTEREGISTERS r)
{
	return reinterpret_cast<CZ80Core *>(core)->GetRegister((CZ80Core::eZ80BYTEREGISTERS)r);
}

//-----------------------------------------------------------------------------------------

unsigned short Z80CORE_GetWordRegister(void *core, eZ80WORDREGISTERS r)
{
	return reinterpret_cast<CZ80Core *>(core)->GetRegister((CZ80Core::eZ80WORDREGISTERS)r);
}

//-----------------------------------------------------------------------------------------

void Z80CORE_SetByteRegister(void *core, eZ80BYTEREGISTERS r, unsigned char data) {
	reinterpret_cast<CZ80Core *>(core)->SetRegister((CZ80Core::eZ80BYTEREGISTERS)r, data);
}

//-----------------------------------------------------------------------------------------

void Z80CORE_SetWordRegister(void *core, eZ80WORDREGISTERS r, unsigned short data) {
	reinterpret_cast<CZ80Core *>(core)->SetRegister((CZ80Core::eZ80WORDREGISTERS)r, data);
}

//-----------------------------------------------------------------------------------------

void Z80CORE_SetIMMode(void *core, unsigned char mode) {
	reinterpret_cast<CZ80Core *>(core)->SetIMMode(mode);
}


//-----------------------------------------------------------------------------------------

}