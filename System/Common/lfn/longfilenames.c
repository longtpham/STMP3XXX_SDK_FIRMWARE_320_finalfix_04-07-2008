#include "longfilenames.h"
#include "stringlib.h"
#define GetByteFromArray GetByteFromArrayInY
extern LFN DirLfn;
////////////////////////////////////////////////////////////////////////
//
//>  Name:          WORD GetUnicodeCharacter()
//
//   Type:          Function
//
//   Description:   indexed starting 0
//
//   Inputs:        pointer to the beginning of the string.
//
//   Outputs:       returns a Unicode character in a _packet BYTE buffer
//
//   Notes: zero is returned if the cahracter is 0xFFFF
//   		because that is the terminator in the directory entry          
//<
/////////////////////////////////////////////////////////////////////////

WORD _reentrant GetUnicodeCharacter(WORD *source, 	int charNumber){		//each Unicode char is 2 bytes
	WORD rtn = 0;
	rtn =  GetByteFromArrayInY(source,charNumber*2+1)<<8;		  //each unicode character is stored in little endian so reverse it
	rtn |= GetByteFromArrayInY(source,charNumber*2);
	if(rtn == 0x00FFFF){
		rtn = 0;	//null terminator for unicode string
	}
	return rtn;
}

////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant LFNConstructFileName()
//
//   Type:          Function
//
//   Description:   Created a Unicode string for the file described by
//					the LFN_FILE_LOCATOR object that it is passed.
//
//   Inputs:        1. Pointer to a LFN_FILE_LOCATOR
//					2. Pointer to an output buffer.  LFNs can be up to 255
//					characters so make sure the buffer is that large.  No error
//					checking is done for overflow.
//
//   Outputs:       returns the length of the Unicode string that is created          
//					by returnung the length the programmer is allowed to do things
//					like:
//					if(LFNConstructFileName(...))	//this will fail the conditional if the
//					LFN does not exists!
//
//<
/////////////////////////////////////////////////////////////////////////
RETCODE _reentrant LFNConstructFileName(LFN_FILE_LOCATOR* LFNFileLocator,UCS3* Buffer){
	int i=1;
	int j=0;
	int LFNStringOffset = 0;
	LFN *pLFNfcb = &DirLfn;

	while(1){		
		if(LFNFileLocator->wFCBEntry-i >=0){	//make sure we don't try and read a negative directory entry
			//the next line populates the DirLfn data structure with pertinent LFN data
			FSReadLFNRecord(LFNFileLocator->bDevice,LFNFileLocator->wFCBEntry-i);
			if(i==1){ 			//only occurs once per file (i only equals 1 the first iteration of the loop).
				if((pLFNfcb->m_wSignature & 0x0000FF) != 0x000001 && (pLFNfcb->m_wSignature & 0x0000FF) != 0x000041){		 //in this case there is no LFN entry
					Buffer[0] = 0;
					FSChangeDir(0,LFNFileLocator->bDevice);	  //change directory to root
					return 0;
				}
			} 
			if(pLFNfcb->m_wAttributes !=  0x00000F){
				Buffer[LFNStringOffset] = 0;				//null terminator
				FSChangeDir(0,LFNFileLocator->bDevice);
				return LFNStringOffset;
			}
		}
		else{
			Buffer[LFNStringOffset] = 0;					//null terminator
			FSChangeDir(0,LFNFileLocator->bDevice);			//change directory to root
			return LFNStringOffset;
		}
		for(j=0;j<5;j++){	  //5 chars
			Buffer[LFNStringOffset++] = GetUnicodeCharacter(pLFNfcb->m_wFileName1,j);
		}
		for(j=0;j<6;j++){	 //6 chars
			Buffer[LFNStringOffset++] = GetUnicodeCharacter(pLFNfcb->m_wFileName2,j);
		}
		for(j=0;j<2;j++){	 //2 chars
			Buffer[LFNStringOffset++] = GetUnicodeCharacter(pLFNfcb->m_wFileName3,j);
		}
		i++;
	}
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant FSLFNGetFileName()
//
//   Type:          Function
//
//   Description:  Extracts the LFN from the file pointed to by the FileHandle 
//
//   Inputs:       1. File Handle 
//				   2. Device where the file is	
//				   3. Pointer to UCS3 buffer: Long File Name will be copied here!
//
//   Outputs:       Length of string: 0-256
//
//   Notes: 8.3 checksum is not checked on LFN Entries to verify integrity...         
//			A changdir must be done before this function is called so 
//			that the current directory is the one in which this file exists.
//<			SysCallFunction callable
//////////////////////////////////////////////////////
RETCODE _reentrant FSLFNGetFileName(int fileHandle, int b, UCS3 *LFNFileName){
	FILE_CONTROL_BLOCK * fileHandleEntry;	  		
	LFN_FILE_LOCATOR LFNFileLocatoror;
	
	fileHandleEntry = FSHNDLGetPointer(fileHandle);	//c-wrapper for HNDLGetPointer in handle.asm
	
	if(fileHandleEntry){							//is the handle in the Handle Table?
		LFNFileLocatoror.bDevice = fileHandleEntry->Flags.B.Device;	//setup the LFN_FILE_LOCATOR object
		LFNFileLocatoror.wFCBEntry = fileHandleEntry->wRecord;	
		
		return LFNConstructFileName(&LFNFileLocatoror,LFNFileName);	//this should populate our string with the LFN			
	}
	else{		//if no handle is in the table then return a zero length string
		LFNFileName[0] = 0;
		return 0;	
	}
}
