;////////////////////////////////////////////////////////////////////////////////
;// Copyright(C) SigmaTel, Inc. 2002-2003
;//
;// File        : usb20project.inc
;// Description : This file contains the private defines, externs and
;//          data structure definitions required by the USB2.0 Device Driver
;////////////////////////////////////////////////////////////////////////////////

    IF (!@def(__usb20project_inc__))
__usb20project_inc__    equ     1

;*******************************************************************************
; Maximum Endpoint Number included endpoint 0
;*******************************************************************************
MAX_NUM_EP              equ     (2) 

;*******************************************************************************
; Maximum languages supported for USB strings
;*******************************************************************************
MAX_USB_LANG            equ     (1)

;*******************************************************************************
; Number of strings descriptor for this application 
; (not including string 0 and last string)
;*******************************************************************************
NUM_USB_STRINGS         equ     (2)

;*******************************************************************************
;Size of packet for control endpoints (always 64 FS and HS)
;*******************************************************************************
APP_CONTROL_MAX_PKT_SIZE    equ (64)    

;*******************************************************************************
; Size of packet for any endpoint in full speed mode
;*******************************************************************************
EP_FS_MAX_PACKET_SIZE       equ (64)    

;*******************************************************************************
; Size of packet for any endpoint in high speed
;*******************************************************************************
EP_HS_MAX_PACKET_SIZE       equ (64)   

;*******************************************************************************
; Number of Configurations
;*******************************************************************************
NUMBER_OF_CONFIG			equ	(1)

;*******************************************************************************
; Number of Interfaces, including alternate settings
;*******************************************************************************
CONFIG_DESC_NUM_INTERFACES 	equ	(1)

;*******************************************************************************
; STMP_USB20_DMA_BASE_ADD is the address to convert addresses from the USB2.0 
; ARC IP space into the STMP space. The address is specified as the STMP
; base address in 24-bit words. To insure that both address spaces are aligned
; on the byte 0 of the 1st word, this address must be a multiple of 4.
; The ARC USB2.0 space is 32bit (4 bytes), where the STMP is a 24bit (3bytes).
; The bytes 0 are aligned on both spaces every 4 STMP words.
;
; Equation to convert ARC address into STMP address (1st byte is byte 0,
; and 1st word is word 0)
; STMP_ADD_WORD = integer(ARC_ADD / 3) + STMP_USB20_DMA_BASE_ADD
; STMP_ADD_BYTE_OFF = ARC_ADD - (3 * integer(ARC_ADD / 3))  
;
; Equation to convert STMP address into ARC address (1st byte is byte 0,
; and 1st word is word 0)
; ARC_ADD = (STMP_ADD_WORD * 3)
;*******************************************************************************
;ASM Only
    WARN '***** STMP_USB20_DMA_BASE_ADD must be a multiple of 4 *******'
;endasm
    
 if (@def('DUSE_EEPROM'))
    
    extern  F_lc_u_eX_USB20_BASE_ADDRESS

STMP_USB20_DMA_BASE_ADD     equ     (F_lc_u_eX_USB20_BASE_ADDRESS)
;    define      STMP_USB20_DMA_BASE_ADD     'F_lc_u_eX_USB20_BASE_ADDRESS'
 endif

        ENDIF ;IF (!@def(__usb20project_inc__))
