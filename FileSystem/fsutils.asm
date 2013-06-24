

    section FsUtils
    
    extern FFSFileOpen
    extern FFSFileCreate
    extern FFSFileDelete
    extern FFSFileEof
    extern FFSFileRead
    extern Ffseek
    extern FFSFileWrite
    extern FFSFileClose
    extern FFSFileAppend
    extern FFSFileAttribute
    extern FFSFileAttributeClear
    extern FDataDriveWriteSector
    extern FDataDriveReadSector
    extern FDataDriveInit
    extern FFSFreeClusters
    extern FBytesPerCluster
    extern FFSFreeSpace
    extern FFastOpen 
    extern Ffilegetdate
    extern Ffilesetattrib 
    extern Ffilesetdate
    extern FFgetFastHandle
    extern FDeleteTree
    extern FDeleteAllRecords
    extern FClearData
    extern FChangeToLowLevelDir
    extern FDelGetRecordNumber
    extern FFileRemove
    extern FFopenw
    extern FFremovew
    extern FFgetc
    extern FFgets
    extern FFputc
    extern FFputs
    extern FGetShortfilename
    extern FConstructLongFileName
    extern FFsShutDown
    extern FMkdirw
    extern FRmdirw
	extern FSetVolumeLabel
	extern FGetVolumeLabel

    
    global FILEOpen
    global FILEClose
    global FILEAppend
    global FILECreate
    global FILEDelete
    global FileEOF
    global FileRead
    global FileSeek
    global FileWrite
    global FILEAttribute
    global FILEAttributeClear        
    global FMediaWrite
    global FMediaRead
    global FSDataDriveInit
    global DiskBytesFree

    include "sysmacro.asm"
    include "const.xref"

    org p,".ptextFsUtils":
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FILEOpen
; Type:             Function
; Description:      Opens the file specified by a1 in device specified by a2.
; Inputs:           
;                   a2 = Device number
;                   a1 = Complex Pointer to file name string.
;                        Valid target memory are X and Y. 
;                   a0 = Attribute(s)
; Outputs:          
;                   Carry = 0 => File opened successfully.
;                   Carry = 1 => Error opening file.
;                   a1 = Error Code 
;                   a1 = File Handle Number.
;
; Notes:
;                   Valid attributes are :
;                       READ
;                       WRITE
;                   No append operations will be allowed to a file opened by this
;                   function (see FILEAppend()). Valid operations include
;                   FileRead, FileWrite and FileSeek.
;<         
;///////////////////////////////////////////////////////////////////////////////
FILEOpen:
    move  ssh,y:(r7)+
    move  a2,b
    move  a1,r0
    move  a0,a
    jsr   FFSFileOpen
    btst  #23,A1        ; Error is negative
    move y:-(r7),ssh
    nop
    rts
    


;///////////////////////////////////////////////////////////////////////////////
;> Name:            FILECreate
; Type:             Function
; Description:      Creates the file which name is specified in a1.
; Inputs:           
;                   a2 = Device number where to create the file
;                   a1 = Complex Pointer to the file name string.
;                        Valid target memory are X and Y. 
; Outputs:          
;                   Carry = 0 => File created successfully
;                   Carry = 1 => Error creating file. 
;                   a1 = Error Code if file fail to create
;                   a1 = Handle for the file created
;
; Notes:
;                   The FAT write code must be loaded prior calling this function.
;                   The new file entry will contain zero bytes and one cluster will
;                   be allocated to the file.
;<         
;///////////////////////////////////////////////////////////////////////////////
FILECreate
    move  a1,r0
    move  a2,a
    jsr   FFSFileCreate
    btst  #23,A1        ; Error is negative
    rts



;///////////////////////////////////////////////////////////////////////////////
;> Name:            FILEDelete
; Type:             Function
; Description:      Deletes file which name is pointed by a1.
;                   
; Inputs:           
;                   a2 = Device number 
;                   a1 = Complex Pointer to the file name string.
;                        Valid target memory are X and Y. 
; Outputs:          
;                   Carry = 0 => File deleted successfully
;                   Carry = 1 => Error deleting file. 
;                   a1 = Error Code 
;
; Notes:
;                   The FAT write code must be loaded prior calling this function.
;<         
;///////////////////////////////////////////////////////////////////////////////
FILEDelete
    move  a1,r0
    move  a2,a
    jsr   FFSFileDelete
    btst  #23,A1        ; Error is negative
    rts



;///////////////////////////////////////////////////////////////////////////////
;> Name:            FileEOF
; Type:             Function
; Description:      Checks if file pointer is at the end of the file.
;                   specified in (A0).
; Inputs:           
;                   a2 = File handle.
; Outputs:          
;                   Carry = 0 => File pointer not at the end of the file.
;                   a1 = Number of bytes from the end or $7fffff if number of bytes
;                        from the end of file greater than $7fffff.
;                   Carry = 1 => File pointer at the end of file, or function failed.
;                   a1 = FCBwarningEOF if file pointer at the end of file
;                   a1 = Error code if file pointer not at the end of file.
;
; Notes:
;<         
;///////////////////////////////////////////////////////////////////////////////
FileEOF
    move a2,a
    jsr  FFSFileEof  
    btst  #23,A1        ; Error is negative
    rts



;///////////////////////////////////////////////////////////////////////////////
;> Name:            FileRead
; Type:             Function
; Description:      Reads n bytes, specified by a1, from the file specified by a2.
;                   The data read is saved in the target buffer specified by a0, m0, r0.
; Inputs:           
;                   a2 = File Handle
;                   a1 = Number of bytes to read. (maximum number is 65535)
;                   a0 = Target buffer memory space.
;                       Memory space supported:
;                           $100000 - P mem
;                           $400000 - Y mem
;                           $800000 - X mem
;                   r0 = Starting address target
;                   m0 = Target buffer modulo (-1 for linear)
; Outputs:          
;                   Carry = 0 => Read successful
;                   a1 = Number of bytes read.
;                   Carry = 1 => Error reading file. 
;                   a1 = Error Code 
;
; Notes:
;                   Reads can only be done forward, thus you can not read the previous n bytes
;                   from the current position.
;                   This function will fail for files opened in APPEND mode.
;<         
;///////////////////////////////////////////////////////////////////////////////
FileRead:
    move a2,b
    move a0,x0
    move a1,a
    move m0,y0
    move y:<Const_ffffff,m0
    jsr  FFSFileRead
    btst  #23,A1        ; Error is negative
    rts
 

;///////////////////////////////////////////////////////////////////////////////
;> Name:            FileSeek
; Type:             Function
; Description:      Adjusts the file pointer by the number of bytes specified in a0
; Inputs:           
;                   a2 = File handle
;                   a0 = Number of bytes to seek (bits 0-23)
;                   a1 = Number of bytes to seek (bits 24-31)
; Outputs:          
;                   Carry = 0 => Seek successful
;                   a = Number of bytes adjusted.
;                   Carry = 1 => Error seeking file. 
;                   a1 = Error Code 
;
; Notes:
;                   If number of bytes to seek is 0, the file pointer is set to the
;                   beginning of the file.
;                   For files opened in WRITE mode, the write buffer is flushed prior
;                   adjusting the file pointer.
;                   This function is not valid if the file is in APPEND mode.
;<         
;///////////////////////////////////////////////////////////////////////////////
FileSeek
    move a2,b
    move #0,a2
    move y:<Const_ffffff,x0
    jsr  Ffseek
    btst  #23,A1        ; Error is negative
    rts


;///////////////////////////////////////////////////////////////////////////////
;> Name:            FileWrite
; Type:             Function
; Description:      Writes n bytes, specified by a1, to the file specified by a2.
;                   The data to write is located in the source buffer specified by a0, m0, r0.
; Inputs:           
;                   a2 = File Handle
;                   a1 = Number of bytes to write. (maximum number is 65535)
;                   a0 = Source buffer memory space.
;                       Memory space supported:
;                           $100000 - P mem
;                           $400000 - Y mem
;                           $800000 - X mem
;                   r0 = Starting address source
;                   m0 = Target buffer modulo (-1 for linear)
; Outputs:          
;                   Carry = 0 => Write successful
;                   a1 = Number of bytes written.
;                   Carry = 1 => Error writing file. 
;                   a1 = Error Code 
;
; Notes:
;                   Writes can only be done forward, thus you can not write the previous n bytes
;                   from the current position.
;                   This function will only append to a file when open by FileCreate or
;                   FileAppend.
;<         
;///////////////////////////////////////////////////////////////////////////////
FileWrite
    move a2,b
    move a0,x0
    move a1,a
    move m0,y0
    move y:<Const_ffffff,m0
    jsr  FFSFileWrite
    btst  #23,A1        ; Error is negative
    rts


FILEClose
    move a2,a
    jsr  FFSFileClose
    btst  #23,A1        ; Error is negative
    rts

;///////////////////////////////////////////////////////////////////////////////
;> Name:            FILEAppend
; Type:             Function
; Description:      Searches the device specified by a2 for file specified by a1
;                   and opens it in APPEND mode.
; Inputs:           
;                   a2 = Device number
;                   a1 = Complex Pointer to file name string.
;                        Valid target memory are X and Y. 
; Outputs:          
;                   Carry = 0 => File successfully opened.
;                   Carry = 1 => Error. 
;                   a1 = Error Code
;                   a1 = File handle number. 
;
; Notes:
;                   This function does not create the file. The file can not be 
;                   read when opened in this mode, only writes are valid.
;<         
;///////////////////////////////////////////////////////////////////////////////
FILEAppend
    move  a1,r0
    move  a2,a
    jsr   FFSFileAppend
    btst  #23,A1        ; Error is negative
    rts
    
    

;///////////////////////////////////////////////////////////////////////////////
;> Name:            FILEAttribute
; Type:             Function
; Description:      Logically ORes the file attributes with those 
;                   specified in (A0).
; Inputs:           
;                   a2 = Device number
;                   a1 = Complex Pointer to file name string.
;                        Valid target memory are X and Y. 
;                   a0 = Attribute(s) to change
; Outputs:          
;                   Carry = 0 => Attributes changed successfully
;                   Carry = 1 => Error changing attributes. 
;                   a1 = Error Code 
;
; Notes:
;                   Valid attributes are :
;                       READ
;                       WRITE
;                       APPEND
;                       TRUNCATE
;                       CREATE
;<         
;///////////////////////////////////////////////////////////////////////////////
FILEAttribute
    move a2,b
    move a1,r0
    move a0,a
    jsr  FFSFileAttribute
    btst  #23,A1        ; Error is negative
    rts


FILEAttributeClear
    move a2,b
    move a1,r0
    move a0,a
    jsr  FFSFileAttributeClear
    btst  #23,A1        ; Error is negative
    rts




FMediaWrite
	move  (r7)+
    jsr   FDataDriveWriteSector
    move  (r7)-
	rts


FMediaRead
	move  (r7)+
    jsr   FDataDriveReadSector
    move  (r7)-
	rts

FSDataDriveInit
	move  (r7)+
    jsr   FDataDriveInit
    move  (r7)-
    rts

DiskBytesFree
;           a1 = num bytes high
;           a0 = num bytes low
;           b1 = number of free clusters
;           b0 = number of bytes per cluster
    move  a2,a
    push a1
    jsr  FFSFreeClusters
    pop  x0
    push a1                 
    tfr  x0,a
    push x0                 ; Device
    jsr  FBytesPerCluster
    pop  x0
    push a1
    tfr  x0,a
    jsr  FFSFreeSpace
	clr  B
    pop  b0                 ; Bytes per cluster
    pop  b1                  ; Free clusters
    btst  #23,A1            ; Error is negative
    rts    

    endsec


