;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2003
;
;  File        : usb20decstrmac.asm
;  Description : Memory Declaration for USB2.0 Strings Macros
;///////////////////////////////////////////////////////////////////////////////
 IF !@DEF('usb20decstrmac_asm')
    define   usb20decstrmac_asm    '1'

    page    255,255,0

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           UsbAllocateString
;
;   Type:           Macro
;
;   Description:    Extracts a string defined using the DEFINE directive in
;                   an assembly file. The string is unicode compatible.
;                   The macro allocates the memory needed to store the string
;                   and its length (in bytes).
;
;   Inputs:         Usage : SaveRange stringname,Type,"constant string"
;                        
;   Outputs:        
;                   
;   Notes:          
;           Usage: 
;       Begin Code Example 
;    org x,"UsbStrings_X":
;
;
;    DEFINE USBSTRING1 'SigmaTel, Inc.'
;    DEFINE USBSTRING2 'STMP3500'
;    DEFINE USBSTRING3 'Bad String Index'
        
;    UsbAllocateString pbtUsbString1,"USBSTRING1"
;    UsbAllocateString pbtUsbString2,"USBSTRING2"
;    UsbAllocateString pbtUsbString3,"USBSTRING3"
;
;       End Code Example
;
;    For USBSTRING1, this macro creates
;   FpbtUsbString1:
;   pbtUsbString1:
;       DCB     28
;       DCB     'i',0x00,'S'
;       DCB     0x00,'g',0x00
;       DCB     'a',0x00,'m'
;       ....
;        
;<
;///////////////////////////////////////////////////////////////////////////////

UsbAllocateString macro LAB,STRINGTOPACK 
F\LAB: 
LAB: 
    DCB     (@LEN("STRINGTOPACK")*2)
 DUPF NUM,0,@LEN("STRINGTOPACK")-1,3

     if (@LEN("STRINGTOPACK")-NUM-1)<3
       ; Handle Last bytes of the string
       if (@LEN("STRINGTOPACK")-NUM-1)==0
            ; Handle case where one more byte to go
            DCB $0,$0,["STRINGTOPACK",NUM,1]
       else
         if (@LEN("STRINGTOPACK")-NUM-1)==1
                ; Handle case where 2 more bytes to go
                DCB [["STRINGTOPACK",NUM,3],1,1],$0,["STRINGTOPACK",NUM,1]
                DCB $0,$0,$0
         else
                ; Handle case 3 more bytes to go         
                DCB [["STRINGTOPACK",NUM,3],1,1],$0,["STRINGTOPACK",NUM,1]
                DCB $0,[["STRINGTOPACK",NUM,3],2,1],$0
         endif
       endif

     else 
        ; Handle regular bytes
         DCB [["STRINGTOPACK",NUM,3],1,1],$0,["STRINGTOPACK",NUM,1]
         DCB $0,[["STRINGTOPACK",NUM,3],2,1],$0
     endif

 ENDM 
FEnd\LAB:
End\LAB: 
 endm 

  endif ; IF !@DEF('usb20decstrmac_asm')



    
