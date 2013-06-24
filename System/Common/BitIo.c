////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: BitIo.c
// Description: Utilities operating on bits
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "chkdsk.h"
#include "types.h"
#include "bitio.h"

extern FAT_STRUCT stFat;

WORD MaskArray[24]={0x1, 0x2, 0x4, 0x8,
                    0x10, 0x20, 0x40, 0x80,
                    0x100, 0x200, 0x400, 0x800,
                    0x1000, 0x2000, 0x4000, 0x8000,
                    0x10000, 0x20000, 0x40000, 0x80000,
                    0x100000, 0x200000, 0x400000, 0x800000};


extern INT _reentrant FAT32_UpdateBit(INT DeviceNum, LONG sectorNumber, WORD wOffsetToWord, WORD wOffsetBit,WORD Bittype);
extern WORD FAT32_ReadWord(INT DeviceNum, LONG sectorNumber, WORD wOffsetToWord);
extern WORD  DivBy24(WORD woffset);
///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetBitY
//
//   Type:           Function
//
//   Description:    Gets one bit from a buffer in Y memory
//                   
//

//   Inputs:         wBitNumber         : Bit number in the buffer
//                   *pwBuffer          : Pointer to buffer in Y memory
//
//   Outputs:        TRUE if bit is set
//                   FALSE if bit is clear
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
//BOOL _reentrant GetBitY(DWORD wBitNumber, WORD _X *pwBuffer,BYTE bLogDevNumber,FAT_TYPE  TypeFileSystem)
//{
//    WORD wOffsetToWord,woffset;
//    BYTE wOffsetBit;
//    INT _X *buf;
//    DWORD sector;
//    WORD word;
//
//   
//    if(TypeFileSystem == FS_FAT32)
//    {   
//	    sector =   wBitNumber >>CLUSTER_ENTRY_PER_SECTOR_SHIFT;
//		woffset =  wBitNumber & 0x0fff;
//	    wOffsetToWord = DivBy24(woffset);
//	    wOffsetBit = woffset -(wOffsetToWord*24);
//	    sector = sector+ stFat.FirstSecondaryFatSect;
//        
//        word = FAT32_ReadWord(bLogDevNumber, sector, wOffsetToWord);
//        
//        if( (word & MaskArray[wOffsetBit]) == 0x0)
//	        return(FALSE);
//	    else
//	        return(TRUE);  
//	}          
//    else
//    {       
//		wOffsetToWord = wBitNumber/
//		24;
//		wOffsetBit = wBitNumber - (wOffsetToWord*24);
//
//		if((pwBuffer[wOffsetToWord] & MaskArray[wOffsetBit]) == 0x0)
//		    return(FALSE);
//		else
//		    return(TRUE);    
//    } 
//
//
//}                                            
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
////
////>  Name:           SetBitY
////
////   Type:           Function
////
////   Description:    Set one bit in Y memory 
////                   
////
////   Inputs:         wBitNumber         : Bit number in the buffer
////                   *pwBuffer          : Pointer to buffer in Y memory
////
////   Outputs:        None
////                   
////   Notes:          none
////<
/////////////////////////////////////////////////////////////////////////////////
//
//INT _reentrant SetBitY(DWORD wBitNumber, WORD _X *pwBuffer,BYTE bLogDevNumber,FAT_TYPE  TypeFileSystem)
//{
//    DWORD sector;
//    BYTE wOffsetBit;
//	INT _X *buf;
//	WORD wOffsetToWord;
//	WORD woffset;
//    
//    if(TypeFileSystem == FS_FAT32)
//    {
//	    sector =   wBitNumber >>CLUSTER_ENTRY_PER_SECTOR_SHIFT;
//		woffset =  wBitNumber & 0x0fff;
//	    wOffsetToWord = DivBy24(woffset);
//	    wOffsetBit = woffset -(wOffsetToWord*24);
//	    sector = sector+ stFat.FirstSecondaryFatSect;
//        return FAT32_UpdateBit(bLogDevNumber,sector,wOffsetToWord,wOffsetBit,1);
//   }
//   else
//   {
//    wOffsetToWord = wBitNumber/24;
//    wOffsetBit = wBitNumber - (wOffsetToWord*24);
//
//    pwBuffer[wOffsetToWord] = pwBuffer[wOffsetToWord] | MaskArray[wOffsetBit];
//	return (NOERROR);
//   }
//}

/////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           UpdateBit
//
//   Type:           Function
//
//   Description:    Get the bit and set it to o or 1 accordig to the passing parqameter 
//                   
//
//   Inputs:         wBitNumber         : Bit number in the buffer
//                   *pwBuffer          : Pointer to buffer in X memory. 
//					bLogDevNumber	  : Device number
//						FAT_TYPE          : FAT12,FAT16 OR FAT32
//						Bittype           : bit vallue to be set to 0 or 1					
//   Outputs:       bit value	 befoe set
					  
//                   
//   Notes:         pwBuffer comment corrected to buffer in X not Y. oct 2004
//<
/////////////////////////////////////////////////////////////////////////////////


WORD _reentrant UpdateBit(DWORD wBitNumber, WORD _X *pwBuffer,BYTE bLogDevNumber,FAT_TYPE  TypeFileSystem,WORD Bittype)
{
    WORD wOffsetToWord,woffset;
    BYTE wOffsetBit;
    INT _X *buf;
    DWORD sector;
    WORD word,temp;

   
    if(TypeFileSystem == FS_FAT32)
    {   
	    sector =   wBitNumber >>CLUSTER_ENTRY_PER_SECTOR_SHIFT;
		woffset =  wBitNumber & 0x0fff;
	    wOffsetToWord = DivBy24(woffset);
	    wOffsetBit = woffset -(wOffsetToWord*24);
	    sector = sector+ stFat.FirstSecondaryFatSect;
        return FAT32_UpdateBit(bLogDevNumber,sector,wOffsetToWord,wOffsetBit,Bittype);
	}          
    else
    {       
		wOffsetToWord = wBitNumber/24;
		wOffsetBit = wBitNumber - (wOffsetToWord*24);

			switch(Bittype)
		{
			case GET_BIT:
				return (pwBuffer[wOffsetToWord] & MaskArray[wOffsetBit]);
			case SET_BIT:
				temp = (pwBuffer[wOffsetToWord] & MaskArray[wOffsetBit]);
	            pwBuffer[wOffsetToWord] = pwBuffer[wOffsetToWord] | MaskArray[wOffsetBit];
				return temp;
			case FREE_BIT:
				temp = 0xffffff ^ MaskArray[wOffsetBit];
                pwBuffer[wOffsetToWord] = pwBuffer[wOffsetToWord] & temp;
 				return 0;	
 		}	
 	 }							


   return(NOERROR);    
   


}                                            




#pragma asm
FDivBy24:
	move A,x0
	move #>$55556,y0
	mpy  x0,y0,A
	move A1,A
	rts
#pragma endasm