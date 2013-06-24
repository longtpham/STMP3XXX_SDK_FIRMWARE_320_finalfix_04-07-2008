;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : sysvoice.asm
;  Description : Routines to find voice recording
;		    InitVoiceNumber must be in system memory
;		    All other files are in application memory
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_SysVoice

    page    132,60,0,0,0
    opt     mex

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    include "getdevicevalue.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "systemdefs.inc"
    include "hwequ.inc"
    include "errors.inc"
    include "directorydef.inc"
    include "resource.inc"
    include "systemdefs.inc"
    include "sysresources.inc"
    include "project.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

;    global    FindFirstVoice
;    global    FindLastVoice
;    global    FindNextVoice
;    global    FindPrevVoice
;    global    FindNextVoiceNumber
    global    CreateVoiceFilename
;    global    InitVoiceNumber
    global    VoiceDirCreated
    global    Fg_iVoiceDirCreated

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include   "const.xref"
    include   "sysresource.xref"
    list

    extern    CP_GetByte
    extern    CP_SetByte

 comment ~
    extern    SysCountSongs
    extern    SysFindSong
    extern    CreateDIR
    extern    ChangeDirectory
    extern    ReadDIRRecord
    extern    DIRRecName
    extern    DIRRecExt
    extern    DIRRecAttr
    extern    SysAllLinear
    extern    DCB_FCB
    extern    RootDirectory
    extern    GetDevVal_DCB
    extern    SysFlushCoderMemory
    extern    EncCurrentDeviceNum
    extern    NextVoiceNumValue
    extern    FileSystemMode
 ~
    ; locator generated labels used when loading resources
;    extern    F_lc_e_WriteFAT12_p
;    extern    F_lc_e_WriteFAT12_x
;    extern    F_lc_e_WriteFAT12_y
;    extern    F_lc_b_WriteFAT12_p
;    extern    F_lc_b_WriteFAT12_x
;    extern    F_lc_b_WriteFAT12_y
    extern    F_lc_u_bP_directory
    extern    F_lc_u_eP_directory
    extern    F_lc_u_bP_sys_cluster

 if (@DEF('FUNCLET'))           
    extern    SysCallFunclet
 endif


;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

 if !(@DEF(MAX_VOICE_NUM))
MAX_VOICE_NUM               equ     4000000
 endif ;!(@DEF(MAX_VOICE_NUM))
 if !(@DEF(DIGITS_IN_VOICE_FILENAME))
DIGITS_IN_VOICE_FILENAME    equ 7
 endif ;!(@DEF(DIGITS_IN_VOICE_FILENAME))



;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////

    org y,"SYSFUN_SysVoice_Y":

ExtList         dc      1               ; Only one extension
                dc      "VAW"           ; "WAV" extension
        
voicedir        dc  'OV'++DirectorySeparator,'ECI',DirectorySeparator,0 ; "/VOICE/"
voicepath       dc  'OV'++DirectorySeparator,'ECI',0       ; "/VOICE" - for creating DIR

voicenameprefix dc  'OV'++DirectorySeparator,'ECI',DirectorySeparator,0 ; "/VOICE/"
voicenamesuffix dc  'AW.',$000056       ;  ".WAV"

maxvoicenumber  dc  0   

dirsave         dc  0

Fg_iVoiceDirCreated            
VoiceDirCreated             dc  0       ;0 -> Voice directory not created
                                        ;1 -> Voice directory created

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org p,"DIRAPP_SysVoice_P":


;  HOW TO USE THIS:
;
;   move    #VOICE_DEVICE,a2
;   jsr FindNextVoiceNumber ; Create number for unique filename: "\VOICE\Vnnnnnnn.WAV"
;   jcc _NameOK
;
;   <handle error - couldn't come up with a new voice number>
;
;_NameOK
;   move    a1,y:NextVoiceNumber    ; The return from FindNextVoiceNumber
;
;
;   ; DO THE CALL ABOVE ONCE, AT STARTUP OR WHEN YOU GO INTO RECORD MODE.
;   ; THEN HOLD ONTO THE RESULT AND JUST INCREMENT IT EACH TIME YOU CREATE
;   ; A FILE.  YOU ONLY NEED TO RERUN FIndNextVoiceNumber WHEN FILES MAY
;   ; HAVE BEEN ADDED OR DELETED BY SOMEBODY ELSE.
;
;   ...
;
;   move    y:NextVoiceNumber,x0    ; Ready to create a file
;   move    #fname_str,a1       ; A1 = address of string to receive filename
;   bset    #22,a1
;   push    a1          ; save complex pointer to filename string
;
;   jsr CreateVoiceFilename
;
;   pop a1          ; set up FILECreate
;   move    #VOICE_DEVICE,a2
;   jsr FILECreate      ; Create the file
;   jcc _Create_OK
;
;   <handle error - file create failed>
;
;_Create_OK
;   <write stuff to file>
;
;   move    a1,a2           ; Put file handle in A2
;   jsr FILEClose       ; Close the file
;
;   move    y:NextVoiceNumber,a ; Increment this for next create.
;   move    #>1,x0          ; (You should really check that it 
;   add x0,a            ; doesn't go over 4,000,000, due to
;   move    a,y:NextVoiceNumber ; a rounding problem above that.)







 comment ~


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FindNextVoiceNumber
;
;   Type:           Function
;
;   Description:    Scan the specified volume's "\VOICE" directory and determine the
;                   next number to use for generating a unique filename.
;
;   Inputs:         a2: Device Number
;
;   Outputs:        If successful:
;                       sr.C: Clear
;                       a1: NextNumber
;                   If unsuccessful:
;                       sr.C: Set
;                       A = Error Number
;
;   Notes:          Creates the "\VOICE" directory on device, if it doesn't
;                   already exist.
;
;                   Voice filenames take the form
;                       Vnnnnnnn.WAV
;                   where "nnnnnnn" is a zero-padded decimal number.
;
;                   Adapted from SysCountSongs
;<
;///////////////////////////////////////////////////////////////////////////////
;   PSEUDOCODE:
;   maxnumber = -1
;   for (each directory record) {
;       if (end of directory)
;           break
;       else if (not a file)
;           next
;       else if (no "V" at start)
;           next
;       else if (no ".WAV" at end)
;           next
;       number=0
;       for (idx=1;idx;<8;idx++) {
;           digit = fname[idx] - '0'
;           if ((digit < 0) || (digit > 9))
;               next record
;           number = 10*number + digit
;       }
;       if (number > maxnumber)
;           maxnumber = number
;   }
;
;   A = maxnumber + 1   # returned value
;   if (maxnumber >= 0)
;       return OK
;   else
;       return FAIL
; 
;
;*******************************************************************************
FindNextVoiceNumber

    ; Save parameters
    push    a2
    move    a2,y:dirsave

    move    #0,a1
    move    a1,y:maxvoicenumber         ; MAX = 0

    ; Ensure all address registers are linear
    jsr     SysAllLinear


_ChangeDIR
    ; NEED TO SET A1 to point at "\VOICE\"
    move    #>voicedir,a1
    bset    #22,a1                      ; Y: space

    ; Change to the directory specified by the directory path
    jsr     ChangeDirectory             ; A2=Device number, A1=Pointer to path
    jcc     _SetupFCBPointers

    ; Can't change directory - need to create it, first.
    move    #>voicepath,a1
    bset    #22,a1                      ; Y: space
    pop     a2
    push    a2
    jsr CreateDIR
    jcc _ChangeDIR

    ; Creation of directory failed.  So sorry...
    ; Balance the data stack
    pop     a2                          ; Device number
    move    #>ERROR_CREATING_VOICE_DIR,a1
    jmp _LeaveWithError



_SetupFCBPointers
    ; Get the device number
    pop     x1
    push    x1

    ; Get the address of the FCB
    GetDeviceValue x1,DCB,DCB_FCB
    move    a1,y0

    ; Make a pointer to FCB name field
    move    #>DIRRecName,a
    add     y0,a
    move    a1,r1

    ; Make a pointer to FCB extension field
    move    #>DIRRecExt,a
    add     y0,a
    move    a1,r2

    ; Make a pointer to FCB attribute field
    move    #>DIRRecAttr,a
    add     y0,a
    move    a1,r3

    ; Restore the parameters
    pop     x1                          ; Device number

    ; Start with the first directory record
    move    #>FirstDIRRecord,x0

_NextRecord
    ; Setup parameters for calling ReadDIRRecord
    move    x0,a1                       ; Record number
    move    x1,a2                       ; Device number

    ; Save variables
    push    x1                          ; Device number
    push    x0                          ; Record number
    push    r0                          ; Pointer to extension list
    push    r1                          ; Pointer to FCB name
    push    r2                          ; Pointer to FCB extension
    push    r3                          ; Pointer to FCB attributes
    push    m0
    push    m1
    push    m2
    push    m3

    ; Read the directory record into the FCB
    jsr     ReadDIRRecord               ; A2=device number, A1=record number

        ; Restore variables
    pop     m3
    pop     m2
    pop     m1
    pop     m0
    pop     r3                          ; Pointer to FCB attributes
    pop     r2                          ; Pointer to FCB extension
    pop     r1                          ; Pointer to FCB name
    pop     r0                          ; Pointer to extension list
    pop     x0                          ; Record number
    pop     x1                          ; Device number

    ; Failed to the read the directory record
    jcs     _LeaveWithError             ; Something's wrong with the directory

    ; Increment the record number
    clr     a
    move    x0,a0
    inc     a
    move    a0,x0

    ; Check if this is the last record (first byte is zero)
    clr     a   #>$FF,y0                ; Watch out for sign extension!
    move    y:(r1),a1
    and     y0,a

    ; Reached the last record in the directory
    jeq     _Return                     ; Found last record

    ; Check for the initial 'V' (in Vnnnnnnn.WAV).
    ; (This will also rule out deleted files and the extended
    ; records of long filenames.)
    move    #>'V',y0
    cmp     y0,a
    jne     _NextRecord

    ; Verify that the record is not directory or volume
    ; (This will also throw out long filename records.)
    move    y:(r3),a
    move    #>(DIRECTORY|VOLUME_LABEL),y0
    and     y0,a
    jne     _NextRecord                 ; Continue (invalid attributes)


    ; Extension MUST be ".WAV" to count
    move    y:(r2),a
    move    #>'VAW',y0
    cmp     y0,a
    jne     _NextRecord
    

    ; Now, decode the name into a number
    push    r1
    push    x1

    move    r1,b1                       ; Point at filename string
    bset    #22,b1
    move    #0,x1                       ; point at first character

    jsr     VoiceName2Num
    jcc     _Num_OK

    pop     x1                          ; filename does not match pattern (or other error)  
    pop     r1
    jmp     _NextRecord

_Num_OK
    pop     x1
    pop     r1

    move    a1,a

    move    y:maxvoicenumber,y0
    cmp     y0,a
    tlt     y0,a
    move    a,y:maxvoicenumber


    ; Move on to the next record
    jmp     _NextRecord


_Return
    ; change back to the root directory
    move    y:dirsave,a2
    move    #>RootDirectory,a1
    bset    #22,a1                      ; Y: space
    jsr     ChangeDirectory             ; A2=Device number, A1=Pointer to path
    jcs     _big_trouble

    move    y:maxvoicenumber,a0
    inc     a
    move    a0,a                        ; return maxvoicenumber + 1
    
    bclr    #HW_SR_C_BITPOS,sr                ; Success - clear carry bit

    jge     _ReturnNow

_LeaveWithError
    bset   #HW_SR_C_BITPOS,sr                 ; Error - set carry bit

_ReturnNow
    rts

_big_trouble
    error

 ~
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           CreateVoiceFilename
;
;   Type:           Function
;
;   Description:    ???
;
;   Inputs:         A2 = Device number
;                   A1 = Complex pointer to string to receive filename
;                   X0 = Number to generate filename (<= 4000000.)
;
;   Outputs:        ???
;                   
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
CreateVoiceFilename

    push    x0

    move    a1,y0                       ; Move complex pointer to X0
    move    #>voicenameprefix,a1
    bset    #22,a1
    move    a1,x0
    move    #0,r1                       ; Point at first character
    move    #0,r0

    move    #>7,a
    jsr     Strncopy

    move    a0,b1                       ; point to the next char in the filename string
    move    r1,x1
    pop     a1                          ; Restore "the number"
    jsr     VoiceNum2Name               ; encode the number into the filename

    move    b1,a0

    rts


    

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           Strncopy
;
;   Type:           Function
;
;   Description:    ???
;
;   Inputs:         x0 - complex pointer to input string
;                   r0 - starting byte offset into input string (not preserved)
;                   y0 - complex pointer to output string
;                   r1 - starting byte offset into output string (not preserved)
;                   a1 - number of bytes to copy (not preserved)
;
;   Outputs:        ???
;                   
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
Strncopy
    do  a1,_CopyBytes
        push    r1
        move    r0,r1
        move    x0,a0
        jsr     CP_GetByte
        move    (r0)+

        pop     r1
        move    y0,a0
        jsr     CP_SetByte
        move    (r1)+
_CopyBytes
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           VoiceName2Num
;
;   Type:           Function
;
;   Description:    ???
;
;   Inputs:         b1 - complex pointer to filename string
;                   x1 - starting byte offset into string (ptr to the first digit)
;
;   Outputs:        If successful:
;                       sr.Carry = Clear
;                       a1 = the number decoded from the filename/error code
;                   If unsuccessful:
;                       sr.Carry = Set
;                   
;   Notes:          Filename is expected to be in the form "Vnnnnnnn.WAV", where
;                   "n" is a decimal digit (zero padding on the left).  This routine
;                   checks for the initial "V" and for DIGITS_IN_VOICE_FILENAME digits
;                   (currently 7 digits are expected);  if the filename does not match
;                   the pattern, an error is returned.  (The ".WAV" extension is not
;                   verified.)
;
;                   Modifies: a,b,y0,y1,r1
;<
;///////////////////////////////////////////////////////////////////////////////
VoiceName2Num

    ; Now try to decode the "nnnnnnn" part.  If any part is
    ; not a digit, skip this file; it's not one of ours.        VOICEHANG1 note This occured in stmp1202.

    move    b1,a0
    move    x1,r1

    jsr     CP_GetByte                  ; Check for initial 'V'
    move    a1,b
    move    #>'V',y0
    cmp     y0,b
    jeq     _V_OK

    bset    #HW_SR_C_BITPOS,sr                ; Doesn't start with 'V'.  Return error.
    rts

_V_OK
    move    (r1)+                       ; Point at first digit.

    move    #0,y1                       ; Y1 = decoded number
    

    do      #DIGITS_IN_VOICE_FILENAME,_DigitsLoop
        jsr     CP_GetByte
        jcc     _Char_OK
        jmp     _AbortDigitsLoop

_Char_OK                            ; char is in A1

        move    a1,b        
        move    #>'0',y0                ; B = char - '0'
        sub     y0,b                    ; if (B<0 || B>9)
        jlt     _AbortDigitsLoop
        move    #>9,y0                  
        cmp     y0,b                    ;   skip this record
        jgt     _AbortDigitsLoop

        ; char - '0' is now in B1
        clr     b   b1,y0
        move    y0,b0                   ; Put (char - '0') in b0
        move    #>5,y0                  ; (5, instead of 10, due to built-in left shift)
        mac     y0,y1,b                 ; Add to 10*number_so_far
        move    b0,y1                   ; Y1 = 10*Y1 + (char-'0')

        move    (r1)+                   ; point at next char

_DigitsLoop
    jmp     _NumberOK

_AbortDigitsLoop
    enddo
    bset    #HW_SR_C_BITPOS,sr
    rts


_NumberOK
    move    y1,a                        ; maxvoicenumber = MAX(maxvoicenumber,number_from_filename)

    bclr    #HW_SR_C_BITPOS,sr
    rts

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           VoiceNum2Name
;
;   Type:           Function
;
;   Description:    ???
;
;   Inputs:         a1 - the number to be used to geenrate the filename (0<=n<=4000000)
;                   b1 - complex pointer to filename string
;                   x1 - starting byte offset into string (ptr to the first digit)
;
;   Outputs:        If successful:
;                       sr.Carry = Clear
;                       Writes filename to string pointed to by b1[x1]
;                   If unsuccessful:
;                       sr.Carry = Set
;                   
;   Notes:          Filename will be in the form "Vnnnnnnn.WAV", where
;                   "n" is a decimal digit (zero padding on the left).
;
;                   Modifies: a,b,x0,x1,y0,y1,r1
;<
;///////////////////////////////////////////////////////////////////////////////
VoiceNum2Name
    move    a1,x0                       ; Save "the number" in x0

    move    a1,a                        ; if (num < 0)
    tst     a
    jge     _NumPositive
    bset    #HW_SR_C_BITPOS,sr                ;   return ERROR
    rts

_NumPositive
    move    #>MAX_VOICE_NUM,y0
    cmp     y0,a                        ; if (num > 4000000)
    jle     _NumValid
    bset    #HW_SR_C_BITPOS,sr                ;   return ERROR
    rts

_NumValid

    move    b1,a0                       ; Set up string ptr for CP_SetByte
    move    x1,r1

    move    #>'V',a1                    ; Start with 'V'
    jsr     CP_SetByte
    move    (r1)+
    ; Convert integer to ASCII (7 digits)
    do      #DIGITS_IN_VOICE_FILENAME,_num_loop        

        move    x0,y0

        move    #.1,x1                  ; Divide number by 10^lc (right shift by lc decimal digits)
        do      lc,_ShiftRight
        mpy     x1,y0,b
        move    b1,y0
_ShiftRight

        lsl     b                       ; sign manipulation (?)
        asr     b

        move    #>10,x1                 ; Shift back up one decimal digit
        move    b0,y0
        mpy     y0,x1,b #>'0',y1        ; b1 = current digit
        ; convert to ASCII, using '0' 
        add     y1,b
        move    b1,a1                   ; move to A1 for CP_SetByte

        jsr     CP_SetByte              ; add digit to filename string
        move    (r1)+
_num_loop

    move    a0,y0                       ; complex pointer to append to filename string
    move    #voicenamesuffix,a0         ; complex pointer to ".WAV" string
    bset    #22,a0
    move    a0,x0
    move    #0,r0                       ; start copying from beginning of ".WAV"
    move    #>5,a                       ; copy 5 bytes (including EOS)
    jsr     Strncopy

    rts

 comment ~
;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FindFirstVoice
;
;   Type:           Function
;
;   Description:    This function finds the voice recording in the specified directory
;                   which has the lowest number.  The filename of the first voice recording
;                   is returned in the file name buffer.
;
;   Inputs:         a2: Device Number
;                   a1: Complex pointer to a directory path
;                   b1: Complex pointer to file name buffer
;                   x1: Byte offset within file name buffer
;
;   Outputs:        If successful:
;                       sr.Carry = Clear
;                   If unsuccessful:
;                       sr.Carry = Set
;                       a1 = Error code
;                   
;   Notes:              ???
;<
;///////////////////////////////////////////////////////////////////////////////
FindFirstVoice
    ; Save the input parameters
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number

    ; Init the filename to "V4000000.WAV" (Largest valid recording)
    move    #>MAX_VOICE_NUM,a1
    jsr     VoiceNum2Name

    ; Restore the input parameters
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer

    ; If VoiceNum2Mame failed then exit
    jcs     _Exit

    ; FindNextVoice will wrap around to the first voice recording
    jsr     FindNextVoice

_Exit
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FindLastVoice
;
;   Type:           Function
;
;   Description:    This function finds the voice recording in the specified directory
;                   which has the highest number.  The filename of the last voice recording
;                   is returned in the file name buffer.
;
;   Inputs:         a2: Device Number
;                   a1: Complex pointer to a directory path
;                   b1: Complex pointer to file name buffer
;                   x1: Byte offset within file name buffer
;
;   Outputs:        If successful:
;                       sr.Carry = Clear
;                   If unsuccessful:
;                       sr.Carry = Set
;                       a1 = Error code
;                   
;   Notes:          ??? 
;<
;///////////////////////////////////////////////////////////////////////////////
FindLastVoice
    ; Save the input parameters
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number

    ; Init the filename to "V0000000.WAV"
    move    y:<Const_000000,a1
    jsr     VoiceNum2Name

    ; Restore the input parameters
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer

    ; If VoiceNum2Name failed then exit
    jcs     _Exit

    ; FindPrevVoice will wrap around to the last voice recording
    jsr     FindPrevVoice

_Exit
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FindNextVoice
;
;   Type:           Function
;
;   Description:    This function finds the next voice recording in the specified
;                   directory.  The name of the current voice recording is passed to
;                   this function in a file name buffer.  This function finds the
;                   voice recording whose name contains the next higher number and
;                   overwrites the passed in filename with the next higher filename.
;                   If no voice recording has a higher number, then this function
;                   wraps around and returns the voice recording with the lowest number.  
;
;   Inputs:         a2 = Device Number
;                   a1 = Complex pointer to a directory path
;                   b1 = Complex pointer to file name buffer
;                   x1 = Byte offset within file name buffer
;
;   Outputs:        If successful:
;                       sr.Carry = Clear
;                   If unsuccessful:
;                       sr.Carry = Set
;                       a1 = Error code
;                   
;   Notes:          ??? 
;<
;///////////////////////////////////////////////////////////////////////////////
FindNextVoice
    ; Save the input parameters
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number

;  debug
;  nop
    ; Extract the current recording number from the filename
    jsr     VoiceName2Num               ; Returns number in a1.  VOICEHANG1
    jcs     _Exit4
    move    a1,x0                       ; Put CurrentNum in x0

    ; Restore parameters needed by SysCountSongs
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path

    ; Put the address of the extension list in r0
    move    #>ExtList,r0

    ; Save the registers
    push    r0                          ; Pointer to extension list
    push    x0                          ; CurrentNum
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number

    ; Count the number of WAV files in the directory
    jsr     SysCountSongs               ; Returns song count in a1
    jcs     _Exit6

    ; If there are no WAV files then return an error
    move    a1,a
    tst     a
    jne     _Setup

    ; There are no WAV files
    move    #>ERR_FILE_NOT_FOUND,a1
    or      #$01,ccr
    jmp     _Exit6
        
_Setup
    ; For song number = song count to zero
    move    a1,b2                       ; SysFindSong needs song number in b2

    ; Init FirstNum and NextNum to large invalid numbers
    move    #>MAX_VOICE_NUM+1,y1        ; FirstNum
    move    #>MAX_VOICE_NUM+1,y0        ; NextNum

    ; Restore all the registers
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path
    pop     x0                          ; CurrentNum
    pop     r0                          ; Pointer to extension list
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer

_NextSong
    ; Save all the registers
    push    r0                          ; Pointer to extension list
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number
    push    b2                          ; Song number
    push    y0                          ; NextNum
    push    y1                          ; FirstNum
    push    x0                          ; CurrentNum
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer

    ; Get the filename of the specified song number
    jsr     SysFindSong
;  debug
;  nop
    jcs     _Exit9

    ; Restore the pointer to the filename (then save it again)
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer

    ; Extract the recording number from the filename
    jsr     VoiceName2Num               ; Returns number in a1

    ; Restore enough of the registers to get CurrentNum, FirstNum and NextNum
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer
    pop     x0                          ; CurrentNum
    pop     y1                          ; FirstNum
    pop     y0                          ; NextNum

    ; If filename of the WAV file is invalid then skip the file
    jcs     _DecSongNumber

    ; Num = recording number
    move    a1,a

    ; If Num > CurrentNum and Num < NextNum        
    cmp     x0,a
    jle     _CheckFirst
    cmp     y0,a
    jge     _CheckFirst
    move    a1,y0                       ; Then NextNum = Num
    
_CheckFirst
    ; If Num < FirstNum
    cmp     y1,a
    jge     _DecSongNumber
    move    a1,y1                       ; Then FirstNum = Num

_DecSongNumber
    ; Decrement the song number
    clr     a
    pop     a0                          ; Song number
    dec     a
    move    a0,b2

    ; Restore the rest of the registers
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path
    pop     r0                          ; Pointer to extension list

    ; If the song number is not zero then find the next song
    jne     _NextSong

    ; If did not find NextNum then wrap around to the FirstNum
    move    #>MAX_VOICE_NUM+1,a
    cmp     y0,a
    jne     _MakeFilename               ; NextNum was found
    cmp     y1,a
    jne     _WrapAround                 ; FirstNum was found

    ; All the WAV files must have invalid names!
    move    #>ERR_FILE_NOT_FOUND,a1
    or      #$01,ccr
    jmp     _Exit

_WrapAround
    ; NextNum = FirstNum
    move    y1,y0

_MakeFilename
    move    y0,a1
    jsr     VoiceNum2Name
    jmp     _Exit
        
_Exit9
    pop     b1
    pop     b1
    pop     b1
_Exit6
    pop     b1
    pop     b1
_Exit4
    pop     b1
    pop     b1
    pop     b1
    pop     b1
_Exit
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FindPrevVoice
;
;   Type:           Function
;
;   Description:    This function finds the previous voice recording in the specified
;                   directory.  The name of the current voice recording is passed to
;                   this function in a file name buffer.  This function finds the
;                   voice recording whose name contains the next lower number and
;                   overwrites the passed in filename with the next lower filename.
;                   If no voice recording has a lower number, then this function
;                   wraps around and returns the voice recording with the highest number.
;
;   Inputs:         a2: Device Number
;                   a1: Complex pointer to a directory path
;                   b1: Complex pointer to file name buffer
;                   x1: Byte offset within file name buffer
;
;   Outputs:        If successful:
;                       sr.Carry = Clear
;                   If unsuccessful:
;                       sr.Carry = Set
;                       a1 = Error code
;                   
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
FindPrevVoice
    ; Save the input parameters
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number

    ; Extract the current recording number from the filename
    jsr     VoiceName2Num               ; Returns number in a1
    jcs     _Exit4
    move    a1,x0                       ; Put CurrentNum in x0

    ; Restore parameters needed by SysCountSongs
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path

    ; Put the address of the extension list in r0
    move    #>ExtList,r0

    ; Save the registers
    push    r0                          ; Pointer to extension list
    push    x0                          ; CurrentNum
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number

    ; Count the number of WAV files in the directory
    jsr     SysCountSongs               ; Returns song count in a1
    jcs     _Exit6

    ; If there are no WAV files then return an error
    move    a1,a
    tst     a
    jne     _Setup

    ; There are no WAV files
    move    #>ERR_FILE_NOT_FOUND,a1
    or      #$01,ccr
    jmp     _Exit6
    
_Setup
    ; For song number = song count to zero
    move    a1,b2                       ; SysFindSong needs song number in b2

    ; Init LastNum and PrevNum to small invalid numbers
    move    #>-1,y1    ; LastNum
    move    #>-1,y0    ; PrevNum

    ; Restore all the registers
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path
    pop     x0                          ; CurrentNum
    pop     r0                          ; Pointer to extension list
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer

_NextSong
    ; Save all the registers
    push    r0                          ; Pointer to extension list
    push    a1                          ; Pointer to directory path
    push    a2                          ; Device number
    push    b2                          ; Song number
    push    y0                          ; PrevNum
    push    y1                          ; LastNum
    push    x0                          ; CurrentNum
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer

    ; Get the filename of the specified song number
    jsr     SysFindSong
    jcs     _Exit9

    ; Restore the pointer to the filename (then save it again)
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer
    push    b1                          ; Pointer to filename buffer
    push    x1                          ; Byte offset within filename buffer

    ; Extract the recording number from the filename
    jsr     VoiceName2Num               ; Returns number in a1

    ; Restore enough of the registers to get CurrentNum, LastNum and PrevNum
    pop     x1                          ; Byte offset within filename buffer
    pop     b1                          ; Pointer to filename buffer
    pop     x0                          ; CurrentNum
    pop     y1                          ; LastNum
    pop     y0                          ; PrevNum

    ; If filename of the WAV file is invalid then skip the file
    jcs     _DecSongNumber

    ; Num = recording number
    move    a1,a

    ; If Num < CurrentNum and Num > PrevNum        
    cmp     x0,a
    jge     _CheckLast
    cmp     y0,a
    jle     _CheckLast
    move    a1,y0                       ; Then PrevNum = Num
    
_CheckLast
    ; If Num > LastNum
    cmp     y1,a
    jle     _DecSongNumber
    move    a1,y1                       ; Then LastNum = Num

_DecSongNumber
    ; Decrement the song number
    clr     a
    pop     a0                          ; Song number
    dec     a
    move    a0,b2

    ; Restore the rest of the registers
    pop     a2                          ; Device number
    pop     a1                          ; Pointer to directory path
    pop     r0                          ; Pointer to extension list

    ; If the song number is not zero then find the next song
    jne     _NextSong

    ; If did not find PrevNum then wrap around to the LastNum
    move    #>-1,a
    cmp     y0,a
    jne     _MakeFilename               ; PrevNum was found
    cmp     y1,a
    jne     _WrapAround                 ; LastNum was found

    ; All the WAV files must have invalid names!
    move    #>ERR_FILE_NOT_FOUND,a1
    or      #$01,ccr
    jmp     _Exit

_WrapAround
    ; PrevNum = LastNum
    move    y1,y0

_MakeFilename
    move    y0,a1
    jsr     VoiceNum2Name
    jmp     _Exit
    
_Exit9
    pop     b1
    pop     b1
    pop     b1
_Exit6
    pop     b1
    pop     b1
_Exit4
    pop     b1
    pop     b1
    pop     b1
    pop     b1
_Exit
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           InitVoiceNumber
;
;   Type:           Function
;
;   Description:    Gets the next voice number available
;
;   Inputs:         none
;
;   Outputs:        none
;                   
;   Notes:          This function must be permanently resident in memory (sys space)
;                   The next voice number is saved in x:NextVoiceNumValue
;                   
;<
;///////////////////////////////////////////////////////////////////////////////
    org p,"SYS_SysVoice_P":
InitVoiceNumber

 if (@DEF('FUNCLET'))           

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_INITVOICENUMBER

    org p,"SYSFUNCLET_InitVoiceNumber_P":
    
    dc      RSRC_FUNCLET_INITVOICENUMBER           ; Funclet Resource number

 endif

    ; Load the fat write code
    jsr     SysFlushCoderMemory

    ; Load the P memory
    move    #>RSRC_FATWRITE_P_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_e_WriteFAT12_p-F_lc_b_WriteFAT12_p)*3,y1 ; Size of the overlay area
    move    #>F_lc_b_WriteFAT12_p,y0        ; Address of the overlay area
    move    #TARGET_MEM_P,a1                ; Overlay area is in P memory
    jsr     SysLoadResource

    ; Load the X memory
    move    #>RSRC_FATWRITE_X_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_e_WriteFAT12_x-F_lc_b_WriteFAT12_x)*3,y1 ; Size of the overlay area
    move    #>F_lc_b_WriteFAT12_x,y0        ; Address of the overlay area
    move    #TARGET_MEM_X,a1                ; Overlay area is in X memory
    jsr     SysLoadResource

    move    #>RSRC_FATWRITE_Y_CODE,x0       ; Resource number
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>(F_lc_e_WriteFAT12_y-F_lc_b_WriteFAT12_y)*3,y1 ; Size of the overlay area
    move    #>F_lc_b_WriteFAT12_y,y0        ; Address of the overlay area
    move    #TARGET_MEM_Y,a1                ; Overlay area is in Y memory
    jsr     SysLoadResource

    ; Write mode
    move    #FileSystemMode,r1
    nop
    bset    #FILE_SYS_MODE_BIT,y:(r1)       ;Sets the file system write loaded

    ; Load the Directory app    
    ; Setup arguments for calling SysLoadResource
    move    #>RSRC_DIRECTORY_P_CODE,x0      ; Resource number of the applett
    move    #>RSRC_TYPE_DATA,x1             ; Resource type is data
    move    #>F_lc_u_bP_directory,y0        ; Address of the application area
    move    #>(F_lc_u_eP_directory-F_lc_u_bP_directory)*3,y1 ; Size of the application area
    move    #TARGET_MEM_P,a1                ; Application area is in P memory

    ; Load the directory applett into the application area
    jsr     SysLoadResource
    
    ; now get the number
    move    x:EncCurrentDeviceNum,a2
    jsr     FindNextVoiceNumber
    jcc     _NoError
    
    ;Check if error due to VOICE directory creation
    move    a1,x0
    clr     a
    move    #>ERROR_CREATING_VOICE_DIR,a1
    cmp     x0,a
    jne     _error
    
    ;Set system variable 
    move    y:<Const_000000,a0
    move    a0,x:NextVoiceNumValue
    jmp     _LoadFileSystemRead         
    
_NoError    

    move    a1,x:NextVoiceNumValue
    move    y:<Const_000001,a0
    

_LoadFileSystemRead
    move    a0,y:VoiceDirCreated

    ; Read mode
    move    #FileSystemMode,r1
    nop
    bclr    #FILE_SYS_MODE_BIT,y:(r1)       ;Sets the file system write loaded

    ; flush app space
    jsr     SysFlushCoderMemory

    rts
    
_error
    error
 ~
    endsec  ; end sys space

