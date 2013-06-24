#include "fstypes.h"
#include "fsproj.h"
#include "fsapi.h"
#include "platform.h"
#include "FileSystem.h"
#include "battery_config.h"
#include "battery.h"

////////////////////////EXTERN/////////////////////////////////

extern long g_UserTimeoffset;
extern unsigned long ReadRTC(void);
//;///////////////////////////////////////////////////////////////////////////////

////////////////////////GLOBAL/////////////////////////////////
void LoadUserTime(void);
void SaveUserTime(void);
void  FSSwizzleText(_packed char *pText);
_packed char settingfile[]="A:/TIME.DAT";

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           FSSwizzleText(_packed char *pText)

//   Type:           Function
//
//   Description:    TSwizzle the text

//   Inputs:       _packed char *pText

//   Outputs:        none
//
////////////////////////////////////////////////////////////////////////////////

void  FSSwizzleText(_packed char *pText)
{
    int bDone=0;
    unsigned int *pTmp=(unsigned int*)pText;
    unsigned int tmp;
    while(!bDone)
    {
        tmp = *pTmp;
        tmp = ((tmp & 0x0000ff)<<16) | ((tmp & 0x00ff00)) | ((tmp & 0xff0000)>>16);
        *pTmp++=tmp;
        if(!(tmp&0x0000ff) || !(tmp&0x00ff00)  || !(tmp&0xff0000))
            bDone = 1;
    }
}


/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            LoadUserTime
; Type:             Function

; Inputs:           None

; outputs:          None
;
; Notes:            This function load varaible g_UserTimeoffset from file time.dat

;
;<
;///////////////////////////////////////////////////////////////////////////////
*/

#ifdef FUNCLET
    #pragma asm
        org p,".ptextsaveusertime":
        global FLoadUserTime
        FLoadUserTime:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_LOADUSERTIME

        org p,"SYSFUNCLET_LOADUSERTIME_P":
        dc      RSRC_FUNCLET_LOADUSERTIME
    #pragma endasm

void LoadUserTime_funclet(void)
#else
void LoadUserTime(void)
#endif
{
    int Handle,Attribute;
    long crt_time;

    g_UserTimeoffset = 0;

	FSSwizzleText(settingfile);

    if((Handle = Fopen(settingfile,(_packed char*)"r")) <0)
    {
        // The file may not exist, try to create one.
    	FSSwizzleText(settingfile);
        SaveUserTime();
        return;
    }

    if(Fread(Handle,(INT*)&g_UserTimeoffset,6,Y_MEMORY,-1)!=6)
        return;

	crt_time = ReadRTC();
	crt_time /= (long)1000;	//Start time now has the number of seconds
   crt_time=(long)crt_time + (long)g_UserTimeoffset;

   if((crt_time)<0)
   {
        g_UserTimeoffset=0;

        Fclose(Handle);

        Attribute= (READ_ONLY|SYSTEM|HIDDEN);
        FSFileAttributeClear(Attribute,settingfile,0);

        if((Handle = Fopen(settingfile,(_packed char*)"r+")) <0)
            return;

        if(Fwrite(Handle,(INT *)&g_UserTimeoffset,6,Y_MEMORY,-1)!=6)
            return;
        Attribute=(READ_ONLY|SYSTEM|HIDDEN);
        FSFileAttribute(Attribute,settingfile,0);

   }

   Fclose(Handle);
	FSSwizzleText(settingfile);
}
/*
;///////////////////////////////////////////////////////////////////////////////
;> Name:            SaveUserTime

; Type:             Function

; Inputs:           None

; outputs:          None
;
; Notes:            This function Write the  varaible g_UserTimeoffset to file time.dat
                    at the time of player Shutdown.

;
;<
;///////////////////////////////////////////////////////////////////////////////
*/
#ifdef FUNCLET
    #pragma asm
        org p,".ptextsaveusertime":
        global FSaveUserTime
        FSaveUserTime:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SAVEUSERTIME

        org p,"SYSFUNCLET_SAVEUSERTIME_P":
        dc      RSRC_FUNCLET_SAVEUSERTIME
    #pragma endasm

void SaveUserTime_funclet()
#else
void SaveUserTime()
#endif
{
    int Handle,Attribute;

    if(SysBatteryGetLevel()<=0)
        return;

	FSSwizzleText(settingfile);
    Attribute= (READ_ONLY|SYSTEM|HIDDEN);
    FSFileAttributeClear(Attribute,settingfile,0);

    if((Handle = Fopen(settingfile,(_packed char*)"w")) <0)
    {
        return;
    }

//    if(Fseek(Handle,USER_TIME_OFFSET,SEEK_SET))
//        return;

    if(Fwrite(Handle,(INT *)&g_UserTimeoffset,6,Y_MEMORY,-1)!=6)
        return;
    Attribute=(READ_ONLY|SYSTEM|HIDDEN);
    FSFileAttribute(Attribute,settingfile,0);
   	FSSwizzleText(settingfile);

    Fclose(Handle);
  }




