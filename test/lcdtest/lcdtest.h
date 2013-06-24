////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: lcdtest.h
// Description: Tests to verify the functionality of the LCD
////////////////////////////////////////////////////////////////////////////////

#ifndef _LCDTEST_H
#define _LCDTEST_H

extern int _reentrant DisplayModuleInit(struct ModuleEntry *pEntry);
extern struct ModuleEntry *ModuleTableDisplay;
// Functions used to test different messaging APIs of the LCD


extern INT g_iPDM;
extern INT g_iSDMS;

extern   int S_cached_PDM_SEEK;
extern   int S_cached_SDMs_SEEK;
extern   int S_cached_PGM_SEEK;
extern   int S_cached_SGMs_SEEK;
extern   int S_cached_Script_00_SEEK;
extern   int S_cached_Scripts_SEEK;

void _reentrant TestLcdPrintRange(void);
void _reentrant TestLcdPrintString(void);
void _reentrant TestLcdScrollDisplay(void);
void _reentrant TestLcdContrast(void);
void _reentrant TestLcdSetFont(void);
void _reentrant TestLcdCodepage(void);
void _reentrant TestLcdPrintNumber(void);
void _reentrant TestLcdPrintTime(void);

#pragma asm
	
	
	org    y,"lcdTest_Y":

	;global stlogo
	;global Fstlogo
    
Fstlogo
stlogo  dc      $000060   ; 96 pixels wide
        dc      $000020   ; 32 pixels tall
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $c08000
        dc      $e1e0e0
        dc      $efe7e3
        dc      $ffffff
        dc      $f3fbff
        dc      $e3e3e3
        dc      $e3e3e3
        dc      $e3e3e3
        dc      $e3e3e3
        dc      $e7e3e3
        dc      $276fef
        dc      $000303
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $4040c0
        dc      $404040
        dc      $0000c0
        dc      $000000
        dc      $000080
        dc      $000000
        dc      $800000
        dc      $808080
        dc      $808080
        dc      $000000
        dc      $000000
        dc      $000080
        dc      $000000
        dc      $038100
        dc      $ff0f07
        dc      $f8bcfe
        dc      $40e0f0
        dc      $008000
        dc      $000000
        dc      $000000
        dc      $404040
        dc      $40c040
        dc      $404040
        dc      $000000
        dc      $800000
        dc      $808080
        dc      $008080
        dc      $000000
        dc      $800000
        dc      $000000
        dc      $000000

        dc      $000000
        dc      $12121b
        dc      $121212
        dc      $00001e
        dc      $000000
        dc      $00001f
        dc      $000000
        dc      $1f0000
        dc      $101010
        dc      $1d1414
        dc      $000000
        dc      $000000
        dc      $06031f
        dc      $c68c0c
        dc      $78ffe3
        dc      $ff1e3c
        dc      $0307ff
        dc      $0e1811
        dc      $0b090b
        dc      $10180e
        dc      $000000
        dc      $000000
        dc      $001f00
        dc      $000000
        dc      $000000
        dc      $1f0000
        dc      $161616
        dc      $001010
        dc      $000000
        dc      $1f0000
        dc      $101010
        dc      $001010

        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $100000
        dc      $1e1c18
        dc      $1b1f1f
        dc      $181819
        dc      $ff1818
        dc      $18187f
        dc      $181818
        dc      $181818
        dc      $181818
        dc      $1e1c1c
        dc      $18181c
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
        dc      $000000
#pragma endasm

#endif // #ifndef _LCDTEST_H
