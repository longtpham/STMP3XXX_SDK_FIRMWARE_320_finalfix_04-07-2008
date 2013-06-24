;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision: 3 $
;$Date: 8/07/00 6:37p $
;Description: 
;Notes:	
;		
;***********************************************************************/

; The scaling factor of the feedforward path. 
scalingfactor				equ		0.25

; BLOCKSIZE: The numnber  of samples consecutively processed before a gain update.
BLOCKSIZE					equ		`16


MAX_CHANNELS				equ		2

NumberOfBands				equ		5

GEQBUFSIZE					equ  	256


SimInL          			equ     $8000
SimInR          			equ     $8001
SimOutL         			equ     $8000
SimOutR         			equ     $8001
