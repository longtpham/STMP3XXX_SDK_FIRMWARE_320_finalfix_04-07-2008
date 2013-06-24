///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mp3metadata.c
// Description: Functions for extracting the mp3 meta-data.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "resource.h"
#include "metadata.h"
#include "wmametadata.h"

#pragma optimize 1

//This value is the current known header object header size 
//(this must be seeked past to get to the objects in the header)

typedef union
{
    WORD wValues[6];            //there is some wasted space here  (6 words is 18 bytes, not 16)
    _packed BYTE cValues[16];
} GUID;

typedef struct
{
    GUID ObjectGUID;
    DWORD ObjectSize;         //more wasted space (3 words is 9 bytes, not 8)
} ASFOBJECTHEADER;


BYTE _reentrant  GetWmaMetaData_ReadBYTE(BYTE iHandle)
{
    WORD TmpVal;
    TmpVal=0;//zero out 

    FSFileRead(1,iHandle,MEM_SPACE_Y,-1,&TmpVal);//read in 1 byte
    return (BYTE)TmpVal;

}


WORD _reentrant  GetWmaMetaData_ReadWORD(BYTE iHandle)
{
    WORD TmpVal;
    TmpVal=0;//zero out 

    FSFileRead(2,iHandle,MEM_SPACE_Y,-1,&TmpVal);//read in 2 bytes
    return (WORD)TmpVal;

}

DWORD _reentrant  GetWmaMetaData_ReadDWORD(BYTE iHandle)
{
    WORD TmpVal[2];
    TmpVal[1]=0;//zero out the upper word.

    FSFileRead(4,iHandle,MEM_SPACE_Y,-1,TmpVal);//read in 4 bytes
    return ((DWORD)TmpVal[0] + ((DWORD)TmpVal[1]<<24));//they just so happen to be in that order

}

DWORD _reentrant  GetWmaMetaData_ReadQWORD(BYTE iHandle)
{//this reads in a QWORD from the ASF file, but truncates to a DWORD
    WORD TmpVal[3];
    TmpVal[2]=0;

    FSFileRead(8,iHandle,MEM_SPACE_Y,-1,TmpVal);//read in 8 bytes
    return ((DWORD)TmpVal[0] + ((DWORD)TmpVal[1]<<24));//we only use the bottom two words (6 bytes).

}



RETCODE _reentrant GetWmaMetaData_ReadObjectHeaderPrologue(BYTE iHandle, ASFOBJECTHEADER*pObjectHeader)
{
    int iError;
    //this zero's out the non even sized pieces  
    pObjectHeader->ObjectGUID.wValues[5] = 0;
    iError = FSFileRead(16,iHandle,MEM_SPACE_Y,-1,(void*)pObjectHeader);
    pObjectHeader->ObjectSize = GetWmaMetaData_ReadQWORD(iHandle);
    return iError;
}


RETCODE _reentrant GetWmaMetaData_CompareGUID(GUID *pLeft,GUID *pRight)
{
    int i;
    register int *pLeftGUID=(int*)pLeft;
    register int *pRightGUID=(int*)pRight;
    RETCODE returnValue = FALSE;
    for(i=0;i<6;i++)
    {
        if(*pLeftGUID++ != *pRightGUID++)
        {
            returnValue = FALSE;
            break;
        }
        returnValue = TRUE;
    }
    return returnValue;
}

DWORD _reentrant GetWmaMetaData_FindNextObject(BYTE iHandle, int iMaxObjects,GUID *pGUID)
//Will return -1 if no object found,  or the size of the object.  (We shouldn't have any objects that are
//0xffffffffffff in size (thats 281 trillion bytes) 
{
    int i;
    ASFOBJECTHEADER ObjectHeader;
    WORD returnValue=0;
    for(i=0;i<iMaxObjects;i++)
    {
        //read the GUID in
        GetWmaMetaData_ReadObjectHeaderPrologue(iHandle,&ObjectHeader);
        if(GetWmaMetaData_CompareGUID(&ObjectHeader.ObjectGUID,pGUID))
        {
            returnValue=ObjectHeader.ObjectSize;
            break;//they match, lets stop seeking.
        }
        else
        {
            fseek(iHandle,ObjectHeader.ObjectSize-24,SEEK_CUR);//skip to the next object.
        }
    }
    return returnValue;
}



DWORD _reentrant GetWmaMetaData_FindFirstObject(BYTE iHandle, int iMaxObjects,GUID *pGUID)
{
    fseek(iHandle,WMA_HEADER_OBJECT_HEADER_SIZE,SEEK_SET);

    return GetWmaMetaData_FindNextObject(iHandle,iMaxObjects,pGUID);
}


void  _reentrant GetWmaMetaData_ReadString(BYTE iHandle, UCS3 *pPointer, WORD wBufferLength,WORD wStoredLength)
{
    int i;
    UCS3 *pTmpPtr = pPointer;
    UCS3 wData;
    if(pPointer)
    {
        for(i=0;i<wStoredLength;i++)
        {
            wData = GetWmaMetaData_ReadWORD(iHandle);
            if(i<wBufferLength)
            {
                *pTmpPtr++ = wData;
            }
            else
            {
                //don't store the word into the string (but we still want to read the whole string out)
            }
        }
        pPointer[wBufferLength-1] = 0;  //terminate the string, just in case there's no zero at the end.
    }

}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetWmaMetaData
//
//   Type:          Function
//
//   Description:   Finds the meta-data within a wma file.
//
//   Inputs:        Ptr to pathname, current device id, ptr to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant GetWmaMetaData(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA * MetaData)
{
    int     bContinue = FALSE;
    int     iError;
    RETCODE rtn = META_DATA_NOT_FOUND;
    ASFOBJECTHEADER HeaderHeader;
    GUID    TempGUID;
    WORD    wObjectCount=0;
    int     iHandle;
    _packed BYTE * pcFilePathName = (_packed BYTE*)PackedPathNameAddress;
    
    WORD TmpVal[3];

    // Open the file.
    iHandle = FSFileOpen(pcFilePathName, READ, btCurrentDevice);
    if(iHandle != ERROR)
    {
        MetaData->iFileType = (INT)WMA_TYPE;
        MetaData->iResourceNum = -1;//RSRC_WMADEC_CODE;
        MetaData->bVBR = FALSE;
        iError = fseek(iHandle,0,SEEK_SET);
        //verify that that header object is there.
        if(iError != -1)
        {
            rtn = META_DATA_FOUND;
            TempGUID.wValues[0]=ASF_HEADER_GUID_0;
            TempGUID.wValues[1]=ASF_HEADER_GUID_1;
            TempGUID.wValues[2]=ASF_HEADER_GUID_2;
            TempGUID.wValues[3]=ASF_HEADER_GUID_3;
            TempGUID.wValues[4]=ASF_HEADER_GUID_4;
            TempGUID.wValues[5]=ASF_HEADER_GUID_5;

            GetWmaMetaData_ReadObjectHeaderPrologue(iHandle,&HeaderHeader);
            if(GetWmaMetaData_CompareGUID(&HeaderHeader.ObjectGUID,&TempGUID))
            {
                wObjectCount = GetWmaMetaData_ReadDWORD(iHandle);
                GetWmaMetaData_ReadBYTE(iHandle);
                if(GetWmaMetaData_ReadBYTE(iHandle)==0x02)
                {
                    bContinue = TRUE;
                }
            }
        }
        else
          rtn = META_DATA_FILESEEK_ERROR;
        if(bContinue)
        {
            int wSize;
            //we've got a header object, so lets start getting objects out of it
            //first, the 
            TempGUID.wValues[0]=ASF_FILE_PROPERTIES_GUID_0;
            TempGUID.wValues[1]=ASF_FILE_PROPERTIES_GUID_1;
            TempGUID.wValues[2]=ASF_FILE_PROPERTIES_GUID_2;
            TempGUID.wValues[3]=ASF_FILE_PROPERTIES_GUID_3;
            TempGUID.wValues[4]=ASF_FILE_PROPERTIES_GUID_4;
            TempGUID.wValues[5]=ASF_FILE_PROPERTIES_GUID_5;
            if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
            {
              // The following code should work properly, but doesn't because the Windows Media
              // Player messes up the File Size parameter (adds a 00100 to it).  Thus, we'll use
              // our own file size function.
              //  fseek(iHandle,16,SEEK_CUR);//skip the FILE ID
              //  MetaData->dwNumBytes = GetWmaMetaData_ReadQWORD(iHandle);
                MetaData->dwNumBytes = FSFileSize(iHandle);
                MetaData->dwDataBytes = (MetaData->dwNumBytes - HeaderHeader.ObjectSize);
                // Skip the File ID, the File Size, and the creation date. (16 + 8 + 8)
                fseek(iHandle,32,SEEK_CUR);

                MetaData->dwNumFrames= GetWmaMetaData_ReadQWORD(iHandle);
                MetaData->iTime      = GetWmaMetaData_ReadQWORD(iHandle)/10000;
                fseek(iHandle,8,SEEK_CUR);//skip the send duration
                MetaData->iTime     -= GetWmaMetaData_ReadQWORD(iHandle);//subtract off the preroll amount
                MetaData->iTime     /= 1000; //convert the time to seconds

                TempGUID.wValues[0]=ASF_STREAM_PROPERTIES_GUID_0;
                TempGUID.wValues[1]=ASF_STREAM_PROPERTIES_GUID_1;
                TempGUID.wValues[2]=ASF_STREAM_PROPERTIES_GUID_2;
                TempGUID.wValues[3]=ASF_STREAM_PROPERTIES_GUID_3;
                TempGUID.wValues[4]=ASF_STREAM_PROPERTIES_GUID_4;
                TempGUID.wValues[5]=ASF_STREAM_PROPERTIES_GUID_5;
            }
            else
            {
                bContinue = FALSE;
            }
        }
            //get information from the STREAM object
        if(bContinue)
        {
            int wSize;
            wSize = GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID);
            while(wSize > 0)
            {

                GUID TempGUID2;
                TempGUID2.wValues[0] = ASF_AUDIO_MEDIA_GUID_0;
                TempGUID2.wValues[1] = ASF_AUDIO_MEDIA_GUID_1;
                TempGUID2.wValues[2] = ASF_AUDIO_MEDIA_GUID_2;
                TempGUID2.wValues[3] = ASF_AUDIO_MEDIA_GUID_3;
                TempGUID2.wValues[4] = ASF_AUDIO_MEDIA_GUID_4;
                TempGUID2.wValues[5] = ASF_AUDIO_MEDIA_GUID_5;

                FSFileRead(16,iHandle,MEM_SPACE_Y,-1,(void*)&TempGUID);
                if(GetWmaMetaData_CompareGUID(&TempGUID,&TempGUID2))
                {
                    break;
                }
                //seek to the next object
                fseek(iHandle,wSize-16,SEEK_CUR);
                wSize = GetWmaMetaData_FindNextObject(iHandle,wObjectCount,&TempGUID);
            }
            if(wSize > 0)
            {//wSize will be positive when we've found our stream
                iError = fseek(iHandle,16+8+4+4+2+4+2+2,SEEK_CUR);//skip past error correction type, time offset, type specific data length(since we know what
                                                        //it is),error correction data length, flags, reserved
                                                        //plus parts of the codec specific data (codec,channels)
                MetaData->wSampRate= GetWmaMetaData_ReadDWORD(iHandle);    
                MetaData->wBitRate = GetWmaMetaData_ReadDWORD(iHandle)*8;    
            }
            else
            {
                bContinue = FALSE;
            }
        }
            //now get the song information, etc.
        if(bContinue)
        {
            TempGUID.wValues[0]=ASF_CONTENT_DESCRIPTION_GUID_0;
            TempGUID.wValues[1]=ASF_CONTENT_DESCRIPTION_GUID_1;
            TempGUID.wValues[2]=ASF_CONTENT_DESCRIPTION_GUID_2;
            TempGUID.wValues[3]=ASF_CONTENT_DESCRIPTION_GUID_3;
            TempGUID.wValues[4]=ASF_CONTENT_DESCRIPTION_GUID_4;
            TempGUID.wValues[5]=ASF_CONTENT_DESCRIPTION_GUID_5;
            if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
            {
                int wTitleLength = GetWmaMetaData_ReadWORD(iHandle)/2;
                int wArtistLength = GetWmaMetaData_ReadWORD(iHandle)/2;

                fseek(iHandle,2+2+2,SEEK_CUR);//seek passed Copyright Length, Description Length, Rating Length

                GetWmaMetaData_ReadString(iHandle, MetaData->wTitle,MetaData->iTitleBufferLength,wTitleLength);
                GetWmaMetaData_ReadString(iHandle, MetaData->wArtist,MetaData->iArtistBufferLength,wArtistLength);

                //Find the album information
                if(MetaData->wAlbum)
                {
                    TempGUID.wValues[0]=ASF_EXTENDED_CONTENT_GUID_0;
                    TempGUID.wValues[1]=ASF_EXTENDED_CONTENT_GUID_1;
                    TempGUID.wValues[2]=ASF_EXTENDED_CONTENT_GUID_2;
                    TempGUID.wValues[3]=ASF_EXTENDED_CONTENT_GUID_3;
                    TempGUID.wValues[4]=ASF_EXTENDED_CONTENT_GUID_4;
                    TempGUID.wValues[5]=ASF_EXTENDED_CONTENT_GUID_5;
                    if(GetWmaMetaData_FindFirstObject(iHandle,wObjectCount,&TempGUID)>0)
                    {
                        int i;
                        int wContentDescriptorCount ;
                        UCS3 sDescriptorName[15];
                        wContentDescriptorCount = GetWmaMetaData_ReadWORD(iHandle);
                        for(i=0;i<wContentDescriptorCount;i++)
                        {
                            int wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle)/2;
                            GetWmaMetaData_ReadString(iHandle, sDescriptorName, 14,wDescriptorNameLength);
                            if(sDescriptorName[ 0]=='W' && 
                               sDescriptorName[ 1]=='M' &&
                               sDescriptorName[ 2]=='/' &&
                               sDescriptorName[ 3]=='A' &&
                               sDescriptorName[ 4]=='l' &&
                               sDescriptorName[ 5]=='b' &&
                               sDescriptorName[ 6]=='u' &&
                               sDescriptorName[ 7]=='m' &&
                               sDescriptorName[ 8]=='T' &&
                               sDescriptorName[ 9]=='i' &&
                               sDescriptorName[10]=='t' &&
                               sDescriptorName[11]=='l' &&
                               sDescriptorName[12]=='e')
                            {
                                if(GetWmaMetaData_ReadWORD(iHandle)==0)//must be a unicode string
                                {
                                    wDescriptorNameLength = GetWmaMetaData_ReadWORD(iHandle)/2;
                                    GetWmaMetaData_ReadString(iHandle, MetaData->wAlbum,MetaData->iAlbumBufferLength,wDescriptorNameLength);
                                }
                                break;
                            }
                            else
                            {//skip over this extended content descriptor
                                fseek(iHandle,2,SEEK_CUR);//seek past the data type
                                //get the descriptor value length, and skip past it
                                fseek(iHandle,GetWmaMetaData_ReadWORD(iHandle),SEEK_CUR);
                            }
                        }
                    }
                }

            }
        }
        if(iHandle != -1)
        {
            FSFileClose(iHandle);
        }

    }
    else
      rtn = META_DATA_FILEOPEN_ERROR;
    return rtn;
}
