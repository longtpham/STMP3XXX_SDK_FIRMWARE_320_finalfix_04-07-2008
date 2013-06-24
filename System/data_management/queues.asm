
    include "const.xref"

    global  QueueGetFilledSpace
    global  QueueGetEmptySpace

    org p,".ptextqueues":

QueueGetFilledSpace
;       Takes the CircularBufferDescriptor in R0
;       returns with filled length in A
;                CircularBufferDescriptor in R0
    move    y:<Const_000001,y1  
    move    y:(r0)+,a
    move    y:(r0)+,x1
    move    y:(r0)-,b
    sub     b,a y:(r0)-
    jge     _exit
    add     x1,a                ;add on the modulo plus 1 (the size of the buffer)
    add     y1,a
_exit
    rts


QueueGetEmptySpace
;       Takes the CircularBufferDescriptor in R0
;       returns with empty space in A
;                CircularBufferDescriptor in R0
    move    y:<Const_000001,y1  
    move    y:(r0)+,b
    move    y:(r0)+,x1
    move    y:(r0)-,a
    sub     b,a y:(r0)-  
    jgt     _exit
    add     x1,a                ;add on the modulo plus 1 (the size of the buffer)
    add     y1,a
_exit
    sub     y1,a 
    rts
