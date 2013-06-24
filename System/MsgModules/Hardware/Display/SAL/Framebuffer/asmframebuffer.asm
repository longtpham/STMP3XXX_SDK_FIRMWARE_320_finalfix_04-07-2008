    include "sysmacro.asm"
    include "const.xref"

    GLOBAL  FSALDisplayCopyBitmapToBitmap


    DEFINE  GetByteFromArray    'GetByteFromArrayInY'
    DEFINE  PutByteInArray      'PutByteInArrayInY'

    extern FPushMask
    extern FPopMask
    extern Fg_iMaskTop
    extern Fg_iMaskBottom
    extern Fg_iMaskRight
    extern Fg_iMaskLeft

    extern  GetByteFromArray
    extern  PutByteInArray

    opt OP
    opt OPSPEED
    opt OPSP

    org y,"Framebuffer_y":
xDestination                    dc  0
yDestination                    dc  0
;bInvert                         n1
;pSource                         r1
;pDestination                    r2
;iSourceWidth                    n5
iSourceHeight                   dc  0
;iDestinationWidth               n6
;iInitialSourceBytePosition      r3
iInitialDestinationBytePosition dc  0
;iSourceBytePosition             r5
;iDestinationBytePosition        r6
;iByteInStripe                   do loop
;iStandardMultiply               r4

iXBytesInStripe                 dc  0
iWholeBytesInStripe             dc  0

iNotTopByteMask             dc  0     
iTopByteMask                dc  0
iBottomByteMask             dc  0
iNotBottomByteMask          dc  0

;state indicator            n2
PARTIAL_TOP_BYTE            equ 15
PARTIAL_BOTTOM_BYTE         equ 14
PUSH_ZERO_INTO_SOURCE       equ 13

;n3,n4 are still free to use in optimization below...

    org p,".ptextframebuffer":

FSALDisplayCopyBitmapToBitmap
    ;a1   xDestination
    ;b1   yDestination
    ;x0   bInvert
    ;r0   pSource
    ;r4   pDestination
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    push SSH


    move a1,y:xDestination
    move b1,y:yDestination

    move x0,n1                          ;store off all values for later

    move y:(r4)+,y1
    move y1,n6                 ;store off the destination width
    move y:(r4)+
    ;push mask
    move y:(r0)+,x0                             ;get the source width
    add   x0,a  x0,n5               ;add it to the xDestination, store off the source width
    move y:(r0)+,y0 
    move    y0,y:iSourceHeight
    move  a1,x0
    add   y0,b      y:xDestination,a
    move  b1,y0
    move  y:yDestination,b

    push    n1
    push    r4                      ;push destination pointer
    push    r0                      ;"    source pointer
    push    n5                      ;"    source width
    push    n6                      ;"    destination width
    jsr     FPushMask
    pop     n6                      ;pop destination width
    pop     n5                      ;"   source width
    pop     r1                      ;"   source pointer
    pop     r2                      ;"   destination pointer
    pop     n1
    ;make sure the destination is within the mask
    move    y:Fg_iMaskRight,y1
    move    y:xDestination,a 
    cmp     y1,a
    jgt     _ExitDisplayCopyFrameBufferToFrameBuffer
    move    n5,x0
    add     x0,a
    move    y:Fg_iMaskLeft,y1
    cmp     y1,a
    jlt     _ExitDisplayCopyFrameBufferToFrameBuffer

    move    y:Fg_iMaskTop,y1
    move    y:yDestination,a 
    cmp     y1,a
    jgt     _ExitDisplayCopyFrameBufferToFrameBuffer
    move    y:iSourceHeight,x0
    add     x0,a
    move    y:Fg_iMaskBottom,y1
    cmp     y1,a
    jlt     _ExitDisplayCopyFrameBufferToFrameBuffer


    ;if we're here, some part of the bitmap must overlap the mask rectangle

_DetermineBitOffset
    ;determine bit offset for the general case 
    move    #>$000007,x1
    move    y:yDestination,a
    and     x1,a  
    move    a1,n0
    move    #Const_008000,r0
    nop 
    lua     (r0)+n0,r4
    
    clr     a
    move    a1,n2               ;clear out our special condition flags

_DetermineTopPartialByteMask
    move    y:Fg_iMaskTop,a     ;get the Fg_iMaskTop, which will be the top bit we put down (since it is no bigger than the current bitmap)
    move    #>$000007,x1        ;see if its byte-aligned
    and     x1,a  
    jeq     _NoTopMask          ;if it is aligned (Fg_iMaskTop&7 == 0), skip out of this.
    bset    #PARTIAL_TOP_BYTE,n2              ;not aligned--set the 'top' partial byte flag
    move    a1,n0               ;set up some stuff to get the shift array multiplier
    move    #Const_008000,r0     ;get the pointer to the shift array
    move    #>$ff,x0            ;load up our nominal mask
    move    y:(r0+n0),y0        ;get the shift multipler
    mpy     x0,y0,a             ;this shifts the noiminal mask over
    and     x0,a                ;now our top partial byte mask is in a1
    move    a1,y:iTopByteMask
    not     a
    and     x0,a
    move    a1,y:iNotTopByteMask
_NoTopMask


_DetermineBottomByteMask
    move    y:Fg_iMaskBottom,a  ;get the Fg_iMaskBottom (since it will always be no bigger than the current bitmap)
    move    #>$000007,x1        ;
    and     x1,a                ;see if its byte aligned
    jeq     _NoBottomMask
    bset    #PARTIAL_BOTTOM_BYTE,n2              ;set the 'bottom' partial byte flag if not aligned
    move    a1,n0               ;move the bitoffset into the index register
    move    #Const_008000,r0     ;get the pointer to the shift array
    move    #>$ff,x0            ;put our nominal shift into x0
    move    y:(r0+n0),y0        ;find out the shift multiplier
    mpy     x0,y0,a             ;shift the mask
    move    a1,y:iNotBottomByteMask
    not     a                   ;we want the negative of that shift
    and     x0,a                ;we just want the lower 8 bits
    move    a1,y:iBottomByteMask
_NoBottomMask


    ;determine starting byte position in source and destination
    move    y:Fg_iMaskLeft,a
    move    y:xDestination,x0 
    sub     x0,a  
    jge     _XDestinationInsideOfLeftMask
    clr     a
_XDestinationInsideOfLeftMask
    move    a1,r3
    add     x0,a
    move    a1,y:iInitialDestinationBytePosition
    move    a1,y1                               ;starting byte position is in y1
    ;find the end position
    move    y:xDestination,b
    move    n5,x0
    add     x0,b                                ;a0 now has the extreme right position
    move    y:Fg_iMaskRight,x1
    cmp     x1,b
    jlt     _XDestinationInsideOfRightMask
    move    x1,b
_XDestinationInsideOfRightMask
    sub     y1,b
    jle     _ExitDisplayCopyFrameBufferToFrameBuffer
    move    b,y:iXBytesInStripe

_FindSourceInitialBytePosition
    ;is the top mask lower than  yDestination?

    move    y:Fg_iMaskTop,a
    move    y:yDestination,x0 
    sub     x0,a
    jgt     _YDestinationInsideOfMask
        clr     a                           ;source
_YDestinationInsideOfMask
    move    a,x0
    mpy     x0,#3,a                         ;a should have the byte count offset
    move    a0,b
    tst b    
    jne _NoPushZeroIntoSource
        bset    #PUSH_ZERO_INTO_SOURCE,n2
_NoPushZeroIntoSource
    move    a1,x0                           ;take this byte count offset, and change the iSourceBytePosition 
    move    n5,y0                           ;n5 is the source width
    mpy     x0,y0,a                         ;multiply the Y byte offset by the source width
    asr     a       r3,x1                   ;get the original byte index due to the X position
    move    a0,a                            ;add them together
    add     x1,a
    move    a1,r3                           ;put the byte offset back into R3

_FindDestinationInitialBytePosition
    move    y:Fg_iMaskTop,x0
    mpy     x0,#3,a                         ;a should have the byte count offset

    move    a1,x0                           ;take this byte count offset, and change the iDestinationBytePosition 
    move    n6,y0
    mpy     x0,y0,a
    asr     a   y:iInitialDestinationBytePosition,x1
    move    a0,a
    add     x1,a
    move    a1,y:iInitialDestinationBytePosition

    
_DetermineWholeBytesInEachYStripe
    move    y:Fg_iMaskTop,x0
    mpy     x0,#3,a
    move    y:Fg_iMaskBottom,x1
    mpy     x1,#3,b
    move    a1,x0
    move    b1,b
    sub     x0,b
    move    b1,y:iWholeBytesInStripe
    

_ForEachVerticalStripe
        ; set up source and destination initial byte positions
        move    r3,r5
        move    y:iInitialDestinationBytePosition,r6

        clr a
        ;preload the top, mask off everything
        jclr    #PUSH_ZERO_INTO_SOURCE,n2,_InitialLoadAndShift
            push    a0
            jmp     _TopByte
_InitialLoadAndShift
        ;get the source byte
        clr     a 
        jset    #1,n1,_continue1
            move    r1,r0
            move    r5,a
            jsr     GetByteFromArray
            move    (r5)+n5     ;increment the source pointer down
        ;invert it or clear it (depending on what we're doing
_continue1
        jclr    #0,n1,_DontInvert1
            not     a   #>$ff,x0
            and     x0,a
_DontInvert1
        ;shift it to the right position
        move    a1,x0
        move    y:(r4),y1
        mpy     x0,y1,b                     ;shift the source byte to the correct position
        push    b1                          ;push it onto the stack.

_TopByte        
        ;get the old saved part to build an entire byte
        clr     a 

        jset    #1,n1,_continue2
            move    r1,r0
            move    r5,a
            jsr     GetByteFromArray
            move    (r5)+n5     ;increment the source pointer down
        ;invert it or clear it (depending on what we're doing
_continue2
        jclr    #0,n1,_DontInvert2
            not     a   #>$ff,x0
            and     x0,a
_DontInvert2
            ;shift it to the right position
        pop     x1                          ;get the old saved part
        move    a1,x0
        move    y:(r4),y1
        mpy     x0,y1,b                     ;shift the source byte to the correct position
        push    b1                          ;push the remainder onto the stack.
        move    y:<Const_000080,x0          ;move the shifted piece to b0, right justified
        move    b0,y0
        mpy     x0,y0,b
        or      x1,b                        ;here we have the entire byte

        jclr    #PARTIAL_TOP_BYTE,n2,_NoTopMaskToApply
            push    b1
            move    r2,r0                       ;get the destination byte
            move    r6,a                        ;
            jsr     GetByteFromArray
            move    y:iTopByteMask,x0           ;get the top mask
            and     x0,a                        ;mask off the destination word
            pop     b                           ;get the old saved byte
            move    y:iNotTopByteMask,x0        
            and     x0,b                        ;mask the part you want to keep
            move    a1,x0
            or      x0,b                        ;or it together

_NoTopMaskToApply
        clr     a
        move    y:iWholeBytesInStripe,a0
        tst     a
        jne _dostripe
        jmp _byteloop
_dostripe
        do a0,_byteloop
_ForEachByte
            move    r2,r0               
            move    r6,a
            jsr     PutByteInArray              ;write it to the destination buffer
            move    (r6)+n6    

            clr     a 

            jset    #1,n1,_continue3
                move    r1,r0
                move    r5,a
                jsr     GetByteFromArray
                move    (r5)+n5     ;increment the source pointer down
            ;invert it or clear it (depending on what we're doing
_continue3
            jclr    #0,n1,_DontInvert3
                not     a   #>$ff,x0
                and     x0,a
_DontInvert3
                ;shift it to the right position
            pop     x1                          ;get the old saved part
            move    a1,x0
            move    y:(r4),y1
            mpy     x0,y1,b                     ;shift the source byte to the correct position
            push    b1                          ;push the remainder onto the stack.
            move    y:<Const_000080,x0          ;move the shifted piece to b0, right justified
            move    b0,y0
            mpy     x0,y0,b
            or      x1,b                        ;here we have the entire byte

_byteloop 
        pop     a                               ;get rid of the old value pushed on
        jclr    #PARTIAL_BOTTOM_BYTE,n2,_NoBottomByteMask
        push    b1
        move    r2,r0                       ;get the destination buffer data so we can or the left over with it
        move    r6,a 
        jsr     GetByteFromArray
        move    y:iBottomByteMask,y1
        and     y1,a
        move    a1,x0
        pop     b1
        move    y:iNotBottomByteMask,y1
        and     y1,b
        or      x0,b
        move    r2,r0               
        move    r6,a 
        jsr     PutByteInArray              ;write it to the destination buffer
_NoBottomByteMask
_DoneWithStripe
        move    (r3)+                                   ;increment the source initial byte over one

        move    y:iInitialDestinationBytePosition,a0        ;incrememnt the destination initial byte over one
        inc     a
        move    a0,r6
        move    a0,y:iInitialDestinationBytePosition

        clr     a                                       ;decrement the bytes in stripe
        move    y:iXBytesInStripe,a0
        dec     a
        move    a0,y:iXBytesInStripe
        tst     a
        jne _ForEachVerticalStripe
    ;ok, we're done.. YAY

_ExitDisplayCopyFrameBufferToFrameBuffer
    ;finish up by popping off the source bitmap mask
    jsr FPopMask

    pop SSH
    nop
    rts
