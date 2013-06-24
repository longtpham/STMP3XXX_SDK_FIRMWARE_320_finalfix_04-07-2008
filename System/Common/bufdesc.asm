;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2003
;
;  File        : budesc.asm
;  Description : STMP3 Buffer Routines (System Routines) 
;                See buffer descriptor data structure.
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_BufDesc

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "hwequ.inc"
    include "resource.inc"
    list       

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions
    global  InitBufferEmpty
    global  InitBufferFull
    global  InitBufferSize
    global  FillBufferY
    global  GetFilledSpace
    global  GetEmptySpace
    global  FGetEmptySpace
        
    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    ; WMA specific code
    global  FillBufferX
    endif

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "const.xref"
    list       
    
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

    org p,"SYSFUN_BufDesc_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InitBufferEmpty
;
;   Type:           Function
;
;   Description:    Sets the buffer head = tail = beginning of buffer
;                   
;   Inputs:         r0 = pointer to the buffer descriptor
;                   m0 = linear addressing
;
;   Outputs:        r0 = pointer to the buffer descriptor
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
InitBufferEmpty

    move    #>BD_BufferBaseAddress,n0
    nop
    move    y:(r0+n0),r2                ; get the buffer BASE ADDRESS
    move    #>BD_HdPtr,n0
    nop
    move    r2,y:(r0+n0)                ; reset the head pointer back to the base address
    move    #>BD_TailPtr,n0
    nop
    move    r2,y:(r0+n0)                ; reset the tail pointer back to the base address
    rts
                                    
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InitBufferFull
;
;   Type:           Function
;
;   Description:    Sets the buffer head = end of buffer, tail = beginning of buffer
;
;   Inputs:         r0 = pointer to the buffer descriptor
;                   m0 = linear addressing
;
;   Outputs:        r0 = pointer to the buffer descriptor
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
InitBufferFull

    move    #>BD_BufferBaseAddress,n0
    nop
    move    y:(r0+n0),x0                ; get the buffer BASE ADDRESS
    move    #>BD_BufferModulo,n0
    nop
    move    y:(r0+n0),x1                ; get the buffer MODULO
    move    #>BD_HdPtr,n0
    clr     a
    move    x0,a1
    add     x1,a                        ; a has base + modulo = last location in buffer
    move    a1,y:(r0+n0)                ; set the head pointer to the last location
    move    #>BD_TailPtr,n0
    nop
    move    x0,y:(r0+n0)                ; set the tail pointer to the base address
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InitBufferSize
;
;   Type:           Function
;
;   Description:    Moves head "n0" buffer locations away from tail
;
;   Inputs:         r0 = pointer to the buffer descriptor
;                   m0 = linear addressing
;                   n0 = size to initialize buffer to
;
;   Outputs:        r0 = pointer to the buffer descriptor
;                   
;   Notes:         
;<
;///////////////////////////////////////////////////////////////////////////////
InitBufferSize

    move    n0,x0
    move    #>BD_BufferSize,n0
    nop
    move    y:(r0+n0),x1                ; get the buffer SIZE


    move    #>BD_TailPtr,n0
    clr     a
    move    y:(r0+n0),a1                ; get the TAIL PTR
    add     x0,a    #BD_BufferBaseAddress,n0 ; a = "SIZE"+TAILptr

    clr     b
    move    y:(r0+n0),b1
    move    b1,y1                       ; save copy
    add     x1,b                        ; b = bufSIZE+BASE

    cmp     a,b
    jgt     _NoModuloCorrect
    sub     b,a
    add     y1,a
_NoModuloCorrect
    move    #>BD_HdPtr,n0
    nop
    move    a1,y:(r0+n0)                ; HEAD PTR is "size" away from TAIL PTR
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FillBufferY
;
;   Type:           Function
;
;   Description:    Move head "n0" buffer locations away from tail, then fills 
;                   with "y1" value into Y memory from tail to head
;
;   Inputs:         r0 = pointer to the buffer descriptor
;                   m0 = linear addressing
;                   n0 = size to initialize buffer to
;                   y1 = value to initialize buffer to
;
;   Outputs:        r0 = pointer to the buffer descriptor
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
FillBufferY
    move    n0,x0                       ; store "size" to init
    move    #>BD_BufferSize,n0
    nop
    move    y:(r0+n0),x1                ; get the buffer SIZE

    move    #>BD_TailPtr,n0
    clr     a
    move    y:(r0+n0),a1                ; get the TailPtr
    move    a1,r2                       ; store for FILL part (r2=TailPtr)
    add     x0,a    #BD_BufferBaseAddress,n0 ; a = "size"+TailPtr

    clr     b
    move    y:(r0+n0),b1
    move    b1,n2                       ; store buffer base address (n2=BaseAddr)
    add     x1,b                        ; b = bufSIZE+BASE
    cmp     a,b
    jgt     _NoModuloCorrect
_ModuloCorrection
    sub     b,a
    move    a1,y0                       ; store distance past "end" of buffer
    move    n2,x1                       ; BASE addr
    add     x1,a    #>BD_HdPtr,n0       ; BASE + overlap
    nop
    move    a1,y:(r0+n0)                ; HEAD PTR is "size" away from TAIL PTR

    move    x0,b                        ; "size" to init
    sub     y0,b                        ; b = distance to the "end"
    move    b,x1                        ; x1 = distance TO end of buffer

    do      x1,_fill_to_end
      move    y1,y:(r2)+
      nop
_fill_to_end

    move    n2,r2                       ; point to BASE address
    move    y0,b 
    tst     b
    jeq     _exit_fillY                 ; avoid do #0
    do      y0,_fill_from_beginning
      move    y1,y:(r2)+
      nop
_fill_from_beginning
    nop
    rts
_NoModuloCorrect
    move    #>BD_HdPtr,n0
    nop
    move    a1,y:(r0+n0)                ; HEAD PTR is "size" away from TAIL PTR
    nop
    do      x0,_end_fill_loop
      move    y1,y:(r2)+
      nop
_end_fill_loop
    nop
_exit_fillY
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FillBufferX
;
;   Type:           Function
;
;   Description:    Move head "n0" buffer locations away from tail, then fills 
;                   with "y1" value into X memory from tail to head
;
;   Inputs:         r0 = pointer to the buffer descriptor
;                   m0 = linear addressing
;                   n0 = size to initialize buffer to
;                   y1 = value to initialize buffer to
;
;   Outputs:        r0 = pointer to the buffer descriptor
;                   
;   Notes:          Never used
;<
;///////////////////////////////////////////////////////////////////////////////

    if (!(@def('DEVICE_3400')||@def('DEVICE_3300')))
    ; WMA specific code
FillBufferX
    move    n0,x0                       ; store "size" to init
    nop
    move    #>BD_BufferSize,n0
    nop
    move    y:(r0+n0),x1                ; get the buffer SIZE

    move    #>BD_TailPtr,n0
    clr     a
    move    y:(r0+n0),a1                ; get the TailPtr
    move    a1,r2                       ; store for FILL part (r2=TailPtr)
    add     x0,a                        ; a = "size"+TailPtr

    move    #BD_BufferBaseAddress,n0
    clr     b
    move    y:(r0+n0),b1
    move    b1,n2                       ; store buffer base address (n2=BaseAddr)
    add     x1,b                        ; b = bufSIZE+BASE
    cmp     a,b
    jgt     _NoModuloCorrect
_ModuloCorrection
    sub     b,a
    move    a1,y0                       ; store distance past "end" of buffer
    move    n2,x1                       ; BASE addr
    add     x1,a                        ; BASE + overlap
    move    #>BD_HdPtr,n0
    nop
    move    a1,y:(r0+n0)                ; HEAD PTR is "size" away from TAIL PTR

    move    x0,b                        ; "size" to init
    sub     y0,b                        ; b = distance to the "end"
    move    b,x1                        ; x1 = distance TO end of buffer

    do      x1,_fill_to_end
      move    y1,x:(r2)+
      nop
_fill_to_end

    move    n2,r2                       ; point to BASE address
    do      y0,_fill_from_beginning
      move    y1,x:(r2)+
      nop
_fill_from_beginning
    nop
    rts
_NoModuloCorrect
    move    #>BD_HdPtr,n0
    nop
    move    a1,y:(r0+n0)                ; HEAD PTR is "size" away from TAIL PTR
    nop
    do      x0,_end_fill_loop
      move    y1,x:(r2)+
      nop
_end_fill_loop
    nop
    nop
    rts
    endif

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           GetFilledSpace
;
;   Type:           Function
;
;   Description:    Returns in accumulator a the filled space (head-tail) fixed 
;                   for proper modulus of buffer
;
;   Inputs:         r0 = pointer to the buffer descriptor
;                   m0 = linear addressing
;
;   Outputs:        count in a
;                   r0 = pointer to the buffer descriptor
;
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
GetFilledSpace
    move    #>BD_BufferSize,n0
    nop
    move    y:(r0+n0),y0                ; get the buffer SIZE
    move    #>BD_HdPtr,n0
    nop
    move    y:(r0+n0),a                 ; a=head pointer
    move    #>BD_TailPtr,n0
    nop
    move    y:(r0+n0),x0                ; x0=tail pointer
    sub     x0,a                        ; a=head-tail
    ; using jge here means that head=tail is an empty buffer
    jge     _NoModuloCorrect
    add     y0,a                        ; perform modulo correction
_NoModuloCorrect
    nop

    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           GetEmptySpace (or FGetEmptySpace(void*ptr))
;
;   Type:           Function
;
;   Description:    Returns in accumulator a the empty space (tail-head) fixed 
;                   for proper modulus of buffer (and decremented by one)
;
;   Inputs:         r4 = pointer to the buffer descriptor 
;                   m4 = linear addressing
;                   (r0 for FGetEmptySpace)
;
;
;   Outputs:        count in a
;                   r0 = pointer to the buffer descriptor
;                   y0 = total buffer size
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
FGetEmptySpace
    move    r0,r4
GetEmptySpace
    move    #>BD_BufferSize,n4
    nop
    move    y:(r4+n4),y0                ; y0= buffer descriptor's SIZE value
    move    #>BD_HdPtr,n4
    nop
    move    y:(r4+n4),x0                ; x0=head pointer
    move    #>BD_TailPtr,n4
    nop
    move    y:(r4+n4),a                 ; a=tail pointer
    sub     x0,a                        ; a=tail-head
    ; using jgt here is consistent with GetFilled space, head==tail is empty, not full
    jgt     _NoModuloCorrect
    add     y0,a                        ; perform modulo correction using buffer size
_NoModuloCorrect                
    ; we never want to return a count that would allow the 
    ; processing routine to fill the buffer to head=tail
    ; so always decrement the result by 1
    move    y:<Const_ffffff,x0
    add     x0,a
    rts

    endsec
