/*----------------------------------------------------------------------------
 SigmaTel Inc
 $Archive: /Fatfs/FileSystem/Fat32/Utility/FSUnicode.c $                                        
 $Revision: 10 $                                       
 $Date: 9/13/03 7:07p $                                           
 Description: FSUnicode.c
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
		File Includes
----------------------------------------------------------------------------*/

#include "types.h"
#include "fstypes.h"
#include "fserror.h"

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm
#endif


extern INT FSGetByte(void *buffer, INT Offset,INT MemoryType);
extern INT FSGetWord(void *buffer, INT Offset,INT MemoryType);
extern void PutByte(void *buffer, INT byte,INT Offset ,INT MemoryType);
extern void PutWord(void *buffer, INT word, INT Offset,INT MemoryType);


INT Long_NameRes_Ch[10]={0x22,0x2A,0x2F,0x3A,0x3C,0x3E,0x3F,0x5C,0x7C}; 
INT Short_NameRes_Ch[6]={0X2B,0X2C,0X3B,0X3D,0X5B,0X5D};  

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}


/*----------------------------------------------------------------------------

>  Function Name:  _reentrant void DBCStoUnicode(_packed char *filepath,_packed char *buf,INT index,INT length)

   FunctionType:   Reentrant

   Inputs:         1) Pointer to file name
				   2) Pointer to buffer
				   3) Index   (starting point for string)
                   4) Length  (Ending point for string)

  Outputs:         Nil
     
  Description:     Convert the given DBCS string to unicode
----------------------------------------------------------------------------*/

#ifdef FUNCLET
    #pragma asm
        FDBCStoUnicode:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_DBCSTOUNICODE

        org p,"SYSFUNCLET_DBCSTOUNICODE_P":
        dc      RSRC_FUNCLET_DBCSTOUNICODE
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
void _reentrant DBCStoUnicode_Funclet(_packed char *filepath,_packed char *buf,INT index,INT length)
#else
void _reentrant DBCStoUnicode(_packed char *filepath,_packed char *buf,INT index,INT length)
#endif

{
   INT offset=0,word=0,i;
   INT Byte;

  for(i=index;i<length;i++)
   {
   Byte = FSGetByte(filepath,i,DEFAULT_MEMORY);
   if(Byte=='/')
	Byte=0;
   PutByte(&word,Byte,0,DEFAULT_MEMORY);
   PutWord(buf,word,offset,DEFAULT_MEMORY); 
   offset+=2; 
   } 
   PutWord(buf,0,offset,DEFAULT_MEMORY); 
 }

#ifdef FUNCLET
    #pragma asm
        org p,".ptextFsunicode":
    #pragma endasm
#endif


/*----------------------------------------------------------------------------

>  Function Name:  Void  UnicodeToOEM(_packed char *file,_packed char *shortname,INT length,INT index)

   FunctionType:   Reentrant

   Inputs:         1) Ponter to file name
                   2) Ponter to Character
                   3) Length  (Ending point for string)
                   4) Index   (strating point for string)
                   
   
   Outputs:       None
      
 ----------------------------------------------------------------------------*/
              
#ifdef FUNCLET
    #pragma asm
        FUnicodeToOEM:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_UNICODETOOEM

        org p,"SYSFUNCLET_UNICODETOOEM_P":
        dc      RSRC_FUNCLET_UNICODETOOEM
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
void _reentrant UnicodeToOEM_Funclet(_packed char *file,_packed char *shortname,INT length,INT index)
#else
void _reentrant UnicodeToOEM(_packed char *file,_packed char *shortname,INT length,INT index)
#endif

{
INT offset=0,j,k=0;
INT word=0;
INT highbyte=0;
  offset=index;
  while(offset<length) // offset<length is 3.05 ver.  sdk2.600 uses k<length
  {
   word =FSGetWord(file,offset,DEFAULT_MEMORY);

    //start 3.095.RC0 block which modified the sgtl-HK version that is on sdk2.600
        // Modified By SGTL-HK, 20-5-2005
	if (word == 0x0000)
		break;
    highbyte = ((word & 0xff00) >> 8);
		//	Modified By SGTL-HK, 16-2-2005
	word = word & 0xFF;
	if (word == 0x00)
		word = 0x5F;
	for (j = 0; j < LONGNAMERES_CH; j++)
	{
		if (word == Long_NameRes_Ch[j])
		word = 0x5F;
	}
	//if (word > 0x7F)
	//	word = 0x5F;
    //end 3.095.RC0 block.


   for(j=0;j<SHORTNAMERES_CH;j++)
	{
	 if(word == Short_NameRes_Ch[j])
	 	word = 0x5F;
    }

    if (((word==' ')||(word=='.'))&&(highbyte!=0))
        word = 0x5f;

     PutByte(shortname,word,k,DEFAULT_MEMORY);
  	 offset+=2; 
	 k++;
  } 
  PutByte(shortname,0,k,DEFAULT_MEMORY);
}

