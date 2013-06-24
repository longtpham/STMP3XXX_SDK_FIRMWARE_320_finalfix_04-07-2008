    nolist
    include 'sysmacro.asm'
    include 'const.xref'
    list

    OPT OP

    global  strrchr
    extern  strlen

    org p,"strrchr":
;///////////////////////////////////////////////////////////////////////////////
;> Name: strrchr
;
;  Type: Function
;  Description: _asmfunc BYTE _Y *_Y strrchr(char _Y * _Y pUnpacked, WORD CharToFind)
;               Finds the last occurrence of "CharToFind" character in a string.
;               returns a pointer to the substring in pUnpacked that starts with ChartoFind.
;               searches backwards in pUnpacked to find it
;               if CharToFind is not in pUnpacked, strrchr returns NULL.
;
;  Inputs: 
;          BYTE _Y * _Y pUnpacked   (r0):   unpacked buffer destination string
;          BYTE CharToFind          (a1):   character to search for
;  Outputs:
;          (r0)                         :   pointer to substring OR 0 if not found
;  Notes:   ascii and Unicode-3 friendly.  
;           Does not create a new buffer
;<
;///////////////////////////////////////////////////////////////////////////////
strrchr
    push    a1                      ; char to look backwards for
    push    r0                      ; ptr to string
    jsr     strlen                  ; get length of the string pointed to by r0
    pop     r0
    move    a1,n0                   ; length of string
    move    r0,x0                   ; when r0 = x0, we are done searching string (reached beginning)
    move    y:(r0)+n0,b             ; r0 points to end of string
    move    y:-(r0),b               ; b = last character
    clr     a
    pop     a1                      ; a1 = char we are searching for
_topofloop
    cmp     a,b                     
    jeq     _exit
    move    r0,b
    cmp     x0,b
    jeq     _not_found              ;yes, then we've reached the end of the string and not found CharToFind
    move    y:-(r0),b
    jmp     _topofloop              ;lets keep searching
_not_found
    move    y:>Const_000000,r0      ; return error
_exit
    rts
