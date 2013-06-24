    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    global  strchr

    org p,"strchr":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strchr
;
;  Type: Function
;  Description: _asmfunc BYTE _Y *_Y strchr(char _Y * _Y pUnpacked, WORD CharToFind)
;               returns a pointer to the substring in pUnpacked that starts with ChartoFind.
;               if CharToFind is not in pUnpacked, strchr returns NULL.
;
;  Inputs: 
;          BYTE _Y * _Y pDest   (r0):   unpacked buffer destination string
;           BYTE CharToFind     (a1):   character to search for
;  Outputs:
;           (r0)                    :   pointer to substring
;  Notes:   ascii and Unicode-3 friendly.  
;           Does not create a new buffer
;<
;///////////////////////////////////////////////////////////////////////////////
strchr
   
_topofloop
    move    y:(r0)+,b
    cmp     a,b                     
    jeq     _exit
    tst     b                       ;is a 0 (end of string?)
    jeq     _not_found              ;yes, then we've reached the end of the string and not found CharToFind
    jmp     _topofloop              ;lets keep searching
_not_found
    move    y:<Const_000001,r0      ;lets put 1 in r0 so when it decrements, it will be NULL
    nop
_exit
    move    (r0)-                   ;this backs off the increment to R0 when reading
    rts
