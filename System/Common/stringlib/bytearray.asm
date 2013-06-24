
    include "const.xref"
    include "sysmacro.asm"

    

    ;This is kind of messy, but in order to make sure that both X and Y versions of this code are always in sync and
    ;correct, they are defined with this macro.
    ;
    ;DefineGetByteFromMemorySpace <X|Y|P>   will define the code and section for X, Y, or P to get a byte from a packed array
    ;DefinePutByteToMemorySpace <X|Y|P>     will define the code and section for X, Y, or P to put a byte into a packed array

    ;There are two sets of macros so that each function can be broken up into separate files, so the lib can include only
    ;the functions it needs to.

    ;NOTE:
    ;IMPORTANT:
    ;REALLY IMPORTANT:  These functions do not corrupt r0,1,2,3,4,5,6 or n1,2,3,4,5,6 and there are functions out there that
    ;                   depend on this fact.  DO NOT CHANGE THIS!

OneThird equ 0.333333333333

    ;
    ; A contains the byteCounter  (It will not work if A2 is non zero!)
    ; return value: A1 contains the result. 
    ; A contains the byteCounter

DefineGetByteFromMemorySpace macro MemSpace

    opt OP
    opt OPSPEED
    opt OPSP


    GLOBAL  GetByteFromArrayIn\MemSpace
    org p,".GetByteFromArrayIn\MemSpace\^_P":

GetByteFromArrayIn\MemSpace:
    clr B     A,x0       #>OneThird,y1                
    mpy  x0,y1,A  y:<Const_200000,x0
    move A0,B1
    lsr  B  A1,n0                    ; Avoid sign.
    cmp  x0,B #>$ff,y1
    jlt ^_CaseA                   
    move y:<Const_400000,x0
    cmp x0,B MemSpace:(r0+n0),x0
    jlt ^_CaseB
    ; Case C follows.
    ; In this case data is in upper 8 bits
^_CaseC
    mpy #16,x0,A  
    and y1,A
    move    A1,A
    rts
 
^_CaseB    ; Case B: Middle byte
    mpy  #8,x0,A 
    and y1,A  
    move    A1,A
    rts
 
^_CaseA
    ;Case A
    ;Lower Byte
    move    MemSpace:(r0+n0),A
    and y1,A
    move    A1,A
    rts

    endm

DefinePutByteInMemorySpace macro MemSpace

    opt OP
    opt OPSPEED
    opt OPSP

    GLOBAL PutByteInArrayIn\MemSpace
    org p,".PutByteInArrayIn\MemSpace\^_P":

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; R0 is the pointer to the buffer in Memspace
; A1 contains the byte index
; B1 contains the byte value
; return value: none 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PutByteInArrayIn\MemSpace:
    move    b1,y0                       ;store off the new value in y0
    clr     B       A,x0       #>OneThird,y1                
    mpy     x0,y1,A y:<Const_200000,x0
    move    A0,B1
    lsr     B       A1,n0                    ; Avoid sign.
    cmp     x0,B    #>$FFFF00,y1
    jlt ^_CaseA                   
    move    y:<Const_400000,x0
    cmp     x0,B    #>$FF00FF,y1
    jlt ^_CaseB
    ; Case C follows.
    ; In this case data is in upper 8 bits
^_CaseC
    move    #>$00FFFF,y1        ;load in the proper mask    
    move    MemSpace:(r0+n0),b         ;get the original value
    mpy     #8,y0,A             ;the value to write is now in the upper 8 bits of a0.  No need to worry about the lower 16 bits of a0
    and     y1,b    a0,x1       ;mask off the original value, move the value to write into x1
    or      x1,b                ;or the value to insert with the masked original value.
    move    b1,MemSpace:(r0+n0)        ;put the combined value into the array
    rts
 
^_CaseB    ; Case B: Middle byte
    move    MemSpace:(r0+n0),b         ;mask is already loaded, load up the original value
    and     y1,b                ;mask off the new word
    mpy     #16,y0,A            ;shift the read value to its proper position (though its ends up in A0)
    move    a0,a                ;move it to a1
    move    #>$00ff00,y1        ;mask off everything but the new byte
    and     y1,A   b1,x0         ;mask the new value, and move the original value into x0
    or      x0,a                ;or the new and the original
    move    a1,MemSpace:(r0+n0)        ;move the combined value to memory
    rts
 
^_CaseA
    ;Case A
    ;Lower Byte
    move    MemSpace:(r0+n0),b         ;get the original value
    and     y1,b        y0,a    ;mask off the lower 8 bits, and move the new value into a
    move    #>$FF,y1            ;set up the mask
    and     y1,a   b1,x0        ;make sure we have nothing but the lower 8 bits
    or      x0,a                ;or them together
    move    a1,MemSpace:(r0+n0)        ;store them away
    rts

    endm
