;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : hwequ.inc
;  Description : STMP Hardware Constants
;///////////////////////////////////////////////////////////////////////////////

; ////////////////////////////////////////////////////////////////////////////////
; hwequ is being eliminated for SDK3XXX.  For SDK2.5XX it will now reside in the   
; local folder instead of ..\inc.  All register includes are converted to .h by 
; registers.mk placed in the ..\lcdexample\player\output_XXXX folder.
; ////////////////////////////////////////////////////////////////////////////////

    if (!@def(HWEQU_INC))
HWEQU_INC equ 1
;ASM Only
	include "regscd.inc"
	include "regsclkctrl.inc"
	include "regscodec.inc"
	include "regscore.inc"
	include "regsdcdc.inc"
	include "regsecc.inc"
	include "regsemc.inc"
	include "regsespi.inc"
	include "regsfilco.inc"
	include "regsgpflash.inc"
	include "regsgpio.inc"
	include "regsi2c.inc"
	include "regsi2s.inc"
	include "regsicoll.inc"
	include "regslradc.inc"	
	include "regspwm.inc"
	include "regsrevision.inc"
	include "regsrtc.inc"
	include "regssdram.inc"
	include "regsspare.inc"
	include "regsspi.inc"
	include "regsswizzle.inc"
	include "regssystem.inc"
	include "regstb.inc"
	include "regstimer.inc"
	include "regsusb20.inc"
	include "regsusb20phy.inc"
;endasm

;C insert
;#include "types.h"
;#include "regsclkctrl.h"
;#include "regscore.h"
;#include "regscodec.h"
;#include "regsdcdc.h"
;#include "regsemc.h"
;#include "regsgpio.h"
;#include "regsi2c.h"
;#include "regsi2s.h"
;#include "regsicoll.h"
;#include "regslradc.h"
;#include "regspwm.h"
;#include "regsrevision.h"
;#include "regsrtc.h"
;#include "regsspare.h"
;#include "regsspi.h"
;#include "regsswizzle.h"
;#include "regssdram.h"
;#include "regstb.h"
;#include "regstimer.h"
;#include "regsusb20.h"
;#include "regsusb20phy.h"
;endc
                             

    endif ; if (!@def(hwequ))
