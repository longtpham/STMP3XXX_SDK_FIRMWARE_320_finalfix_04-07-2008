;******************************************************************************
; Copyright(C) SigmaTel, Inc. 2000
;
; STMP Foreground Module Table
;******************************************************************************
        
;*******************************************************************************
;  Include files
;*******************************************************************************
		include "dacmemy.xref"        
;*******************************************************************************
;  Extern definitions
;*******************************************************************************
		extern 	CopytoEffectsBuffer
		extern 	EmptyFunction	
		extern 	GeqProcess
		extern 	GeqInit
		extern  DacDriverProcess
		extern	EffectsDescriptor
    if @def('SPECTRUM_ANAL')		
        extern  SpectralAnalProcess
    endif
;*******************************************************************************
;  Global definitions
;*******************************************************************************
        global	DecoderOutputBufferType
		global 	DacSourceBufDesc
		global	DacSinkBufDesc
		global 	CopyBufSourceDesc
		global 	CopyBufSinkDesc
		global  ModuleControlBlockBase
		global	FMADCModuleBase
		global 	CopyModuleBase
		global 	GeqModuleBase
		global 	DacModuleBase
		global 	NUM_FGND_MODULES
  if @def('WOW')
		global 	FGeqModuleBase
  endif
;*******************************************************************************
;  X memory
;*******************************************************************************

;*******************************************************************************
;  Y memory
;*******************************************************************************
  if @def('WOW')
		extern 	WowProcess
        global 	FWowModuleBase
  endif

  if @def('SPECTRUM_ANAL')
    if @def('WOW')
NUM_FGND_MODULES	equ 5
    else
NUM_FGND_MODULES    equ 4
    endif
  else
    if @def('WOW')
NUM_FGND_MODULES    equ 4
    else
NUM_FGND_MODULES    equ 3
    endif
  endif 

	org     y,"FgndModuleTable":

; Place this module here, if we need an equalizer for FM playback
; ADC driver for FM input.
; FM module must reset it back to EmptyFunction, if not used.
; Set the descriptors to something valid for timebeing. Does not do anything.
ModuleControlBlockBase
FMADCModuleBase
; 			dc			1						; Module Enable Flag always 1
; 			dc			0						; IOBufIdenticalFlag
; 			dc			DacBufDescriptor		; Source buffer descriptor
; 			dc			DacBufDescriptor		; Sink Buffer descriptor
; 			dc			EmptyFunction			; Routine
; 			dc			0						; Parameter0
; 			dc			0						; Parameter1

; This is the normally the first working module
; Copies decoder/ADC output to a WorkBuffer. For non-compatible 
; output buffer decoders.
CopyModuleBase
			dc			1						; Module Enable Flag always 1
			dc			0						; IOBufIdenticalFlag
CopyBufSourceDesc
			dc			0						; Source buffer descriptor. Written by CopyBufferInit
CopyBufSinkDesc
			dc			EffectsDescriptor		; Sink Buffer descriptor
			dc			CopytoEffectsBuffer		; Routine
DecoderOutputChannelType
			dc			0						; 0 = Mono, 1 = Stereo.
DecoderOutputBufferType
			dc			0						; 0 = X memory, 1 = Y memory.

; Place all the other effects' modules below.
  if @def('WOW')
FGeqModuleBase
  endif
GeqModuleBase
			dc			0						; Module Enable Flag / disable by default. Saves 4 hrs AA batt life for nonscroll songs.
			dc			1						; IOBufIdenticalFlag
			dc			EffectsDescriptor		; Source buffer descriptor
			dc			EffectsDescriptor		; Sink Buffer descriptor
			dc			GeqProcess				; Routine
			dc			0						; Parameter0
			dc			0						; Parameter1
 if (@def('WOW'))
FWowModuleBase
			dc			0						; Module Enable Flag / disable by default
			dc			1						; IOBufIdenticalFlag
			dc			EffectsDescriptor		; Source buffer descriptor
			dc			EffectsDescriptor		; Sink Buffer descriptor
			dc			WowProcess			    ; Routine
			dc			0						; Parameter0
			dc			0						; Parameter1
 endif

;SPECTRUM_ANAL is defined or undefined in config.mk
    if @def('SPECTRUM_ANAL')
SpectralAnalBase
			dc			1						; Module Enable Flag
			dc			1						; IOBufIdenticalFlag
			dc			EffectsDescriptor		; Source buffer descriptor
			dc			EffectsDescriptor		; Sink Buffer descriptor
			dc			SpectralAnalProcess				; Routine
			dc			0						; Parameter0
			dc			0						; Parameter1
    endif

; This is the last module in the list
DacModuleBase
			dc			1						; Module Enable Flag
			dc			0						; IOBufIdenticalFlag
DacSourceBufDesc
			dc			EffectsDescriptor		; Source buffer descriptor
DacSinkBufDesc
			dc			DacBufDescriptor		; Sink Buffer descriptor
			dc			DacDriverProcess		; Routine
			dc			0						; Parameter0
			dc			0						; Parameter1

