#include "fstypes.h"
#include "fsproj.h"
#include "fsapi.h"
#include "platform.h"

//Do not remove this Dummyvariable,as it is put here to  genrate data section 
//.ydatafsutilsc Which is used in Y_application.dsc for section arragment
Dummyvariable=0;
_packed char namebuffer[256];

void _asmfunc packed_strcpy(_packed char *dest, _packed char *src);
void _asmfunc packed_strcat(_packed char *dest, _packed char *src);

void _reentrant ConvName(_packed BYTE *fname, int DeviceNumber)
{
    INT i,byte;
    INT off0 = 0, off1 = 0;
    _memset(namebuffer,0, 86);  // 86*3 = 258
    byte = FSGetByte(fname,0,Y_MEMORY);
    if (byte == 0x2f)
    {
        namebuffer[0] = 0x2f3a00 | DriveLetter[DeviceNumber];
        off0 = 1;
        off1 = 3;
    }
    for (i = 0; i < 253; i++)
    {   
        byte = FSGetByte(fname,i+off0,Y_MEMORY);
        PutByte(namebuffer,byte,i+off1,Y_MEMORY);
    }
}


/*
//////////////////////////////////////////////////////////////////////////////
> Name:            FSFileOpen
 Type:             Function
 Description:      C wrapper for FILEOpen
 Inputs:           
                   a = Attribute(s)
                   r0 = Pointer to file name string
                   b = Device number.
 Outputs:          
                   a = Handle for the file created or -1 if function failed

 Notes:
                   see FILEOpen for further information.
                   File  name must be in Y memory
<         
//////////////////////////////////////////////////////////////////////////////
*/

INT _reentrant FSFileOpen( _packed BYTE *fname,INT attribute, INT DeviceNumber)
{
    INT retval = 0;
	ConvName(fname, DeviceNumber);
	if (attribute == 1)
		retval = Fopen(namebuffer,(_packed char *)"r");
	else if (attribute == 2)
		retval = Fopen(namebuffer,(_packed char *)"r+");
#ifdef AUDIBLE
	else if (attribute == 3)
		retval = Fopen(namebuffer,(_packed char *)"w+");
#endif
	return retval;
}
/*
//////////////////////////////////////////////////////////////////////////////
> Name:            FSFastOpen
 Type:             Function
 Description:      C wrapper for FILEOpen
 Inputs:           
                   a = Fast key(s)
                   b = Attribute
 Outputs:          
                   a = Handle for the file created or -1 if function failed

 Notes:
                           
//////////////////////////////////////////////////////////////////////////////
*/

INT _reentrant FSFastOpen(DWORD Key, INT attribute)
{
    INT retval = 0;
	//ConvName(fname, DeviceNumber);
	if (attribute == 1)
		retval = FastOpen(Key,(_packed char *)"r");
	else if (attribute == 2)
		retval = FastOpen(Key,(_packed char *)"r+");
	return retval;
}


/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileCreate
; Type:             Function
; Description:      C wrapper for FILECreate
; Inputs:           
;                   a = Device Number
;                   r0 = Pointer to File Name String
; Outputs:          
;                   a = Handle for the file created or -1 if function failed
;
; Notes:
;                   see FILECreate for further information.
;                   File name must be in Y memory
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT FSFileCreate( _packed BYTE *fname,INT DeviceNumber)
{
	ConvName(fname, DeviceNumber);
    return Fopen(namebuffer,(_packed char *)"w");
}



/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileDelete
; Type:             Function
; Description:      C wrapper for FILEDelete
; Inputs:           
;                   a = Device Number
;                   r0 = Pointer to File Name String
; Outputs:          
;                   a = -1 if function failed
;
; Notes:
;                   File name must be in Y memory
;<         
;///////////////////////////////////////////////////////////////////////////////
*/

INT FSFileDelete(_packed BYTE *fname,INT DeviceNumber)
{
	ConvName(fname, DeviceNumber);
    return Fremove(namebuffer);
}
    
    
    
/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileEof
; Type:             Function
; Description:      C wrapper for FileEOF
; Inputs:           
;                   a = File Handle.
; Outputs:          
;                   a = Number of bytes from the end or $7fffff if number of bytes
;                        from the end of file greater than $7fffff.
;                   a = -1 if function failed.
;
; Notes:
;                   see FILECreate for further information.
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT _reentrant FSFileEof(INT Handle)
{
    LONG retval;
    retval = Feof(Handle);
    if (retval > (LONG)0x7fffff)
    {
        retval = (LONG)0x7fffff;
    }
    return (INT)retval;
}


/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileRead
; Type:             Function
; Description:      C wrapper for FileRead
; Inputs:           
;                   a = Number of bytes to read
;                   b = Handle
;                   x0 = Memory Space
;                   y0 = Modulo
;                   r0 = Buffer Address
; Outputs:          
;                   a =Number bytes read or -1 if function failed
;
; Notes:
;                   see FileRead for further information.
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT _reentrant FSFileRead(INT BytestoRead, INT Handle, INT memory_type, INT modulo, WORD *buffer)
{
    INT memtype;
    if (memory_type == 0x100000)
        memtype = P_MEMORY;
    else if (memory_type == 0x400000)
        memtype = Y_MEMORY;
    else
        memtype = X_MEMORY;

    return Fread(Handle, (INT *) buffer, (LONG) BytestoRead, memtype,(INT) modulo);
}    



/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            fseek
; Type:             Function
; Description:      C wrapper for FileSeek
; Inputs:           
;                   b1= FileHandle
;                   a = Number of bytes to seek
;                   x0= origin (SEEK_CUR,SEEK_END,SEEK_SET)           
; Outputs:          
;                   a = 0 if successful or -1 if function failed
;
; Notes:
;                   must convert all variables to FileSeek parameters..
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT _reentrant fseek(INT Handle, LONG SeekOffset, INT type)
{
    return Fseek(Handle,SeekOffset,type);
}

/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileWrite
; Type:             Function
; Description:      C wrapper for FileWrite
; Inputs:           
;                   a = Number of bytes to write
;                   b = Handle
;                   x0 = Memory Space
;                   y0 = Modulo
;                   r0 = Buffer Address
; Outputs:          
;                   a =Number bytes written or -1 if function failed
;
; Notes:
;                   see FileWrite for further information.
;<         
;///////////////////////////////////////////////////////////////////////////////*/

INT _reentrant FSFileWrite(INT wNumberBytes, INT Handle, INT memory_type, INT iModulo, WORD *buffer)
{
    INT memtype;
    if (memory_type == 0x100000)
        memtype = P_MEMORY;
    else if (memory_type == 0x400000)
        memtype = Y_MEMORY;
    else
        memtype = X_MEMORY;

    return Fwrite(Handle, (INT *)buffer, (LONG) wNumberBytes, memtype, iModulo);
}



LONG _reentrant ftell(INT Handle)
{
    return Ftell(Handle);
}



LONG _reentrant FSFileSize(INT HandleNumber)
{
    return GetFileSize(HandleNumber);
}


/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSChangeDir
; Type:             Function
; Description:      C wrapper for ChangeDirectory
; Inputs:           
;                   a = Device Number
;                   r0 = Pointer to Directory Name String
; Outputs:          
;                   a = -1 if function failed
;
; Notes:
;                   see ChangeDirectory for further information.
;                   Directory name must be in Y memory
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT _reentrant FSChangeDir( _packed BYTE *dirname,INT DeviceNumber)
{
	ConvName(dirname, DeviceNumber);
    return Chdir(namebuffer);
}



INT _reentrant FSCreateDir(_packed BYTE *dirname,INT DeviceNumber)
{
	ConvName(dirname, DeviceNumber);
    return Mkdir(namebuffer);
}


INT _reentrant FSDeleteDir(_packed BYTE *dirname,INT DeviceNumber)
{
	ConvName(dirname, DeviceNumber);
    return Rmdir(namebuffer);
}

// 1Jul2005 convert to reentrant for yovl mem savings but uses p
INT _reentrant FSFileClose(INT Handle)
{
    return Fclose(Handle);
//    Fclose(Handle); // can use this ver if _inline but extern keyword in h file prevents inline from working
}


/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileAppend
; Type:             Function
; Description:      C wrapper for FILEAppend
; Inputs:           
;                   a = Device Number
;                   r0 = Pointer to File Name String
; Outputs:          
;                   a = Handle for the file created or -1 if function failed
;
; Notes:
;                   see FILEAppend for further information.
;                   File name must be in Y memory
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT _reentrant FSFileAppend(_packed BYTE *fname,INT DeviceNumber)
{
	ConvName(fname, DeviceNumber);
	return Fopen(namebuffer,(_packed char *)"a");
}


/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            FSFileAttribute
; Type:             Function
; Description:      C wrapper for FILEAttribute
; Inputs:           
;                   a = Attribute(s)
;                   r0 = Pointer to file name string
;                   b = Device number.
; Outputs:          
;                   a = -1 if function failed
;
; Notes:
;                   see FILEAttribute for further information.
;<         
;///////////////////////////////////////////////////////////////////////////////
*/
INT _reentrant FSFileAttribute(INT Attributes, _packed BYTE *fname, INT DeviceNumber)
{
    INT retval, attrib;
    INT handle;
	ConvName(fname, DeviceNumber);
	handle = Fopen(namebuffer,(_packed char *)"r");
    if (handle < 0)
        return handle;
    attrib = filegetattrib(namebuffer);
    Attributes = Attributes | attrib;        
    retval = filesetattrib(handle, Attributes);
    Fclose(handle);
    return retval;
}



INT _reentrant FSFileAttributeClear(INT Attributes, _packed BYTE *fname, INT DeviceNumber)
{
    INT retval, attrib;
    INT handle;
	ConvName(fname, DeviceNumber);
	handle = Fopen(namebuffer,(_packed char *)"r");
    if (handle < 0)
        return handle;
    attrib = filegetattrib(namebuffer);
    Attributes = Attributes ^ 0xffffff;
    Attributes = Attributes & attrib;
    retval = filesetattrib(handle, Attributes);
    Fclose(handle);
    return retval;
}
    
    
