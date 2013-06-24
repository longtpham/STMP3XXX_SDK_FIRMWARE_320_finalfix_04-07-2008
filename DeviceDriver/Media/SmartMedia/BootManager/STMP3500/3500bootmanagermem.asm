;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2002-2003
;
;  File        : 3500bootmanager.asm
;  Description : 
;///////////////////////////////////////////////////////////////////////////////

    page    132,60,0,0,0

;*******************************************************************************
;  Include files
;*******************************************************************************
        
;*******************************************************************************
;  Extern definitions
;*******************************************************************************

    global  InitRoutine,ReceiveRoutine,ExitRoutine,InitCount,InitPtr,StoreCSR        
    global  ErrorRoutine
    global  CheckSum   
    global  CheckSumTarget
    global  CipherKey
    
    global  BootyVersion  
    global  WordCount

    global  FlashSinkHeadPtr,FlashSinkTailPtr,FlashLagniappe,FlashPageState         
    global  FlashSaveY1,FlashPageAddressLow,FlashPageAddressHigh,FlashPageBuf,FFlashPageBuf           
    global  FlashSinkBuf,FlashProgress            

    global  SmLookForBlock        
    global  SmPageSize            
    global  SmFullPageSize
    global  NandBlockNum
	global  FNandBlockNum
    global  NandSectorNum    
    global  FNandSectorNum    
    global  NandCurDevNum    
    global  FNandCurDevNum    
    global  pCurrentNANDDescriptor
    global  FpCurrentNANDDescriptor
	global	FCurrentNANDBusWidth
	global	CurrentNANDBusWidth

    global  SmBlockSize
    global  SmBlockMax           
    global  SmDataStatusOffset    
    global  SmDataStatusMask      
    global  SmDataStatusShift     
    global  SmBlockStatusOffset   
    global  SmBlockStatusMask     
    global  SmBlockStatusShift    
    global  SmTag0Offset          
    global  SmTag0Mask            
    global  SmTag0Shift           
    global  SmTag1Offset          
    global  SmTag1Mask            
    global  SmTag1Shift           
    global  SmCr                  
    global  FlashInitSaveY1
	global  SmBlockTag
    global  FSmBlockTag

    global  NandPortInit
    global  NandWritePortCmd
    global  NandWritePortAddr
    global  NandReadPortData

    global  TimeOutInitHigh
    global  TimeOutInitLow
    global  TimeOutHigh      
    global  TimeOutLow       

    global  BootInfo
    global  SmCard0Info 
    global  SmCard1Info 
    global  SmCard2Info 
    global  SmCard3Info 
    global  SmCardInfo
    global  UsbInfo    
    
    global  LmemSaveHigh

    global  BOOT_RSRV_SIZE
    global  BOOT_RSRV_START
	global  BOOT_RSRV_END
    
    global  BOOT_FLASH_BUF_SIZE
    global  BOOT_FLASH_BUF_START
	global  BOOT_FLASH_BUF_END
    
    global  BOOT_TEMP_RSRV_SIZE

    global  SM_NUM_PARAMS

    global  BOOT_ATTEMPT               
    global  BOOT_SUCCESS               
    global  INVALID_COMMAND            
    global  SM_END_OF_CARDS

    global  BOOT_INFO_TYPE            
    global  BOOT_INFO_ATTEMPT         
    global  BOOT_INFO_SUCCESS         
    global  BOOT_INFO_INVALID_COMMAND 
    global  BOOT_INFO_VERSION         
    global  BOOT_INFO_TIMEOUT
    global  BOOT_INFO_CHECKSUM

    global  SM_INFO_END_OF_CARDS      
    global  SM_INFO_LARGE_MEDIA_FLAG       
    global  SM_INFO_16BIT_BUS_FLAG         
    global  SM_INFO_LARGE_ADD_FLAG         
    global  SM_INFO_SSFDC_CMD_FLAG     

    global  USB_INFO_USB_DETECTED    
    global  USB_INFO_USB_DISCONNECTED
    global  USB_INFO_PLL_TIMEOUT      
    global  USB_INFO_BOOT_TIMEOUT

	global  TIMEOUT_SLOW_HIGH 
    global  TIMEOUT_SLOW_LOW  
    global  TIMEOUT_USB_HIGH  
    global  TIMEOUT_USB_LOW
    
    if      ("FLH"=="SMFLH")
        global  FlashVectorTest,FlashVectorResult,FlashVectorFaddr,FlashFakeReadPort        
    endif
    
    global  USB_SINKBUFPTR,USB_SINKHEADPTR,USB_SINKTAILPTR
    global  UsbPageState,UsbLagniappe,UsbEp1IrqPending,UsbProgress
    global  UsbSaveY1,UsbTimeOutCount
    
    if      ("USB"=="SMUSB")
        global  UsbVectorTest,UsbVectorResult,UsbVectorFaddr,UsbFakeReadPort        
        global  USB_DBG_BYTECOUNT,USB_DBG_INPORT
    endif
    
    
;*******************************************************************************
;  Extern references
;*******************************************************************************


BOOT_ATTEMPT                        equ     0
BOOT_SUCCESS                        equ     1
INVALID_COMMAND                     equ     2
SM_END_OF_CARDS                     equ     3

BOOT_INFO_TYPE                      equ     23
BOOT_INFO_ATTEMPT                   equ     22
BOOT_INFO_SUCCESS                   equ     21
BOOT_INFO_INVALID_COMMAND           equ     20
BOOT_INFO_VERSION                   equ     19
BOOT_INFO_TIMEOUT                   equ     18
BOOT_INFO_CHECKSUM                  equ     17

SM_INFO_END_OF_CARDS                equ     0
SM_INFO_LARGE_MEDIA_FLAG            equ     1
SM_INFO_16BIT_BUS_FLAG              equ     2
SM_INFO_LARGE_ADD_FLAG              equ     3
SM_INFO_SSFDC_CMD_FLAG              equ     4

USB_INFO_USB_DETECTED               equ     0
USB_INFO_USB_DISCONNECTED           equ     1
USB_INFO_PLL_TIMEOUT                equ     2
USB_INFO_BOOT_TIMEOUT               equ     3

TIMEOUT_SLOW_HIGH                   equ     0   ; 1 turns out to be around 20s
TIMEOUT_SLOW_LOW                    equ     $200000
TIMEOUT_USB_HIGH                    equ     (4*3)+1 ; tweaked to 5 minutes
TIMEOUT_USB_LOW                     equ     0

USB_SINKBUFSIZE                     equ     ($0040)
USB_SINKMODULO                      equ     (USB_SINKBUFSIZE-1)

FLASH_PAGE_BUF_SIZE             equ     (450)  ; the next 100 words are overlaid with the usb mem that directly follows this section.  TOVERIFY review this

    org     x,"Flash__X",OVERLAY,SCRATCH:

BOOT_RSRV_START                 equ     *

Boot_Reserve_Start

FNandBlockNum
NandBlockNum        ds  1

FNandSectorNum
NandSectorNum           ds  1

FNandCurDevNum
NandCurDevNum           ds  1

FpCurrentNANDDescriptor
pCurrentNANDDescriptor  ds  1

; Bus Width for some NANDs can not be determined because
; their ID/MFR codes are not unique (same for 8/16 bit versions)
; For such a NAND the determination is done thru the READ ID2
; command or the extended ID. The DataBusWidth entry in NAND descriptor
; must be re-initialized during the process of determining current NAND.
; The problem here is that NAND descriptor is in P memory and for ROM 
; code we cannot re-init this entry. The solution was to created this 
; globabl variable and use it in code in place of the struct DataBusWidth
; entry.
FCurrentNANDBusWidth
;CurrentNANDBusWidth		dc	8
CurrentNANDBusWidth		ds	1

FBootInfo
BootInfo                ds  1

SaveCommand             ds  1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ExitRoutine         ds  1
ReceiveRoutine      ds  1 
InitRoutine         ds  1
ErrorRoutine        ds  1

InitCount           ds  1
InitPtr             ds  1
StoreCSR            ds  1

CheckSum            ds  1
CheckSumTarget      ds  1
CipherKey           ds  1

BootyVersion        ds  1
WordCount           ds  1

TimeOutInitHigh     ds  1
TimeOutInitLow      ds  1
TimeOutHigh         ds  1
TimeOutLow          ds  1

SmCard3Info         ds  1
SmCard2Info         ds  1
SmCard1Info         ds  1
SmCard0Info         ds  1

; Sm Flags
; 0     - EndOfCards
;           0   False
;           1   True
; 1-16  - Reserved
; 17-22 - See BootInfo def
; 23    - Reserved
SmCardInfo          ds  1

; USB flags
; 0     - USB Detected
;           0   False
;           1   True
; 1     - USB Disconnected during operation
;           0   False
;           1   True
; 2     - PLL Timeout
;           0   False
;           1   True
; 3     - PLL Timeout
;           0   False
;           1   True
; 4-16  - Reserved
; 17-22 - See BootInfo def
; 23    - Reserved
UsbInfo             ds  1

LmemSaveHigh        ds  1

;*******************************************************************************
; Flash defs and USB defs (overlay)
;*******************************************************************************
USB_SINKHEADPTR
FlashSinkHeadPtr        ds  1     

USB_SINKTAILPTR
FlashSinkTailPtr        ds  1

UsbLagniappe
FlashLagniappe          ds  1

UsbPageState
FlashPageState          ds  1

UsbEp1IrqPending
FlashSaveY1             ds  1

UsbProgress
FlashPageAddressLow     ds  1

UsbTimeOutCount
FlashPageAddressHigh    ds  1

FlashProgress           ds  1

SmLookForBlock          ds  1   ; If clear, the current page number is not        
                                ; at block boundary                                          

SmPageSize              ds  1             
SmFullPageSize          ds  1
SmBlockSize             ds  1   
SmDataStatusOffset      ds  1
SmDataStatusMask        ds  1
SmDataStatusShift       ds  1
SmBlockStatusOffset     ds  1
SmBlockStatusMask       ds  1
SmBlockStatusShift      ds  1
SmBlockMax              ds  1
SmTag0Offset            ds  1
SmTag0Mask              ds  1
SmTag0Shift             ds  1
SmTag1Offset            ds  1
SmTag1Mask              ds  1
SmTag1Shift             ds  1

SM_NUM_PARAMS           equ     SmTag1Shift-SmPageSize+1           

SmCr                    ds  1
FlashInitSaveY1         ds  1
FSmBlockTag
SmBlockTag              ds  1

    ; Those are the HAL functions for the NAND ports
NandPortInit            ds  1     
NandWritePortCmd        ds  1
NandWritePortAddr       ds  1
NandReadPortData        ds  1

UsbSaveY1               ds  1

FFlashSinkBuf
FlashSinkBuf            ds 1

; Flash buffer is split into 2 pieces.  The USB Sink buffer uses modulo
; arithmetic so it needs to be on a 0x40 boundary.  The first portion of
; flash memory is overlaid with the Usb Sink Buffer.  The second portion
; is sized for the remainder (FlashPageBuf2 below).

; Flash buffer should actually be much larger (705 words = 2112 bytes) but
; we're locating this before the USB buffers so the data will continue into
; the USB buffer which is 268 bytes.  268 + 450 = 718 words.
;USB_SINKBUFPTR
FFlashPageBuf
FlashPageBuf            dsm  (USB_SINKBUFSIZE)

; Force USB Sink Buffer to point to appropriate location in middle of 
; FlashPageBuf. Only 16 words are required but the modulo operation 
; requires alignment on 16 word boundary. 
USB_SINKBUFPTR          equ FlashPageBuf

Boot_Reserve_End

BOOT_RSRV_END       equ     *
;BOOT_RSRV_SIZE      equ     (Boot_Reserve_End-Boot_Reserve_Start)
BOOT_TEMP_RSRV_SIZE      equ     (Boot_Reserve_End-Boot_Reserve_Start)


; We'll assume the previous section and this section are contiguous.
    org     x,"Flash_Buf_X",OVERLAY,SCRATCH:
    
BOOT_FLASH_BUF_START    equ     *    
Boot_FlashBuf_Start   

; Flash buffer should actually be much larger (705 words = 2112 bytes) but
; we're locating this before the USB buffers so the data will continue into
; the USB buffer which is 268 bytes.  268 + 450 = 718 words.
FFlashPageBuf2
FlashPageBuf2         ds  (FLASH_PAGE_BUF_SIZE-USB_SINKBUFSIZE)

Boot_FlashBuf_End

BOOT_FLASH_BUF_END       equ     *
BOOT_FLASH_BUF_SIZE      equ     (Boot_FlashBuf_End-Boot_FlashBuf_Start)

; Update the Boot Reserve size so that both Flash__X and Flash_Buf_X are cleared.
BOOT_RSRV_SIZE      equ     (BOOT_TEMP_RSRV_SIZE+BOOT_FLASH_BUF_SIZE)

