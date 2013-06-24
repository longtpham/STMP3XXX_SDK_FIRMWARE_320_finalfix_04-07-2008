#ifndef _LFN_H
#define _LFN_H

#include "filesystem.h"
#include "types.h"		//UCS3 typedef


//this is a c struct for the assembly struct DIRLfn in fcb_y.asm
typedef struct{
	unsigned int m_wSignature;			//ordinal field
	WORD m_wFileName1[4];		//first part of Unicode String
	int		m_wReserved;				//Null Terminator
	int 	m_wAttributes;				//should always be 0x0F
	//int 	m_wFlags;					//TBD
	//int 	m_wCheckSum;				//checksum based on 8.3 filename
	WORD m_wFileName2[4];		//second part of the filename
	int		m_wReserved2;				//Null Terminator
	WORD m_wFileName3[2];		//third part of the filename
	int		m_wReserved3;				//Null Terminator
} LFN;

typedef struct {
	int wFCBEntry;
	BYTE bDevice;
}LFN_FILE_LOCATOR;

extern _asmfunc FILE_CONTROL_BLOCK* FSHNDLGetPointer(int handle);
WORD _reentrant GetUnicodeCharacter(WORD *source, int charNumber);
RETCODE _reentrant LFNConstructFileName(LFN_FILE_LOCATOR* LFNFileLocator,UCS3* Buffer);
extern _asmfunc void FSReadLFNRecord(BYTE iDevice,unsigned int iRecord);
 

#endif