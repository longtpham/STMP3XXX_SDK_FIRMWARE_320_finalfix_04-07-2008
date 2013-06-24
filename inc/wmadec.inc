;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2006
;
; STMP Wma Wrapper
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(wmadecxref))
Wmadecxref   equ     1

g_WmaDecSinkBufBlockSize equ  $480             ; block size to process Wma audio
g_WmaDecSinkBufSize      equ  $1A00 ;must be in multiples of 256. 0x1A00-[512 (dec) due to largest build lyrics X ram overflow]=0x1800. 0x1a00 is min theoretical sz to avoid underflow for worst case wma encoding. The size of the Wma decode buffer. TOVERIFY in sdk2.6 this was changed from $1400 to 1A00 so a cymbal sounds better for p4s test suite sgtl QA & nstl complaint. We lack the free mem for that. TOVERIFY4 TOVERIFY3
g_WmaDecSinkBufModulo    equ  g_WmaDecSinkBufSize-1 ; The modulo of the Wma decode buffer

g_WmaDataSrcBufBlockSize equ  $40
g_WmaDataSrcBufSize      equ  $1E5
g_WmaDataSrcBufModulo    equ  g_WmaDataSrcBufSize-1


; Error messages. These values are returned by all WMA API functions.
cWMA_NoErr		       	equ	     	0
cWMA_Failed		       	equ	     	1
cWMA_BadArgument	       	equ	     	2
cWMA_BadAsfHeader	       	equ	     	3
cWMA_BadPacketHeader	       	equ	     	4
cWMA_BrokenFrame	       	equ	     	5
cWMA_NoMoreFrames	       	equ	     	6
cWMA_BadSamplingRate	       	equ	     	7
cWMA_BadNumberOfChannels       	equ	     	8
cWMA_BadVersionNumber	       	equ	     	9
cWMA_BadWeightingMode	       	equ	     	10
cWMA_BadPacketization	       	equ	     	11
cWMA_BadDRMType		       	equ	     	12
cWMA_DRMFailed		       	equ	     	13
cWMA_DRMUnsupported	       	equ	     	14
cWMA_DemoExpired	       	equ	     	15
cWMA_BadState		       	equ	     	16
cWMA_Internal                  	equ	     	17
cWMA_NoMoreDataThisTime	       	equ	     	18


; Sampling frequency index values. This directly map to the DACFrequency table

cWMA_SR_08kHz		      	equ	     	0
cWMA_SR_11_025kHz	      	equ	     	1
cWMA_SR_12kHz		      	equ	     	2	; Not supported by WMA
cWMA_SR_16kHz		      	equ	     	3	
cWMA_SR_22_05kHz	      	equ	     	4
cWMA_SR_24kHz		      	equ	     	5	; Not supported by WMA
cWMA_SR_32kHz		      	equ	     	6
cWMA_SR_44_1kHz		      	equ	     	7
cWMA_SR_48kHz		      	equ	     	8
			      		     

    ENDIF  ; IF (!@def(Wmadecxref))

