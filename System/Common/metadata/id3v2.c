#include "types.h"
#include "stringlib.h"
#include "hwequ.h"
#include "project.h"
#include "FileSystem.h"
#include "metadata.h"
#include "resource.h"
#include "dbcs.h"
#include "id3v2_internal.h"
#ifdef USE_PLAYLIST3
#include "playlist3.h"
#endif
#ifdef JPEG_ALBUM_ART
#include "attachedpictureframe.h"
#include "menumanager.h"
#endif
#ifdef SYNC_LYRICS
#include "menumanager.h"
#include "synclyrics.h"
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant Mp3MetaDataReadID3V2(INT iHandle,INT iIgnored, FILE_META_DATA*pMetaData)
//
//   Type:          Function (code bank callable)
//
//   Description:   Attempts to read an ID3V2 tag from a file starting at the
//                  current position.
//
//   Inputs:        INT iHandle:                Handle to the file pointer,
//                  INT iIgnored:               There because of codebank callable requirements.
//                  FILE_META_DATA*pMetaData:   pointer to meta-data struct.
//
//   Outputs:       RETCODE
//
//   Notes:         (it also leaves the file position at the end of the
//                  ID3V2 tagging information, or where we entered if there
//                  was no ID3V2 tag)
//<
////////////////////////////////////////////////////////////////////////////////
int _reentrant Mp3MetaDataReadID3V2(INT iHandle,INT iIgnored, FILE_META_DATA*pMetaData)
{
    //remember the current position in the file.
    WORD uHeaderSize=0;
    BOOL bSyncSafe = FALSE;
    LONG uCurrentPos = ftell(iHandle);
    if(uCurrentPos != -1)
    {
        WORD uTemp;
        if(FSFileRead(3, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp)== 3)
        {
            if(uTemp == 0x334449)//'ID3'
            {//we've got a ID3 V2 tag at the beginning
                uTemp = 0;
                if(FSFileRead(2, iHandle, MEM_SPACE_Y, (INT)(-1), &uTemp)== 2)
                {
                    if(((uTemp&0x00ff) <= 4) && ((uTemp&0xff00) != 0xff00))
                    {   //its a version we support
                        WORD uFlags=0;
                        //read the flags.
                        FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), &uFlags);
                        // Decide whether frame lengths must be read as Sync Safe Integers.
                        uTemp &= 0xff;      // Only keep the Low Byte.
                        bSyncSafe = uTemp<4 ? FALSE : TRUE; // Sync-Safe Integer reads in V2.4
                        //get the header size
                        uHeaderSize = 10 + Id3V2ReadInt(iHandle, TRUE, V2_ID3_TAGSIZE);  // Header always SyncSafe
                        if((uTemp&0x00ff) == 4) { // V2.4
							//add 10 to the header size if the header has a footer
							if(uFlags & ID3V2_HEADER_FLAG_FOOTER)
								uHeaderSize += 10;
						}
						//update the start position of the MP3 data (must be after the ID3V2Header and padding)
						pMetaData->dwStartPos+=uHeaderSize;
                        //update the length of the song (its shorter by the header size now)
                        pMetaData->dwNumBytes-=uHeaderSize;
                        if((uTemp&0x00ff) == 2) { // V2.2
							if(!(uFlags & ID3V2_HEADER_UNSYNCRONIZATION))
							{//we don't support unsyncronization
								//Id3V2ReadFrame will return FALSE if it does not find a valid Id3V2 Header Frame identifier.
								//(this will break out of the loop below)
								while(Id3V2_2ReadFrame(iHandle, pMetaData, bSyncSafe))
                                ;
							}
						} else { // V2.3 and V2.4
							if(uFlags & ID3V2_HEADER_EXTENDED_HEADER)
							{   //if it has an extended header, consume it.  (We don't care about the contents)
								WORD uLength = Id3V2ReadInt(iHandle, bSyncSafe, V2_ID3_EXTENDED_HEADER_SIZE);
								fseek(iHandle, uLength-4,SEEK_CUR);
							}
							if(!(uFlags & ID3V2_HEADER_UNSYNCRONIZATION))
							{//we don't support unsyncronization
								//Id3V2ReadFrame will return FALSE if it does not find a valid Id3V2 Header Frame identifier.
								//(this will break out of the loop below)
								while(Id3V2_3and2_4ReadFrame(iHandle, pMetaData, bSyncSafe))
                                ;
							}
						}
                    }
                }


            }
        }
        //seek past the ID3V2.4 header, or back to where we started if there was no tag present.
        fseek(iHandle,uCurrentPos + uHeaderSize,SEEK_SET);

    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BOOL _reentrant Id3V2_2ReadFrame(INT iHandle, FILE_META_DATA*pMetaData, BOOL bSyncSafe)
//
//   Type:          Function
//
//   Description:   Attempts to read an ID3V2.2 frame
//
//   Inputs:        INT iHandle:                Handle to the file pointer,
//                  FILE_META_DATA*pMetaData:   pointer to meta-data struct.
//                  BOOL bSyncSafe:             Decide whether this is a sync-safe integer read.
//
//   Outputs:       BOOL:   TRUE if frame was found
//                          FALSE if no frame was found
//
//   Notes:         Advances file position to after where the frame ends if frame found, or back to
//                  where it started reading if no valid frame found.
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant Id3V2_2ReadFrame(INT iHandle, FILE_META_DATA*pMetaData, BOOL bSyncSafe)
{
    BOOL bValidFrame = FALSE;
    WORD uFrameType= 0;//Need to initialize because FSFileRead only reads data into the current word
    WORD uFrameSize;

#ifdef USE_PLAYLIST3
    WORD pTracknum[TRACKNUM_SIZE];
    WORD TracknumSize;
    WORD TmpVal[3];
    WORD* pTmp;
#endif


    FSFileRead(3, iHandle, MEM_SPACE_Y, (INT)(-1), &uFrameType);
    uFrameSize = Id3V2ReadInt(iHandle, bSyncSafe, V2_2_FRAMESIZE);
    if(((uFrameType&0x0000ff) >= 'A' && (uFrameType&0x0000ff) <= 'Z') || ((uFrameType&0x0000ff) >='0' && (uFrameType&0x0000ff) <='9'))
    {
		LONG lStartOfFrame;
		bValidFrame = TRUE;
        lStartOfFrame = ftell(iHandle);//remember where the start of the frame data is
		if((uFrameType&0x0000ff) == 'T')
		{//If its a text frame, lets process it this way to save a little memory.
			UCS3* pString=NULL;
			WORD  iBufferLength=0;
			switch(uFrameType)
			{
				case TAL://The Album name
					pString = pMetaData->wAlbum;
					iBufferLength = pMetaData->iAlbumBufferLength;
					break;
				case TT2://The title of the composition
					pString = pMetaData->wTitle;
					iBufferLength = pMetaData->iTitleBufferLength;
					break;
				case TP1://The artist
					pString = pMetaData->wArtist;
					iBufferLength = pMetaData->iArtistBufferLength;
					break;

#ifdef USE_PLAYLIST3
				case TRK:
				    TmpVal[0] = g_Rebuild;
                    g_Rebuild = 0;
                    Id3V2ReadVariedStringToUnicodeBuffer(iHandle,0,uFrameSize,pTracknum,TRACKNUM_SIZE);
                    g_Rebuild = TmpVal[0];

                    for (pTmp=pTracknum,TracknumSize=0; (*pTmp!=0)&&(*pTmp!='/'); pTmp++)
                        TracknumSize++;

                    TmpVal[0]=0;
                    TmpVal[1]=1;
                    while (TracknumSize)
                    {
                        pTmp--;
                        TmpVal[2] = *pTmp - '0';
                        TmpVal[0] += TmpVal[1]*TmpVal[2];
                        TmpVal[1] *= 10;
                        TracknumSize--;
                    }

                    pMetaData->wTrackNum = TmpVal[0];
				    break;
#endif

					//if you want it to read in a new text frame, set up pString and iBuffer to point to your
					//own UCS3 string and length.
				default:
					pString=NULL;
					iBufferLength=0;
					break;
			}
			if(pString)//if we've set up pString, go and read it...
				Id3V2ReadVariedStringToUnicodeBuffer(iHandle,0,uFrameSize,pString,iBufferLength);

			//Some of the frames aren't strings, but numbers stored as text strings (i.e. "100" instead of 100.
			//these don't fit the above processing, so you'll have to rewrite the algorithm a bit.
		}
#ifdef JPEG_ALBUM_ART
		else if ((uFrameType&0x0000ff) == 'P')
		{
	    	if (uFrameType == PIC)	// The attached picture (album art)
	    		SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicV2_2ReadFrame, iHandle, uFrameSize, NULL);
		}
#endif
#ifdef SYNC_LYRICS
        else if ((uFrameType&0x0000ff) == 'S') {	// Extract ID3V2 Lyric's
			if (uFrameType == SLT)
        		SysCallFunction(RSRC_ID3V2_LYRICS_CODEBANK, Id3V2LyricsReadFrame, iHandle, uFrameSize, NULL);
    	}
#endif
        else
        {
            fseek(iHandle, uFrameSize, SEEK_CUR);//seek to the next frame.
        }
        //if you want to handle a new type of frame, put that here!
        fseek(iHandle, lStartOfFrame+uFrameSize, SEEK_SET);//go to the start of next frame data
    }
    else
    {
        fseek(iHandle, -3, SEEK_CUR);//seek to where we started reading
    }
    return bValidFrame;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          BOOL _reentrant Id3V2_3and2_4ReadFrame(INT iHandle, FILE_META_DATA*pMetaData, BOOL bSyncSafe)
//
//   Type:          Function
//
//   Description:   Attempts to read an ID3V2.3 and ID3v2.4 frame
//
//   Inputs:        INT iHandle:                Handle to the file pointer,
//                  FILE_META_DATA*pMetaData:   pointer to meta-data struct.
//                  BOOL bSyncSafe:             Decide whether this is a sync-safe integer read.
//
//   Outputs:       BOOL:   TRUE if frame was found
//                          FALSE if no frame was found
//
//   Notes:         Advances file position to after where the frame ends if frame found, or back to
//                  where it started reading if no valid frame found.
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant Id3V2_3and2_4ReadFrame(INT iHandle, FILE_META_DATA*pMetaData, BOOL bSyncSafe)
{
    BOOL bValidFrame = FALSE;
    WORD uFrameType= 0;//Need to initialize because FSFileRead only reads data into the current word
    WORD uFrameFlags=0;//and does not initialize the rest of the word.
    WORD uFrameSubType;
    WORD uFrameSize;

#ifdef USE_PLAYLIST3
    WORD pTracknum[TRACKNUM_SIZE];
    WORD TracknumSize;
    WORD TmpVal[3];
    WORD* pTmp;
#endif



    FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), &uFrameType);
    FSFileRead(3, iHandle, MEM_SPACE_Y, (INT)(-1), &uFrameSubType);
    uFrameSize = Id3V2ReadInt(iHandle, bSyncSafe, V2_3_AND_V2_4_FRAMESIZE);
    FSFileRead(2, iHandle, MEM_SPACE_Y, (INT)(-1), &uFrameFlags);
    if((uFrameType >= 'A' && uFrameType <= 'Z') || (uFrameType >='0' && uFrameType <='9'))
    {
        bValidFrame = TRUE;
        if(!(uFrameFlags & (ID3V2_FRAME_HEADER_UNSYNCRONIZATION|ID3V2_FRAME_HEADER_COMPRESSION|ID3V2_FRAME_HEADER_ENCRYPTION)))
        {//we don't support Unsync, Compressed, or Encrypted frames.
            LONG lStartOfFrame;
            lStartOfFrame = ftell(iHandle);//remember where the start of the frame data is
            if(uFrameType=='T')
            {//If its a text frame, lets process it this way to save a little memory.
                UCS3* pString=NULL;
                WORD  iBufferLength=0;
                switch(uFrameSubType)
                {
                case TALB://The Album name
                    pString = pMetaData->wAlbum;
                    iBufferLength = pMetaData->iAlbumBufferLength;
                    break;
                case TIT2://The title of the composition
                    pString = pMetaData->wTitle;
                    iBufferLength = pMetaData->iTitleBufferLength;
                    break;
                case TPE1://The artist
                    pString = pMetaData->wArtist;
                    iBufferLength = pMetaData->iArtistBufferLength;
                    break;
#ifdef USE_PLAYLIST3
				case TCON://The genre
					pString = pMetaData->wGenre;
					iBufferLength = pMetaData->iGenreBufferLength;
					break;
				case TYER://The year
					pString = pMetaData->wYear;
					iBufferLength = pMetaData->iYearBufferLength*3;
					break;
                case TRCK:
                    TmpVal[0] = g_Rebuild;
                    g_Rebuild = 0;
                    Id3V2ReadVariedStringToUnicodeBuffer(iHandle,uFrameFlags,uFrameSize,pTracknum,TRACKNUM_SIZE);
                    g_Rebuild = TmpVal[0];

                    for (pTmp=pTracknum,TracknumSize=0; (*pTmp!=0)&&(*pTmp!='/'); pTmp++)
                        TracknumSize++;

                    TmpVal[0]=0;
                    TmpVal[1]=1;
                    while (TracknumSize)
                    {
                        pTmp--;
                        TmpVal[2] = *pTmp - '0';
                        TmpVal[0] += TmpVal[1]*TmpVal[2];
                        TmpVal[1] *= 10;
                        TracknumSize--;
                    }

                    pMetaData->wTrackNum = TmpVal[0];
                    break;
#endif
                //if you want it to read in a new text frame, set up pString and iBuffer to point to your
                    //own UCS3 string and length.
                default:
                    pString=NULL;
                    iBufferLength=0;
                    break;
                }
                if(pString)//if we've set up pString, go and read it...
                    Id3V2ReadVariedStringToUnicodeBuffer(iHandle,uFrameFlags,uFrameSize,pString,iBufferLength);

                //Some of the 'Txxx' frames aren't strings, but numbers stored as text strings (i.e. "100" instead of 100.
                //these don't fit the above processing, so you'll have to rewrite the algorithm a bit.
            }
#ifdef JPEG_ALBUM_ART
            else if(uFrameType=='A')
            {
            	if (uFrameSubType == APIC)	// The attached picture (album art)
            		SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicV2_3and2_4ReadFrame, iHandle, uFrameSize, NULL);
            }
#endif
#ifdef SYNC_LYRICS
			// Extract ID3V2 Lyric's
            else if(uFrameType=='S')	{
            	if (uFrameSubType == SYLT)
            		SysCallFunction(RSRC_ID3V2_LYRICS_CODEBANK, Id3V2LyricsReadFrame, iHandle, uFrameSize, NULL);
        	}
#endif
            //if you want to handle a new type of frame, put that here!

            fseek(iHandle, lStartOfFrame+uFrameSize, SEEK_SET);//go back to the start of the frame data
        }
        else
        {
            fseek(iHandle, uFrameSize, SEEK_CUR);//seek to the next frame.
        }
    }
    else
    {
        fseek(iHandle, -6, SEEK_CUR);//seek to where we started reading
    }
    return bValidFrame;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Id3V2ReadInt(INT iHandle, BOOL bSyncSafe)
//
//   Type:          Function
//
//   Description:   Reads in an Integer from an ID3V2 tag
//                  Decision about whether this should be a Sync Safe read
//                  is dependent upon bSyncSafe.
//                  Note: Only Version 4 supports Sync Safe lengths in frames.
//                  Reference documents at www.id3.org
//
//   Inputs:        INT iHandle:                Handle to the file pointer,
//                  BOOL bSyncSafe:             Flag that this is a SyncSafe read.
//					INT sizeDescriptorBytes		Specifies no.of size descriptor bytes
//
//   Outputs:       The SyncSafe integer read
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
INT _reentrant Id3V2ReadInt(INT iHandle, BOOL bSyncSafe, INT sizeDescriptorBytes)
{
    INT i;
    INT iTemp = 0;
    INT iReturn =0;
    for(i=0;i<sizeDescriptorBytes;i++)
    {
        FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)&iTemp);
        if (!bSyncSafe)
        {
          iReturn = (iReturn << 8) + iTemp;   // Version 3 is not sync-safe.
        } else      // Assume future releases will include sync-safe integers.
        {
          iReturn = (iReturn << 7) + iTemp;   // Version 4 is sync-safe
        }
    }
    return iReturn;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Id3V2ReadVariedStringToUnicodeBuffer(INT iHandle, WORD uFlags, WORD uFrameSize,UCS3*pString,INT iBufferLength)
//
//   Type:          Function
//
//   Description:   Reads a string in from a TEXT ID3V2 tag frame.  These strings are prepended with a encoding byte
//
//   Inputs:        INT iHandle:                file handle
//                  WORD uFlags:                Frame flags
//                  WORD uFrameSize:            Size of the frame data
//                  UCS3*pString:               pointer to the buffer to write the string into
//                  INT iBufferLength:          length of the buffer pointed to
//
//   Outputs:       pString is filled in. (and null terminated)
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant Id3V2ReadVariedStringToUnicodeBuffer(INT iHandle, WORD uFlags, WORD uFrameSize,UCS3*pString,INT iBufferLength)
{
    INT iTemp=0;
    //Read in the encoding type
    FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)&iTemp);
    uFrameSize--;//subtract one off for the encoding type

    switch(iTemp)
    {
    case ENCODING_ISO_8559:
        {
            int iCounter=0;
            _packed BYTE TempString[MAX_STRING];
            TempString[0]=0;
            if(uFrameSize >= MAX_STRING)
                uFrameSize = MAX_STRING-1;
            if(uFrameSize >=iBufferLength)
                uFrameSize = iBufferLength-1;
            while(iCounter < MAX_STRING && iCounter < uFrameSize)//don't need to check for iBUfferlength overflow (handled below in DBCSToUnicode)
            {
                iCounter++;
                FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)&iTemp);
                if(iTemp)
                    packed_strcat(TempString,(_packed BYTE*)&iTemp);
            }
#ifdef USE_PLAYLIST3
			if (g_Rebuild == TRUE ) {
				// if string length is less than 2, convert it to Unicode of AddSongToLibrary() use, SGTL
				if (packed_strlen(TempString) > 2)
					packed_strcpy((_packed BYTE *)pString,TempString);
				else
					DBCSToUnicode(TempString,pString,iBufferLength);
			} else
#endif
            DBCSToUnicode(TempString,pString,iBufferLength);
        }
        break;
    case ENCODING_UTF_16:
        //read in the BOM
        FSFileRead(2, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)&iTemp);
        uFrameSize -=2;
        Id3V2ReadUTF16String(iHandle,iTemp,uFlags,uFrameSize,pString,iBufferLength);
        break;
    case ENCODING_UTF_16_NOBOM:
        Id3V2ReadUTF16String(iHandle,BOM_BIG_ENDIAN,uFlags,uFrameSize,pString,iBufferLength);
        break;
    }
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Id3V2ReadUTF16String(INT iHandle, WORD uEndianNess,WORD uFlags, WORD uFrameSize,UCS3*pString,INT iBufferLength)
//
//   Type:          Function
//
//   Description:   Reads a UTF-16 string in from a TEXT ID3V2 tag frame.
//
//   Inputs:        INT iHandle:                file handle
//                  WORD uEndianNess:           BOM_BIG_ENDIAN or BOM_LITTLE_ENDIAN
//                  WORD uFlags:                The flags of the frame that contains the string
//                  WORD uFrameSize:            Size of the frame data
//                  UCS3*pString:               pointer to the buffer to write the string into
//                  INT iBufferLength:          length of the buffer pointed to
//
//   Outputs:       pString is filled in. (and null terminated)
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant Id3V2ReadUTF16String(INT iHandle, WORD uEndianNess,WORD uFlags, WORD uFrameSize,UCS3*pString,INT iBufferLength)
{
    INT iTemp1=0;
    INT iTemp2=0;
    UCS3 uValue;
    INT iCounter=0;

    uFrameSize = (uFrameSize + 1)/2; //The frame size should be bytes, now its words.

    if(uFrameSize  >= iBufferLength)
        uFrameSize = iBufferLength -1;

    while( iCounter < uFrameSize)//don't need to check for iBUfferlength overflow (handled below in DBCSToUnicode)
    {
        FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)&iTemp1);
        FSFileRead(1, iHandle, MEM_SPACE_Y, (INT)(-1), (void*)&iTemp2);
        if(uEndianNess == BOM_BIG_ENDIAN)
            uValue = (iTemp1<<8) | (iTemp2);
        else
            uValue = (iTemp2<<8) | (iTemp1);
        pString[iCounter++]= uValue;
        pString[iCounter]= 0;//null terminate
    }
    return TRUE;
}