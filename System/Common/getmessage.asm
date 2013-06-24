    include "project.inc"
    include "sysmacro.asm"

    opt op

    GLOBAL GetMessage
    extern Const_000001

    org p,"GetMessage_P":
IsMessageInQueue
;R0 contains a pointer to the Queue Descriptor
    move (r0)+                                  ;skip past the queue pointer
    move (r0)+                                  ;get the module
    move (r0)+                                  ;skip past the size
    move y:(r0)+,a                              ;get the head pointer
    move y:(r0)+,b                              ;get the tail pointer
    sub  b,a                                    ;a will be non-zero if there is a message.
    rts

;R0 contains a pointer to the Queue Descriptor
;R4 contains a pointer to the structure to copy the message to.
GetMessage
    push r0
    jsr IsMessageInQueue
    pop r0
    tst a
    jeq _NoMessage
    move (r0)+                                  ;skip past the queue pointer
    move y:(r0)+,m1                             ;get the modulo
    move (r0)+                                  ;skip past the size
    move y:(r0)+                                ;skip past the head pointer
    move y:(r0)+,r1                             ;get the tail pointer
    move r4,b                                   ;move the ptr to copy to to b for testing
    tst b y:(r1)+,a1                            ;test, and get the first word.
    jeq _error                                  ;null pointer passed in for MSG!
    do a1,_enddo                                ;do for each
        move a1,y:(r4)+                         ;copy the value you have
        move y:(r1)+,a1                         ;get the next one (this will walk past the end of this message)
_enddo 
    move (r1)-                                  ;it increments the ptr one too many, so decrement the pointer by one
    move m7,m1                                  ;m7 is always -1!, and we must set any modulo back to linear,
    move r1,y:-(r0)                             ;put the message queue tail ptr back
    move y:<Const_000001,a                      ;ensure a is non-zero
_NoMessage
    rts
_error
    ;A null pointer has been passed in. 
    error
