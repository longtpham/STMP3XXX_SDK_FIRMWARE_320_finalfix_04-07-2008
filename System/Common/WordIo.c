////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// Filename: WordIo.c
// Description: Utilities operating on words
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "WordIo.h"

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetWordX
//
//   Type:           Function
//
//   Description:    Gets a 16bit word from a buffer and return in a WORD type.
//                   
//
//   Inputs:         wByteNumber        : position of the LSByte of the Word in the buffer
//                   *pwBuffer          : Pointer to buffer in X memory
//
//   Outputs:        Word to get (16 bits are valid) returned in a WORD type
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
WORD _reentrant GetWordX(WORD wByteNumber, WORD _X *pwBuffer)
{
    WORD wOffsetToWord, wResult;
    BYTE wOffsetByte;
    
    wOffsetToWord = wByteNumber/3;
    wOffsetByte = wByteNumber - (wOffsetToWord*3);
    
    wResult = pwBuffer[wOffsetToWord];
    
    switch(wOffsetByte)
        {
            case 0:
                wResult = wResult & 0x00ffff;
                break;
                
            case 1:
                wResult = (wResult & 0xffff00) >> 8;
                break;
                
            case 2:
                wResult = (wResult & 0xff0000) >> 16;
                wResult = (((pwBuffer[wOffsetToWord+1]) & 0x0000ff) << 8) + wResult;
                break;
        }

    return(wResult);                                        
}    
                

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetDWordX
//
//   Type:           Function
//
//   Description:    Gets a double 16 bit word from a buffer at specified byte.
//
//   Inputs:         wByteNumber        : position of the LSByte of the Word in the buffer
//                   *pwBuffer          : Pointer to buffer in X memory
//
//   Outputs:        Double 16bit word to get (returns 32 bits of data in a DWORD)
//                   
//   Notes:          corrected loss of most significant byte on Oct 8, 2003
//<
///////////////////////////////////////////////////////////////////////////////
DWORD _reentrant GetDWordX(WORD wByteNumber, WORD _X *pwBuffer)
{
    WORD wLSWord;
    WORD wMSWord;
    
    wLSWord = GetWordX(wByteNumber, pwBuffer);
    wMSWord = GetWordX((wByteNumber+2), pwBuffer);
    return( (((DWORD)wMSWord)<<16) + ((DWORD)wLSWord) );
}    



///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetWordY
//
//   Type:           Function
//
//   Description:    Gets a 16bit word from a buffer and return in a WORD type.
//                   
//
//   Inputs:         wByteNumber        : position of the LSByte of the Word in the buffer
//                   *pwBuffer          : Pointer to buffer in Y memory
//
//   Outputs:        Word to get (16 bits are valid) returned in a WORD type
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
WORD _reentrant GetWordY(WORD wByteNumber, WORD _Y *pwBuffer)
{
    WORD wOffsetToWord, wResult;
    BYTE wOffsetByte;
    
    wOffsetToWord = wByteNumber/3;
    wOffsetByte = wByteNumber - (wOffsetToWord*3);
    
    wResult = pwBuffer[wOffsetToWord];
    
    switch(wOffsetByte)
        {
            case 0:
                wResult = wResult & 0x00ffff;
                break;
                
            case 1:
                wResult = (wResult & 0xffff00) >> 8;
                break;
                
            case 2:
                wResult = (wResult & 0xff0000) >> 16;
                wResult = (((pwBuffer[wOffsetToWord+1]) & 0x0000ff) << 8) + wResult;
                break;
        }

    return(wResult);                                        
}    



    
///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           SetWordY
//
//   Type:           Function
//
//   Description:    Set one 16 bit word in buffer at specified byte position
//                   
//
//   Inputs:         wByteNumber        : Byte number in the buffer
//                   wWord              : 16 bit value to write (in type WORD)
//                   *pwBuffer          : Pointer to buffer in Y memory
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
void _reentrant SetWordY(WORD wByteNumber, WORD wWord, WORD _Y *pwBuffer)
{
    WORD wOffsetToWord, wResult, wWordByte;
    BYTE wOffsetByte;
    
    wOffsetToWord = wByteNumber/3;
    wOffsetByte = wByteNumber - (wOffsetToWord*3);
    
    wResult = pwBuffer[wOffsetToWord];

    switch(wOffsetByte)
        {
            case 0:
                wResult = wResult & 0xff0000;
                wResult = wResult | wWord; 
                pwBuffer[wOffsetToWord] = wResult;
                break;
                
            case 1:
                wResult = wResult & 0x0000ff; 
                wResult = wResult | (wWord << 8);
                pwBuffer[wOffsetToWord] = wResult;
                break;
                
            case 2:
                wWordByte = wWord << 16;
                wResult = wResult & 0x00ffff;
                wResult = wResult | wWordByte;
                pwBuffer[wOffsetToWord] = wResult;

                wResult = pwBuffer[wOffsetToWord + 1];
                wWordByte = wWord >> 8;
                wResult = wResult & 0xffff00;
                wResult = wResult | wWordByte;
                pwBuffer[wOffsetToWord + 1] = wResult;
                break;
        }
}    

