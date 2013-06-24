;  SigmaTel Inc
;  $Archive:  $                                        
;  $Revision:  $                                       
;  $Date:  $                                           
;  Description: platform.asm
;  Notes:	This file provides APIs to work on different platforms. For 24 bit STMP34x, it is best
;			done in assembly.
; ----------------------------------------------------------------------------*/

		Section  StmpPlatformAPICHKD

;****************************************************************************************************************
;  Include files
;***************************************************************************************************************
    include "sysmacro.asm"
    include "const.xref"

; Use relative paths only


;***********************************************************************************************************************
;	Global definitions
;***************************************************************************************************************
;        global  FSGetByte
;        global  FSGetWord
;        global  FSGetDWord
;        global  FFSGetByte
;        global  FFSGetWord
;        global  FFSGetDWord
;        global  PutByte
;        global  FPutByte
;        global  PutWord
;        global  FPutWord
;        global  PutDword
;        global  FPutDword
		global  GetDWordY
		global  FGetDWordY

;**************************************************************************************************************
;	Code Start
;**************************************************************************************************************
  
	org p,".pStmpPlatformAPI_CHKD":
;///////////////////////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FSGetByteX
;
;   Type:           Function
;
;   Description:    Get one Byte From X Memory
;
;   Inputs:         r0: Ptr to Buffer
;					A : Offset
;                   B : Memory Type
;
;   Outputs:        return Value in A
;
;   Registers Trashed : Assume all except r7
;                   
;   Notes:
;           
;<
;///////////////////////////////////////////////////////////////////////////////////////////////

    comment ~
FFSGetByte
FSGetByte
;    0 = X memory
;    2 = Y memory
;    4 = P memory
    ; Divide Offset By 3
    move #>$2aaaab,y0
    move A,x0
    mpy  x0,y0,B       B,n1
    ;Get the Buffer Offset
    move B,n0
    move y:<Const_200000,y0
    move #Xmemoryget,r1
    move (r0)+n0
    ;x0=Buffer[Offset]
    jsr  (r1+n1)
    tfr  x1,A           y:<Const_000000,B1
    asr  B              #>$0000ff,x0
    move B0,B
    cmp  y0,B           y:<Const_400000,y0
    jlt  _End
    cmp  y0,B           y:<Const_000080,B
    move y:<Const_008000,y0
    tlt  y0,B
    move B,y0
    mpy  x1,y0,A
_End
    and  x0,A
    move A1,A
    rts
;///////////////////////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FSGetWord
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         r0: Ptr to Buffer
;					A : Offset
;                   B : Memory Type        
;
;   Outputs:        return Value in A
;
;   Registers Trashed : Assume all except r7
;                   
;   Notes:
;<
;///////////////////////////////////////////////////////////////////////////////////////////////

FFSGetWord
FSGetWord
    move (r7)+
    move y:<Const_ffffff,m3
    move A,r3
    move r0,r2
    push B
    jsr  FSGetByte
    move A,y1       
    move (r3)+
    move r3,A
    move r2,r0
    pop  B
    jsr  FSGetByte
    move A,x0
    mpy  x0,#16,A
    move A0,A
    or   y1,A       (r7)-
;    move (r7)-
    rts

;///////////////////////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FSGetDWord
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         r0: Ptr to Buffer
;					A : Offset
;
;   Outputs:        return Value in A1:A0
;
;   Registers Trashed : Assume all except r7
;                   
;   Notes:
;<
;;///////////////////////////////////////////////////////////////////////////////////////////////
FFSGetDWord
FSGetDWord
    move (r7)+
    move y:<Const_ffffff,m3
    move A,r3
    move r0,r2
    push B
    jsr  FSGetByte
    move A,y1       
    move (r3)+
    move r3,A
    move r2,r0
    pop  B
    push B
    jsr  FSGetByte
    move A,x0
    mpy  x0,#16,A
    move A0,A
    or   y1,A       (r3)+
    move A,y1
;    move (r3)+
    move r3,A
    move r2,r0
    pop  B
    push B
    jsr  FSGetByte
    move A,x0
    mpy  x0,#8,A
    move A0,A
    or   y1,A       (r3)+
    move A,y1
;    move (r3)+
    move r3,A
    move r2,r0
    pop  B
    jsr  FSGetByte
    move y1,A0
    move (r7)-
    rts
;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;
;>  Name:           PutByte
;
;   Type:           Function
;
;   Description:    Put one Byte to X Memory
;
;   Inputs:         r0: Ptr to Output Buffer
;					A : Byte to put
;                   B : Offset
;                   x0 : Memory Type
;
;   Outputs:        return Value in A
;
;   Registers Trashed : Assume all except r7
;                   
;   Notes:   
;<
;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPutByte
PutByte
    ;Devide Offset By 3
    move #>$2aaaab,y0
    move B,x1
    mpy  x1,y0,B        x0,n1
;    move x0,n5
    ;Get the Buffer Offset
    move B,n0
    move y:<Const_200000,y0
    move (r0)+n0
    move #Xmemoryget,r1
    move #Xmemoryput,r5
    jsr  (r1+n1)
;    move x:(r0),x1
    move y:<Const_000000,B1
    asr  B              A1,y1
    move B0,B
;    cmp  y0,B           y:<Const_400000,y0
;    jlt  _End
;    cmp  y0,B           #>$00ffff,A1
;    jgt  _Check
;    mpy  y1,#16,B
;    move #>$ff00ff,A1
;    and  x1,A           B0,y1
    cmp  y0,B           y:<Const_400000,y0
    move #>$ffff00,A
    jlt  _End
    cmp  y0,B           #>$00ffff,y0
    move #>$ff00ff,A
    tgt  y0,A
    move y:<Const_008000,B
    move y:<Const_000080,y0
    tlt  y0,B
    move B,y0
    mpy  y1,y0,B        
    move B0,y1
_End
    and  x1,A       x0,n5
    or   y1,A
    jsr  (r5+n5)
    rts


    
;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;
;>  Name:           PutWord
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         r0: Ptr to Output Buffer
;					A : Byte to put
;                   B : Offset
;
;   Outputs:        return Value in A
;
;   Registers Trashed : Assume all except r7
;                   
;   Notes:   
;<
;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPutWord
PutWord
    move (r7)+
    move y:<Const_ffffff,m3
    move B,r3
    move #>$ff,x1
    push A
    and  x1,A       r0,r2
    move A1,A
    jsr  PutByte
    move (r3)+
    move r3,B
    pop  A
    move A,x1
    mpy  x1,#8,A
    move #>$ff,x1
    and  x1,A       r2,r0
    move A1,A
    jsr  PutByte
    move (r7)-
    rts
;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;
;>  Name:           PutDword
;
;   Type:           Function
;
;   Description:    
;
;   Inputs:         r0: Ptr to Output Buffer
;					A : Byte to put
;                   B : Offset
;
;   Outputs:        return Value in A
;
;   Registers Trashed : Assume all except r7
;                   
;   Notes:   
;<
;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FPutDword
PutDword
    move (r7)+
    move y:<Const_ffffff,m3
    move B,r3
    move r0,r2
    move #>$ff,x1
    push A0
    push A1
    move A0,A
    and  x1,A
    move A1,A
    jsr  PutByte
    move (r3)+
    move r3,B
    pop  A
    pop  A0
    push A0
    push A1
    move A0,A
    move A,x1
    mpy  x1,#8,A
    move #>$ff,x1
    and  x1,A       r2,r0
    move A1,A
    jsr  PutByte
    move (r3)+
    move r3,B
    pop  A
    pop  A0
    push A1
    move A0,A
    move A,x1
    mpy  x1,#16,A
    move #>$ff,x1
    and  x1,A       r2,r0
    move A1,A
    jsr  PutByte
    move (r3)+
    move r3,B
    pop  A
    move #>$ff,x1
    and  x1,A       r2,r0
    jsr  PutByte
    move (r7)-
    rts

Xmemoryget    
    move x:(r0),x1
    rts
Ymemoryget
    move y:(r0),x1
    rts    
Pmemoryget
    move p:(r0),x1
    rts

Xmemoryput
    move A1,x:(r0)
    rts
Ymemoryput
    move A1,y:(r0)
    rts    

Pmemoryput
    move A1,p:(r0)
    rts

    ;endsec

  ~


FGetDWordY
GetDWordY
    move (r7)+
    ; Divide Offset By 3
    move #>$2aaaab,y0
    move A,x0
    mpy  x0,y0,B       B,n1
    ;Get the Buffer Offset
    move B,n0
    move y:<Const_200000,y0
    move (r0)+n0
    ;x0=Buffer[Offset]
	move #0,B1
    asr  B              #>$0000ff,x0
    move B0,B
    cmp  y0,B           y:<Const_400000,y0
    jlt  _state0
    cmp  y0,B           y:<Const_000080,y0
;    move y:<Const_008000,y0
    jlt _state1
	clr B
    move y:(r0)+,x0
    mpy x0,#16,A
	move #>$ff,y0
	and y0,A        y:(r0)+,x0
	move A1,B0
	mpy x0,#16,A
	and y0,A
	move #0,A2
	add B,a
	
;	  move y:(r0)+,A0
;      move y:(r0),A1
;      rep #>16
;      asr A
;     ;rts

    move (r7)-
    rts
    


_state0

    move y:(r0)+,A0
	move y:(r0),A1
	move #>$0000FF,x0
	and x0,A
	move (r7)-
	rts


_state1
;	clr B
;	move y:(r0)+,x0
;	mpy x0,#8,A
;	move #$ffff,x0
;	and x0,A 
;	move A1,B0
;	move y:(r0)+,x0
;   ;	move x1,x0
;	mpy x0,#8,A
;	move #$ff,x0
;	and x0,A
;	move #0,A2
;	add B,A
;    move (r7)-
;	rts

   move y:(r0)+,A0
   move y:(r0),A1
   move #>$00FFFF,x0
   and  X0,A			 
   rep #>8
   asr A
   move (r7)-
   rts

	;   move y1,A0
;   move x0,A1
;   move x0,B0
;   move #>$00FFFF,x0
;   and  X0,A					B0,x0
;   rep #>8
;   asr A
;   rts


	endsec