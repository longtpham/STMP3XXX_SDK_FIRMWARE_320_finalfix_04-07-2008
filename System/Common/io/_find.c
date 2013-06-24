#include "types.h"
#include "io.h"
#include "stringlib.h"
#include "filesystem.h"


#ifndef WORD_SIZE
#define WORD_SIZE 24
#endif

//optimize 1 makes it more difficult to debug 
#pragma optimize 1

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
    ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
#pragma endasm    
#endif    

#ifndef MAX_FIND_FIRSTS
#define MAX_FIND_FIRSTS 1
#endif 

struct FindInfo
{
    //the following are internal use pieces
    int     m_bInUse;
    int     m_iCurrentDevice;
    int     m_iCurrentDirEntry;//this is the one to be read on the next call to _findnext
    //The following are the search criteria
    int     m_iAttrib;
    _packed BYTE    m_szFileName[9];
    _packed BYTE    m_szExt[4];
};

struct FindInfo g_FindInfo[MAX_FIND_FIRSTS];


void dummy (void) 
{
}
#ifdef FUNCLET
#pragma asm
        F_findclose:

        nolist
        include "sysmacro.asm"
        list
        extern RSRC_FUNCLET_FINDCLOSE
        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_FINDCLOSE

        org p,"SYSFUNCLET_FINDCLOSE_P":
        dc      RSRC_FUNCLET_FINDCLOSE
#pragma endasm
    
        //Use the exact same prototype of the static function to convert
#define _findclose _findclose_Funclet
#endif
_reentrant int _findclose(int handle)
{

    if(handle<MAX_FIND_FIRSTS)
    {
    
        if(g_FindInfo[handle].m_bInUse)
        {
            g_FindInfo[handle].m_bInUse = FALSE;
            return 0;
        }
    }

    return -1;
}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptext_find":
    #pragma endasm
#endif



#ifdef FUNCLET
#pragma asm
        F_findnext:

        nolist
        include "sysmacro.asm"
        list
        extern RSRC_FUNCLET_FINDNEXT
        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_FINDNEXT

        org p,"SYSFUNCLET_FINDNEXT_P":
        dc      RSRC_FUNCLET_FINDNEXT
#pragma endasm
    
        //Use the exact same prototype of the static function to convert
#define _findnext _findnext_Funclet
#endif
_reentrant int _findnext(int handle, struct _finddata_t *fileinfo )
#undef _findnext
{
    int     iReturn=-1;
    BYTE    iChar;
    register struct FCB      *pFCB;         
    long FileSizeHigh, FileSizeLow; //they really are int, but long used to shift for bitwise OR operations

    if(handle<MAX_FIND_FIRSTS)
    {
        register struct FindInfo *pPtrToFindInfo = &g_FindInfo[handle];
        register _packed BYTE *pFindFilename;
        
        
        register _packed BYTE *pFindExt;
        pFindFilename   = pPtrToFindInfo ->m_szFileName;
        pFindExt        = pPtrToFindInfo->m_szExt;
        if(pPtrToFindInfo->m_bInUse)
        {
            while(1)
            {
                pFCB = ReadDirectory(pPtrToFindInfo->m_iCurrentDevice,pPtrToFindInfo->m_iCurrentDirEntry++);
                if(pFCB)
                {
                    register _packed BYTE *pFCBFileName = pFCB->m_szFileName;
                    register _packed BYTE *pFCBExt      = pFCB->m_szExt;

                    if(pFCB->m_wAttributes == 0x0F) 
                      {
                        continue;                       // skip long filename entries
                      }

                    iChar = packed_get(pFCB->m_szFileName,0);        
                    if(iChar=='.')
                    {//this is the current directory or the parent directory entry
                        continue;
                    }
                    if(iChar==0)
                    {//this is the end of the directory
                        break;
                    }
                    else
                    {
                        if(iChar!=0xe5)
                        {
                            if(iChar==0x05)//handle the e5/05 kanji fix
                              packed_set(pFCBFileName,0,0xe5);          // Note: This fix should only be done
                                                                        // with 8.3 ASCII/DBCS filenames

                            if(!packed_strncmp(pFCBFileName,pFindFilename,packed_strlen(pFindFilename)))
                            {//our filename filespec matched
                                if(!packed_strncmp(pFCBExt,pFindExt,packed_strlen(pFindExt)))
                                {//our extention filespec matched
                                    //lets copy the filename piece by piece
                                    int i,k;
                                    register _packed BYTE *pOutputFilename = fileinfo->name;
                                    for(i=0;i<8;i++)
                                    {
                                        iChar = packed_get(pFCBFileName,i);
                                        if(iChar ==' ')
                                        {
                                            break;  
                                        }
                                        else
                                        {
                                            packed_set(pOutputFilename,i,iChar);
                                        }

                                    }
                                    if(packed_get(pFCBExt,0)!= ' ')//if ext is not a space (meaning there is an extension)
                                        packed_set(pOutputFilename,i++,'.');
                                    for(k=0;k<3;k++)
                                    {
                                        iChar = packed_get(pFCBExt,k);
                                        if(iChar ==' ')
                                        {
                                            break;  
                                        }
                                        else
                                        {
                                            packed_set(pOutputFilename,i+k,iChar);
                                        }

                                    }
                                    packed_set(pOutputFilename,i+k,0);//null terminate it

                                    fileinfo->attrib = pFCB->m_wAttributes;
                                    fileinfo->device = pPtrToFindInfo->m_iCurrentDevice;
				    //Changed for stmp00003761
                                    fileinfo->startrecord = pPtrToFindInfo->m_iCurrentDirEntry;
                                    FileSizeHigh = (WORD)pFCB->m_wFileSizeHigh;
                                    FileSizeHigh <<= WORD_SIZE;
                                    FileSizeLow = (WORD)pFCB->m_wFileSizeLow;
                                    fileinfo->size = FileSizeHigh | FileSizeLow;
                                    iReturn=0;
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return iReturn;
}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptext_find":
    #pragma endasm
#endif


#ifdef FUNCLET
#pragma asm
        F_findfirst:

        nolist
        include "sysmacro.asm"
        list
    
        extern RSRC_FUNCLET_FINDFIRST

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_FINDFIRST

        org p,"SYSFUNCLET_FINDFIRST_P":
        dc      RSRC_FUNCLET_FINDFIRST
#pragma endasm
#define _findfirst _findfirst_Funclet
#endif
_reentrant int _findfirst(_packed BYTE *filespec, struct _finddata_t *fileinfo )
#undef _findfirst
{

    int i;
    int handle;
    UCS3 szFilespec[_MAX_FNAME];
    UCS3 *pStrPtr;
    struct FindInfo *pPtrToFindInfo;

    handle = -1;
    pPtrToFindInfo = NULL;
    if((packed_strlen(filespec))>(_MAX_FNAME-1))
    {
        return handle;//can't look for a file that is too big
    }

    //find an available FindInfo Block
    for(i=0;i<MAX_FIND_FIRSTS;i++)
    {
        if(!g_FindInfo[i].m_bInUse)
        {
            handle=i;
            pPtrToFindInfo = &g_FindInfo[handle];
            break;
        }
    }
    if(pPtrToFindInfo)
    {
        pPtrToFindInfo->m_iCurrentDirEntry=NULL;
        pPtrToFindInfo->m_iAttrib=fileinfo->attrib;
        pPtrToFindInfo->m_iCurrentDevice = fileinfo->device;
        //fill up the FileName search pattern
        unpack_string(filespec,szFilespec);
        pStrPtr = strchr(szFilespec,'.');
        pPtrToFindInfo->m_szExt[0]=0;
        if(pStrPtr)
        {//if we've got a '.', lets put the extension in its own attribute block
            UCS3 *pStrPtrTemp;
            pStrPtrTemp = strchr(pStrPtr, '*');
            if (pStrPtrTemp)
                *pStrPtrTemp = 0;   //lets end the string at * (makes comparisons easier with strncmp)
            pack_string(pStrPtr+1,pPtrToFindInfo->m_szExt);
            pStrPtr[0]=0;//lets block off the extention for the filename
        }
        //put the filespec into the filename
        if(strchr(szFilespec,'*'))
        {
            *strchr(szFilespec,'*')=0;//lets end the string at * (makes comparisons easier with strncmp)
        }
        pack_string(szFilespec,pPtrToFindInfo->m_szFileName);
        //
        pPtrToFindInfo->m_iCurrentDirEntry=fileinfo->startrecord;//start at where asked.
        pPtrToFindInfo->m_bInUse=TRUE;

        if(!_findnext(handle,fileinfo))
        {

        }
        else
        {
            pPtrToFindInfo->m_bInUse=FALSE;
            handle=-1;
        }
    }
    return handle;
}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptext_find":
    #pragma endasm
#endif

