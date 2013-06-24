////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. //
// Filename: fat32test.c
// Description: File System Test Program
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

//#include <cmp_debug.h>

#include "hwequ.h"
#include "mediadetection.h"
#include "project.h"
#include "messages.h"
#include "fattest.h"
#include "lcdsupport.h"
#include "FSAPI.h"
#include "fsproj.h"
#include "FileSpec.h"
#include "Fserror.h"
#include "fstypes.h"
#include "FileSystem.h"
#include "misc.h"
#include "sysirq.h"
#include "ipr.h"

#if defined(DEVICE_3500)
#include "regsgpflash.h"
#endif
#include "sysserialnumber.h"
//#define PROFILER
// #define MEMORYTEST
// #define FINDNEXT_TEST
//#define MAXDIRECTORYTEST
// #define MAXFILEOPENTEST
// #define DELETREETEST
//#define VOLUMETEST
extern INT maxhandles;
extern INT maxdevices;
extern INT maxcaches;

extern int device[];
extern INT DriveLetter[];

// The cache buffers are allocated in the X memory
extern INT _X bufx[XBUF_SIZE];

// All other global, custom configurable data is in Y memory.
// Two handles are reserved by the file system, so the number of handles must be more than 2
extern INT _Y bufy[YBUF_SIZE];
#ifdef FINDNEXT_TEST
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
#endif
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
BOOL g_bEraseComplete = TRUE;
BOOL g_bResourceGetDirty = FALSE;
_reentrant WORD ToggleSystemDrive(void)
{
	return SUCCESS;
}
_reentrant WORD MarkSystemDriveDirty(void)
{
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

#define DISPLAY_LENGTH  96
#define BAR_CHAR        6
#define NOERROR      0
#define TESTFAIL     1
#define TESTSUCCESS  0
#define ERROR        -1
#define NUM_WRITE_charS  1000
#define NUM_WRITE_WORDS  334
#define MAX_COPY_WORDS  500
#define NUM_COMPARE_READ_WORDS 250
#define DRIVE_TAG_RESOURCE_BIN      0x02

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

void SetDclkCount(DWORD dwCount);
DWORD GetDclkCount(void);
//RETCODE ProgramPll(INT iPdiv);

////////////////////////////////////////////////////////////////////////////////
//  Variables
////////////////////////////////////////////////////////////////////////////////
extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern WORD  InitFileSystem(INT device);
void  FS_SwizzleText(_packed char *pText);
void GetUnicodeString(_packed char *filepath,_packed char *buf,INT Strlen);
//int   TestReadMode();

int   TestAppendMode();
INT GetBuffer[300];
INT DirectoryCount=0;
INT BitBuffer[512];
INT ReadBuffer[512];
INT FileNames[80];
INT count=0;
INT count1=0;

int  pwFileBuffer[MAX_COPY_WORDS+1];
int pwCompareBuffer[NUM_WRITE_WORDS+1];
static INT   Buffer[MAX_COPY_WORDS+1];
#ifdef MEMORYTEST
INT _P PBuffer[100];
INT _X XBuffer[300];
#endif
int TestResult=TESTSUCCESS;
int i;
LONG CurrentOffset;
int RetValue=NOERROR;
INT fin,fout,fout1,fout2,fout3,fin1,foutw;
extern  FILESPEC FileSpec;

INT CurrentModule = 0;

_packed char big_read_file[]= "a:/read.big";
_packed char big_write_file[]= "a:/write.big";

_packed char testget[]="a:/file2.txt";
_packed char bigfile[]="a:/test.wav";
_packed char writebig[]="a:/copy.wav";
_packed char testread[]="a:/file1.txt";
_packed char testfile[]="a:/TEST.h";
_packed char testfile1[]="a:/C/C1/test.txt";
_packed char Nofile[]="a:/module/sbrdecoder/applysbr/c/src/test.c";
_packed char handletest[]="a:/Handletable.h";
_packed char putfile[]="a:/putfile.h";
_packed char testfile2[]="a:/MYDIR4/test.h";
_packed char writefile[]="a:/testgetfile.h";
_packed char writefile1[]="a:/writefile.h";
_packed char writefile2[]="a:/testp.h";
_packed char testfile3[]="a:/MYDIR1/EOF.asm";
_packed char testfile4[]="a:/C/C2/C3/test.h";
_packed char testfile5[]="a:/C/C2";
_packed char testfile6[]="a:/MYDIR1/EOF.asm";
_packed char getfile[]="a:/getfile1.h";
_packed char SeekFile[]="a:/seek.wav";
_packed char AttrFile[]="a:/Handletable.h";
_packed char chfile[]="a:/MyDir/SubDir/SubDir1/SubDir2/SubDir3/SubDir4/test.asm";
_packed char chfile1[]="a:/inp.hex";
_packed char chfile2[]="./temp.hex";
_packed char chfile3[]="temp.hex";
_packed char bFileDest[] ="a:/test.asm";
_packed char bFileSource[] ="a:/test.asm";
_packed char readfile[]="a:/c/test.c";
_packed char removefile[]="a:/removefile.txt";


_packed char FindBuffer[] = "*.mp3";
_packed char FindFile[]="..";
_packed char WorkingDir[]="a:/Songs1/Songs2/Songs3/Songs4";

_packed char ReadFILE[] = "Test.wav";
//_packed char rootdir[]="a:/testdir1";
_packed char rootdir[]="a:/testdir123/";
_packed char rootdir1[]="a:/C/C2/C3/testdir1";
_packed char level1dir[]="a:/MYDIR3/testdir1/";
_packed char level2dir[]="a:/MYDIR2/SubDir/testdir1";
_packed char level3dir[]="a:/MyDir/testdir1";
_packed char level5dir[]="a:/MyDir/SubDir/SubDir1/SubDir6/testdir1";
_packed char level6dir[]="a:/MyDir/SubDir/SubDir1/SubDir2/SubDir3/SubDir4/";
_packed char chdirb[]="b:/MyDir/SubDir/SubDir1/SubDir2/SubDir3/SubDir4";
_packed char chfileb[]="test.hex";
_packed char chdirl[]="a:/XXXXXX~1";
_packed char chfile4[]="test.hex";
_packed char chdir[]="a:/C/C2/C3";
_packed char chdir1[]="a:/MYDIR2/SubDir";

_packed char bTextFail[] = "Fail";

_packed char Attrdir[]="a:/MyDir";

char longfile[100] = "a:/longfiletest.asm";
char longfile1[100]= "a:/FSLSubDir/FSLSubDir1/FSLSubDir2/FSLSubDir5/FileSystem_input.inc";
char longfile2[50]= "a:/longfilenametest.asm";

_packed char DeleteDirectory[]="a:/c";
_packed char chroot[]="/";
_packed char chlvel1[]="..";
_packed char chlevel2[]="../../";
_packed char chlevel3[]="../../../..";
_packed char chlevel4[]="../../../../..";
_packed char chlevel5[]="../../../../../..";

int g_iHighPwrPeriphInitMode = USB_HIGH_CURRENT_GRANTED; 
//BOOL g_bWriteBmngr512Bytes=TRUE; // unused stub

SERIAL_NUMBER g_InternalMediaSerialNumber; // Kludged here to make it compile without serial num sections for size.


int   TestFseek(void);

extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT Strlength(_packed char *Buffer);
extern INT FSGetByte(void *buffer, INT Offset,INT MemoryType);
extern void PutByte(void *buffer, INT Offset, INT byte,INT MemoryType);
extern void PutWord(void *buffer, INT Offset, INT word,INT MemoryType);
extern RETCODE _reentrant MediaDiscoverAllocation(WORD wLogMediaNumber);
extern RETCODE _reentrant MediaInit(WORD wLogMediaNumber);
extern RETCODE _reentrant DriveInit(WORD wLogDriveNumber);
extern INT  FSInit(INT _X *bufx, INT _Y *bufy, INT maxdevices, INT maxhandles, INT maxcaches);
extern _reentrant INT Computefreeluster(INT DeviceNumber);

//------------------------------------------------------------------------------
//#include <exec.h>
#define k_01mb (unsigned long)(1048576)
#define k_05mb (unsigned long)(k_01mb * 5)
#define k_10mb (unsigned long)(k_01mb * 10)
#define k_20mb (unsigned long)(k_01mb * 20)
#define k_120mb (unsigned long)(k_01mb * 120)

//long _start_time;
//long _end_time;
int newWriteSpeed(unsigned long sz);
int newProfileReadWrite(void);
int newTestWriteFile(INT loop_counter);
// reference into memsetup.c since it has no interface to include...
extern void region_table_discard_current_resource(void);
//------------------------------------------------------------------------------

int ProfileReadWrite(void);
int TestHandle(void);
int TestReadMode(void);
int TestVolumelabel(INT Device);
int TestWriteMode(void);
int TestAppendMode(void);
int TestReadPlusMode(void);
int TestWritePlusMode(void);
int TestAppendPlusMode(void);
int TestFeof(void);
int TestFileread(void);
int TestWriteFile(void);
int TestFremove(void);
int TestFtell(void);
int TestMkdir(void);
int TestMkdirMax(INT DeviceNum);
int TestRmdir(void);
int TestFgetc(void);
int TestFgets(void);
int TestFputc(void);
int TestFputs(void);
int TestFclose(void);
int Testfilegetattrib(void);
int Testfilesetdate(void);
int TestChdir(void);
int TestFopenwRead(void);
int TestFopenwWrite(void);
int TestFopenwAppend(void);
int TestFopenwReadPlus(void);
int TestFopenwWritePlus(void);
int TestFopenwAppendPlus(void);
int TestFwritePMemory(void);
int TestFwriteXMemory(void);
int TestDeletTree(void);
int TestFwrite(void);
int TestFopen(void);
#ifdef FINDNEXT_TEST
int _reentrant TestFindNext(void);
#endif
RETCODE _reentrant CreateNullSerialNumber(P_SERIAL_NUMBER pSerialNumber);
WORD _reentrant ConvertHexToASCII(WORD wInHex);

////////////////////////////////////////////////////////////////////////////////
//  Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           Main
//
//   Type:           Function
//
//   Description:    Entry point for Fattest
//
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef PROFILER
extern _asmfunc void ProfilerStop(int profno);
extern _asmfunc void ProfilerStart(int profno);
extern _asmfunc void ProfilerInit(void);
#endif

extern _P WORD * _P _lc_u_p_OVLY_start;
extern _Y WORD * _Y _lc_u_y_evictBootCopybegin;
extern _Y WORD * _Y _lc_u_y_evictBootFirstCopyend;
extern _Y WORD * _Y _lc_u_y_evictBootSecondCopyend;
extern _P WORD * _P _lc_u_p_nandMediaSection;

_reentrant void CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize);

//
// Make sure that the CurrentTask which is used by SysCallFunction is set so
// that the SysCallFunction works correctly.
//
INT g_pCurrentTask=0;

// These variables and functions are defined in Janus library.  Either
// these should be conditionally compiled better or all of Janus should
// be included in tests.  For now, just allow tests to build
INT g_JanusDrive = -1;
WORD g_protectedMode = 0;

void Encrypt_data(WORD _X  *inputbuf,DWORD SectorNumber,INT BytesPerSector)
{
  SystemHalt();
}

void   Decrypt_data(WORD  _X *inputBuffer,DWORD SectorNumber,INT BytesPerSector)
{
  SystemHalt();
}

void main(void)
{
    INT Strlen;
	INT TempDrive,Devicecount;
    char bDeviceCounter = MAX_LOGICAL_DEVICES - 1;
    char bNumberDevTested = 0;
    _packed char bEndOfTest[20] = "End of Test";
    _packed char bDevTested[20] = "Dev(s) Tested :";
    _packed char bCFC[20] = "ComputeFreeCluster";


//  if(ProgramPll(0x006000) != NOERROR)
//  {
//      #pragma asm
 //         debug
//      #pragma endasm
//  }

//      #pragma asm
//          debug
//     #pragma endasm
	#pragma asm
		include "sysspeed.inc"
		include "regscore.inc"
		extern  PowerInit
		extern	AnalogInit
        extern  SysSpeedForce
        extern  SysAllLinear
	
        ; dummy for SysHwInit.asm
        global  MixerMasterVol

MixerMasterVol dc 0
        jsr     PowerInit
        jsr     AnalogInit
        jsr     SysAllLinear
   
    #pragma endasm        

    {

        unsigned int CopySize = (unsigned int)(&_lc_u_y_evictBootFirstCopyend-&_lc_u_y_evictBootCopybegin);
        
        CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_evictBootCopybegin, 
                                    (_P WORD * _P)&_lc_u_p_OVLY_start, CopySize);

        
    }

   g_pCurrentTask = NULL;
#if 0
#if defined(DEVICE_3500)

// Set 8mA drive for the GPFlash pins
    HW_GP08MA.I = 7;
    HW_GP18MA.I = 7;
    HW_GP28MA.I = 7;
    HW_GP38MA.I = 7;

#endif
#endif
    // Timing set to conservative values in MediaInit.

    // Init the media
    if(MediaInit(0) != SUCCESS)
    {
        SystemHalt();
    }
   //turn interrupts on
    IPRSetIVLPriority(IVL_LINE_1,IVL_PRIORITY_1);

    // Unmask all interrupts
    SysUnMaskAllInterrupts(INTERRUPTS_UNMASK_ALL);

    #pragma asm   
        move    #>SPEED_MAX,x1
;        jsr     SysSpeedForce   ; TOVERIFY   I left this out as 3.1 does.  3.0 made this call. 
        jsr     SysAllLinear   
    #pragma endasm       


    //
    // Initialize the System resource file state.
    //
    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);

    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }

#ifdef MMC
	for (i=3;i<g_wNumDrives;i++)
#else
    for (i=2;i<g_wNumDrives;i++)
#endif
    {
        if (DriveInit(i) != SUCCESS)
        {
            SystemHalt();
        }
    }

//
#ifdef MMC
    MediaInit(1);
#endif

    if (DriveInit(0) != SUCCESS)
    {
        SystemHalt();
    }

#ifdef MMC
    DriveInit(1);
#endif

    DisplayModuleInit(ModuleTableDisplay);

	SetContrast(5);

    Delay(100);
	

    if (FSInit(bufx, bufy, maxdevices, maxhandles, maxcaches) != SUCCESS)
    {
        SystemHalt();
    }

    FSDriveInit(0);
#ifdef MMC
    FSDriveInit(1);
#endif

	/* Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(bCFC);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) bCFC);

    for (i=0;i<maxdevices;i++)
    {
        Computefreeluster(i);
    }          

	LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

    /*Swizzle the given text string*/
    FS_SwizzleText((_packed char *)Nofile);
    FS_SwizzleText((_packed char *)testfile);
    FS_SwizzleText((_packed char *)testfile1);
    FS_SwizzleText((_packed char *)testfile2);
    FS_SwizzleText((_packed char *)testfile3);
    FS_SwizzleText((_packed char *)testfile4);
    FS_SwizzleText((_packed char *)testfile5);
    FS_SwizzleText((_packed char *)testfile6);
    FS_SwizzleText((_packed char *)readfile);
    FS_SwizzleText((_packed char *)handletest);
    FS_SwizzleText((_packed char *)writefile);
    FS_SwizzleText((_packed char *)writefile1);
    FS_SwizzleText((_packed char *)getfile);
    FS_SwizzleText((_packed char *)rootdir);
    FS_SwizzleText((_packed char *)rootdir1);
    FS_SwizzleText((_packed char *)level1dir);
    FS_SwizzleText((_packed char *)level2dir);
    FS_SwizzleText((_packed char *)level3dir);
    FS_SwizzleText((_packed char *)level5dir);
    FS_SwizzleText((_packed char *)level6dir);
    FS_SwizzleText((_packed char *)chdir);
    FS_SwizzleText((_packed char *)chdir1);   
    FS_SwizzleText((_packed char *)SeekFile);
    FS_SwizzleText((_packed char *)AttrFile);
    FS_SwizzleText((_packed char *)Attrdir);
    FS_SwizzleText((_packed char *)chfile);
    FS_SwizzleText((_packed char *)chfile1);
    FS_SwizzleText((_packed char *)chfile2);
    FS_SwizzleText((_packed char *)chfile3);
    FS_SwizzleText((_packed char *)chroot);
    FS_SwizzleText((_packed char *)chlvel1);
    FS_SwizzleText((_packed char *)chlevel2);
    FS_SwizzleText((_packed char *)chlevel3);
    FS_SwizzleText((_packed char *)chlevel4);
    FS_SwizzleText((_packed char *)chlevel5);
    FS_SwizzleText((_packed char *)FindFile);
    FS_SwizzleText((_packed char *)FindBuffer);
    FS_SwizzleText((_packed char *)WorkingDir);
    FS_SwizzleText((_packed char *)bFileDest);
    FS_SwizzleText((_packed char *)bFileSource);
    FS_SwizzleText((_packed char *)testread);
    FS_SwizzleText((_packed char *)testget);
    FS_SwizzleText((_packed char *)bTextFail);
    FS_SwizzleText((_packed char *)writefile2);
    FS_SwizzleText((_packed char *)putfile);
    FS_SwizzleText((_packed char *)bigfile);
    FS_SwizzleText((_packed char *)writebig);
    FS_SwizzleText((_packed char *)chdirb);
    FS_SwizzleText((_packed char *)chfileb);
    FS_SwizzleText((_packed char *)chdirl);
    FS_SwizzleText((_packed char *)chfile4);
    FS_SwizzleText(bEndOfTest);
    FS_SwizzleText(bDevTested);
    FS_SwizzleText((_packed char *)ReadFILE);
    FS_SwizzleText((_packed char *)DeleteDirectory);

    FS_SwizzleText((_packed char *)big_read_file);
    FS_SwizzleText((_packed char *)big_write_file);

    /*fill the buffer for test*/

    for(i=0;i<512;i++)
    {
      BitBuffer[i]=100+i;
    }
    for (Devicecount=0;Devicecount<maxdevices;Devicecount++)
	{
		 if(FSMediaPresent(Devicecount))
		{
        #ifdef PROFILERx
        if(ProfileReadWrite() == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          SystemHalt();
        }
        #endif

        #if 0
        // stop before test to simplify debugging
        _trap();

        // this tests append to a non-existing file.
        // the same test later in the suite appends to an existing file.
        //if(TestWritePlusMode()==ERROR)
        //{
        //    PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
        //    SystemHalt();
        //}

        #endif

        #if 0
        //----------------------------------------------------------------------
        // 1mb
        //----------------------------------------------------------------------
        // write to blank media

        // HACK - force SysCallFunction to discard the media initialization overlay
        region_table_discard_current_resource();

        if (newWriteSpeed(k_01mb) == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        //----------------------------------------------------------------------
        // overwrite
        if (newWriteSpeed(k_01mb) == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        //----------------------------------------------------------------------
        //----------------------------------------------------------------------
        {
          int loop;
          for (loop = 0; loop < 10; loop++)
          {
            if (newWriteSpeed(k_10mb) == ERROR)
            {
              PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
              // failed
              _trap();
            }
          }
          // passed
          _trap();
        }
        //----------------------------------------------------------------------
        // 5mb
        //----------------------------------------------------------------------
        // write to blank media
        if (newWriteSpeed(k_05mb) == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        //----------------------------------------------------------------------
        // overwrite
        if (newWriteSpeed(k_05mb) == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        //----------------------------------------------------------------------
        // 10mb
        //----------------------------------------------------------------------
        // write to blank media
        if (newWriteSpeed(k_10mb) == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        //----------------------------------------------------------------------
        // overwrite
        if (newWriteSpeed(k_10mb) == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        #endif

        #if 0
        if (newProfileReadWrite() == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        //_trap();
        #endif

        #if 0
        if (TestWriteFile() == ERROR)
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
        PrintStringAddr(5, 2*8, (_packed unsigned char *)bEndOfTest);
        PrintStringAddr(1, 4*8, (_packed unsigned char *)bDevTested);
        PrintNumber(84, 4*8, bNumberDevTested, 2, '0');
        _trap();
        #endif

        #if 0
        // NOTE: the loop count poassed to newTestWriteFile() *must* be an even number.
        // also, this test can be run on blank media, to simplify monitoring the fat
        // and the root directory.
        if (ERROR == newTestWriteFile(8))
        {
          PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
          // failed
          _trap();
        }
        // passed
        _trap();
        #endif

		#ifdef PROFILER
		   	if(ProfileReadWrite()==ERROR)
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		#endif
		#ifdef VOLUMETEST
		    if(TestVolumelabel(Devicecount)==ERROR)
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		#endif

  		    /*Test the Handle */
		     if(TestHandle()==ERROR)
		    {
		      goto printerror;
		    }
		    /* Test Fopen in read("r") mode*/
		    if(TestReadMode()==ERROR)
		    {
		      goto printerror;
		    }
		    /* Test Fopen in write("w") mode*/
		    if(TestWriteMode()==ERROR)
		    {
		      goto printerror;
		    }
		    /* Test Fopen in append ("a") mode*/
		    if(TestAppendMode()==ERROR)
		    {
		      goto printerror;
		    }
		    /* Test Fopen in readplus ("r+") mode*/
		    if(TestReadPlusMode()==ERROR)
		    {
		      goto printerror;
		    }
		    /* Test Fopen in writeplus ("w+") mode*/
		    if(TestWritePlusMode()==ERROR)   
		    {
		      goto printerror;
		    }
		    /* Test Fopen in appendplus("a+") mode*/
		    if(TestAppendPlusMode()==ERROR)
		    {
		      goto printerror;
		    }
		    /*Test the end of file*/    
		    if(TestFeof()==ERROR)
		    {
		      goto printerror;
		    }
		    /*Test File Read*/    
		    if(TestFileread()==ERROR) 
		    {
		      goto printerror;
		    }
		    /*Test File Write*/    
		    if(TestWriteFile()==ERROR)   
		    {
		      goto printerror;
		    }
		     /*Test the File remove*/
		    if(TestFremove()==ERROR)
		    {
		      goto printerror;
		    }
		    /*Test Ftell*/
		    if(TestFtell()==ERROR)
		    {
		      goto printerror;
		    }
		    /*Test the Make Directories*/
		    if(TestMkdir()==ERROR)   
		    {
		      goto printerror;
		    }
		    /* Test Remove Directory*/ 
		    if(TestRmdir()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fgetc (Get one character from file)*/ 
		    if(TestFgetc()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fgets (Get one string from file)*/ 
		    if(TestFgets()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fputc (Put one character to the file)*/ 
		    if(TestFputc()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fputs (Put one string to the file)*/ 
		    if(TestFputs()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fileclose*/
		    if(TestFclose()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test File seek*/
		    if(TestFseek()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Testfilegetattrib*/
		    if( Testfilegetattrib()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Testfilesetdate*/
		    if( Testfilesetdate()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Change Directories*/
		    if(TestChdir()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fopenw for Read(r) mode*/				   
		    if(TestFopenwRead()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fopenw for Write(w) mode*/
		    if(TestFopenwWrite()==ERROR)   
		    {
		      goto printerror;
		    }
		    /*Test Fopenw for Append(a) mode*/
		    if(TestFopenwAppend()==ERROR)   
		    {
		        goto printerror;
		    }
		    /*Test Fopenw for Rplus mode*/
		    if(TestFopenwReadPlus()==ERROR)   
		    {
		        goto printerror;
		    }
		    /*Test Fopenw for Wplus mode*/
		    if(TestFopenwWritePlus()==ERROR)   
		    {
		        goto printerror;
		    }
		    /*Test Fopenw for Aplus mode*/
		    if(TestFopenwAppendPlus()==ERROR)   
		    {
		        goto printerror;
		    }
		#ifdef MEMORYTEST
		    /*Test Fred and Fwrite from P MEMORY*/
		    if(TestFwriteMemory(P_MEMORY)==ERROR)   
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		    /*Test Fred and Fwrite from X MEMORY*/
		    if(TestFwriteMemory(X_MEMORY)==ERROR)   
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		#endif
		#ifdef DELETREETEST
		    if(TestDeletTree()==ERROR)
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		#endif
		#ifdef FINDNEXT_TEST
		    if((RetValue=Chdir(WorkingDir)) <0)
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }

		    if(TestFindNext()==ERROR)    
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		#endif
		/* if want to create maximum number of directory define MAXDIRECTORY 
		   maximum number of directories entries: FAT12 : 256, Fat16:512 , FAT32 : NOLIMIT*/

		#ifdef MAXDIRECTORYTEST   
		    if(TestMkdirMax(Devicecount)==ERROR)   
		    {
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
		    }
		#endif
		#ifdef MAXFILEOPENTEST   
			if(TestFopen()==ERROR)
			{
		        PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
		        SystemHalt();
			}
		#endif
		bNumberDevTested++;
		}
	// Swap the drive letters to run test on external media, if present.
	TempDrive = DriveLetter[Devicecount+1];
	DriveLetter[Devicecount+1] = DriveLetter[Devicecount];
	DriveLetter[Devicecount] = TempDrive;      
   }
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

	PrintStringAddr(5, 2*8, (_packed unsigned char *)bEndOfTest);

	PrintStringAddr(1, 4*8, (_packed unsigned char *)bDevTested);

	PrintNumber(84, 4*8, bNumberDevTested, 2, '0');

   ///////////////////////////////////////////////////////////////////////
   //     SUCCESS
   ///////////////////////////////////////////////////////////////////////
   SystemHalt(); // better to use Assert(0); in case of RETAIL build in which SystemHalt resets CPU.
   // Assert(0);
   ///////////////////////////////////////////////////////////////////////
   //     SUCCESS
   ///////////////////////////////////////////////////////////////////////

 printerror:

   PrintStringAddr(40, 5*8, (_packed unsigned char *)bTextFail);
   ///////////////////////////////////////////////////////////////////////
   //     FAIL
   ///////////////////////////////////////////////////////////////////////
   SystemHalt(); // resets cpu in RETAIL builds (undesired here)
   // Assert(0); // better
   ///////////////////////////////////////////////////////////////////////
   //     FAIL
   ///////////////////////////////////////////////////////////////////////
}
#if 0
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !defined(min)
  #define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#define my_memset(p_buf, ch, n)\
{                              \
  int i = (n);                 \
  while (i)                    \
  {                            \
    p_buf[i - 1] = (ch);       \
    i--;                       \
  }                            \
}
INT _buffer[CACHEBUFSIZE];
int newWriteSpeed(unsigned long sz)
{
  _packed char btitle[20] = "Write Speed";
  _packed char fname01[] = "a:/wr01.mb";
  _packed char fname05[] = "a:/wr05.mb";
  _packed char fname10[] = "a:/wr10.mb";
  _packed char fname20[] = "a:/wr20.mb";

  int retcode = NOERROR;
  unsigned long n_byte_residue;
  unsigned long n_byte_payload;

  LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
  FS_SwizzleText(btitle);
  PrintStringAddr(10, 3*8, (_packed unsigned char *)btitle);

  my_memset(_buffer, 0xAA, CACHEBUFSIZE);
  FS_SwizzleText(fname01);
  FS_SwizzleText(fname05);
  FS_SwizzleText(fname10);
  FS_SwizzleText(fname20);
  switch (sz)
  {
    case k_01mb:
      fout = Fopen(fname01, (_packed char *)"w");
      break;
    case k_05mb:
      fout = Fopen(fname05, (_packed char *)"w");
      break;
    case k_10mb:
      fout = Fopen(fname10, (_packed char *)"w");
      break;
    case k_20mb:
      fout = Fopen(fname20, (_packed char *)"w");
      break;
    case k_120mb:
      fout = Fopen(fname20, (_packed char *)"w");
      break;

  }
  if  (fout < 0)
  {
    retcode = ERROR;
    goto EXIT;
  }
  //ProfilerInit();
  n_byte_residue = sz;
  while (n_byte_residue)
  {
    n_byte_payload = min(n_byte_residue, 512);
    retcode = Fwrite(fout, _buffer, n_byte_payload, Y_MEMORY, -1);
    if (retcode <= 0)
    {
      retcode = ERROR;
      goto EXIT;
    }
    n_byte_residue -= n_byte_payload;
  }
  EXIT:
  if (fout)
  {
    Fclose(fout);
  }
  if (ERROR != retcode)
  {
    FlushCache();
  }
  //_trap();
  //Fremove(fname05);
  return retcode;
}
#endif
#if 0
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !defined(min)
  #define min(a, b) ((a) < (b) ? (a) : (b))
#endif
int newProfileReadWrite()
{
  _packed char btitle[20] = "Test Profiler";

  LONG retcode = NOERROR;
  LONG file_sz;
  LONG n_byte_residue;
  LONG n_byte_payload;

  LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
  FS_SwizzleText(btitle);
  PrintStringAddr(10, 3*8, (_packed unsigned char *)btitle);

  fin = Fopen(big_read_file, (_packed char *)"r");
  fout = Fopen(big_write_file, (_packed char *)"w");
  if ((fin < 0) || (fout < 0))
  {
    retcode = ERROR;
    goto EXIT;
  }
  Fseek(fin, 0, SEEK_END);
  file_sz = Ftell(fin);
  retcode = Fseek(fin, 0, SEEK_SET);
  if (retcode < 0)
  {
    retcode = ERROR;
    goto EXIT;
  }
  n_byte_residue = file_sz;
  while (n_byte_residue)
  {
    n_byte_payload = min(n_byte_residue, 512);
    retcode = Fread(fin, ReadBuffer, n_byte_payload, Y_MEMORY, -1);
    if (retcode <= 0)
    {
      retcode = ERROR;
      goto EXIT;
    }
    retcode = Fwrite(fout, ReadBuffer, n_byte_payload, Y_MEMORY, -1);
    if (retcode <= 0)
    {
      retcode = ERROR;
      goto EXIT;
    }
    n_byte_residue -= n_byte_payload;
  }
  EXIT:
  if (fout)
  {
    Fclose(fout);
  }
  if (fin)
  {
    Fclose(fin);
  }
  if (ERROR != retcode)
  {
    FlushCache();
  }
  return retcode;
}
#endif
#if 0
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int  newTestWriteFile(INT loop_counter)
{
  INT retcode;
  INT iBytesToWrite;
  INT i;
  INT j;
  INT cur_pos = 0;
  LONG lError;
  char bHandleSource;
  char bHandleDest;
  char bEndCopyFlag = 0;
  WORD wProgBarPosX = 0;
  WORD wUpdateDisplay;
  WORD wUpdateTicks;
  _packed char btitle[30] = "newTestWriteFile";

  // clear screen
  LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
  // display title
  FS_SwizzleText(btitle);
  PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

  // Fill the buffers with patterns
  for (i=0; i<NUM_WRITE_WORDS; i++)
  {
    pwFileBuffer[i] = -5592406;   // 0xAAAAAA
    pwCompareBuffer[i] = 0x555555;
  }
  if ((fout = Fopen(bFileDest, (_packed char *)"w")) <0)
    return ERROR;
  FlushCache();
  // Write the initial pattern to the file
  for (i = 0; i < loop_counter; i++)
  {
    RetValue =Fwrite(fout, pwFileBuffer, NUM_WRITE_charS, DEFAULT_MEMORY, -1);

    if ((RetValue <0) || (RetValue != NUM_WRITE_charS))
    {
      // Error writting - Close files
      FlushCache();
      return ERROR;
    }

  }
  if (Fseek(fout, 0, SEEK_SET)<0)
    return ERROR;
  // Overwrite the original pattern for every other chunk of NUM_WRITE_charS
  for (i = 0; i < loop_counter / 2; i++)
  {
    RetValue =Fwrite(fout, pwCompareBuffer, NUM_WRITE_charS, DEFAULT_MEMORY, -1);

    if ((RetValue <0) || (RetValue != NUM_WRITE_charS))
    {
      // Error writting - Close files
      Fclose(fout);
      return ERROR;
    }
    if (Fseek(fout, NUM_WRITE_charS, SEEK_CUR)<0)
    {
      // Error seeking - Close files
      Fclose(fout);
      return ERROR;
    }
  }
  // We should be at the end of the file
  if (Feof(fout)> 0)
  {
    // Error seeking - Close files
    Fclose(fout);
    return ERROR;
  }
  // Close the file then open it again in READ mode
  Fclose(fout);
  FlushCache();

  if ((fin = Fopen(bFileSource, (_packed char *)"r")) <0)
  {
    Fclose(fin);
    return ERROR;
  }
  // Verify that every other chunk was changed correctly
  // First verify the overwritten chunks
  for (i = 0; i < loop_counter / 2; i++)
  {
    // We're comparing words not bytes so clear the read buffer & set the
    // last word to the expected pattern.
    for (j=0; j<(NUM_WRITE_WORDS-1); j++)
    {
      pwFileBuffer[j] = 0;
    }
    pwFileBuffer[NUM_WRITE_WORDS-1] = 0x555555;

    // Overwrite original data
    RetValue = Fread(fin, pwFileBuffer, NUM_WRITE_charS, DEFAULT_MEMORY, -1);

    if ((RetValue <0) ||(RetValue != NUM_WRITE_charS))
    {
      // Error reading - Close files
      Fclose(fin);
      count1++;
      return ERROR;
    }
    cur_pos=Ftell(fin);
    // Skip past a chunk
    if (Fseek(fin, NUM_WRITE_charS, SEEK_CUR) == ERROR)
    {
      // Error seeking - Close files
      Fclose(fin);
      count1++;
      return ERROR;
    }
    cur_pos=Ftell(fin);
    for (j=0; j<NUM_WRITE_WORDS; j++)
    {
      if (pwFileBuffer[j] != 0x555555)
      {
        // Error reading - Close files
        Fclose(fin);
        count1++;
        return ERROR;
      }
    }
    count1++;
  }
  // Now verify the original chunks
  // We should be at the end of the file
  if (Feof(fin)>0)
  {
    // Error seeking - Close files
    Fclose(fin);
    return ERROR;
  }
  // Rewind the file
  if (Fseek(fin, 0, SEEK_SET)<0)
  {
    // Error seeking - Close files
    Fclose(fin);
    return ERROR;
  }
  // Check 'em
  for (i = 0; i < loop_counter / 2; i++)
  {
    // We're comparing words not bytes so clear the read buffer & set the
    //  last word to the expected pattern.
    for (j=0; j<(NUM_WRITE_WORDS-1); j++)
    {
      pwFileBuffer[j] = 0;
    }
    pwFileBuffer[NUM_WRITE_WORDS-1] = -5592406;  // 0xAAAAAA

    // Skip past the overwritten pattern
    if (Fseek(fin, NUM_WRITE_charS, SEEK_CUR) == ERROR)
    {
      // Error seeking - Close files
      Fclose(fin);
      return ERROR;
    }
    // Read the original data
    RetValue = Fread(fin, pwFileBuffer, NUM_WRITE_charS, DEFAULT_MEMORY, -1);

    if ((RetValue<0) || (RetValue != NUM_WRITE_charS))
    {
      // Error reading - Close files
      Fclose(fin);
      return ERROR;
    }
    for (j=0; j<(NUM_WRITE_WORDS-1); j++)
    {
      if (pwFileBuffer[j] != 0xAAAAAA)
      {
        // Error reading - Close files
        Fclose(fin);
        return ERROR;
      }
    }
  }
  Fclose(fin);
  Fclose(fout);
  FlushCache();
  return NOERROR;
}
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// dummy routine for syshwinit functions
void _reentrant PowerUpHeadPhones(void){}

#ifdef DELETREETEST
INT TestDeletTree(void)
{
	INT RetValue =0;
    _packed char btitle[20] = "Test DeleteTree"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *)btitle);

	if((RetValue =DeleteTree(DeleteDirectory)) <0)
	{
		return ERROR;
	}
	FlushCache();
	return NOERROR;
}
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           ProfileReadWrite

//   Type:           Function
//
//   Description:    Test the File Write function

//   Inputs:         none 

//   Outputs:        none
//
//   Notes:          Profiler test the filesystem performance on the differnt media                //<
////////////////////////////////////////////////////////////////////////////////
#ifdef PROFILER
int ProfileReadWrite()
{
    INT fin,BytesToRead;
    LONG FileSize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=1500;
    _packed char btitle[20] = "Test Profiler"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *)btitle);
    
#ifdef PROFILER
	ProfilerInit();
#endif
    //open the file for reading
    if ((fin = Fopen(bigfile,(_packed char *)"r"))<0)
        return  ERROR;
    //open the file for writing
    if ((fout = Fopen(writebig,(_packed char *)"w"))<0)
        return  ERROR;
    /*Get the file size    */
    Fseek(fin,0,SEEK_END);

    FileSize=Ftell(fin);
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    
    
    /*Test the Fwrite by eriting whole file*/
    NumBytesToRead =FileSize;
    BytesToRead =0;
    while(NumByteToWrite < FileSize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
#ifdef PROFILER
			ProfilerStart(0);
#endif
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,Y_MEMORY,-1))<=0)
            {
                Fclose(fin);
                return ERROR;
            }
#ifdef PROFILER
	ProfilerStop(0);
	ProfilerStart(1);
#endif
            if((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,Y_MEMORY,-1))<=0)
            {
                Fclose(fout);
                return ERROR;
            }           
	
#ifdef PROFILER
			ProfilerStop(1);
#endif
        NumByteToWrite+=RetValue;
        }
    NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==FileSize))
        {
            NumByteToWrite = FileSize+1;
        }     
    }
    Fclose(fout);
    Fclose(fin);
    FlushCache();
    
    return NOERROR;
} 
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Program Pll
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        iPdiv          Value for HW_CCR PDIV field
//
//   Outputs:       RETCODDE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
///*RETCODE ProgramPll(INT iPdiv)
//{
//    /* Get it started*/
//    HW_CCR.I = iPdiv|HW_CCR_CKRST_SETMASK|HW_CCR_PLLEN_SETMASK|HW_CCR_ACKEN_SETMASK;
//
//    /*Start the lock timer counter*/
//    HW_CCR.B.LTC = 1;
//
//    /*delay for 3*10k clocks.*/
//    /*Only need to wait for 12.288k but what the heck.*/
//    /* TODO:  The following asm code should be replaced with a C function*/
//#pragma asm
//    clr     a
//    move    #>$001388,a0
//_wait_loop
//    dec     a
//    jne     _wait_loop
//#pragma endasm
//
//    /* We're locked so change the digital clock source to the PLL*/
//    HW_CCR.B.CKSRC = 1;
//
//    return NOERROR;
//}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestwriteMode
//
//   Type:           Function
//
//   Description:    Test the write mode for function Fopen
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          Open the file in write mode and try to read and write using different handles
//                   if the file is not present, it will create the file.
//<
////////////////////////////////////////////////////////////////////////////////
int TestWriteMode(void)
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed char btitle[20] = "Test WriteMode"; 
	/* Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /* Open file in  write mode (if the file is not present, it will create the file) */
    if ((fout=Fopen(writefile1,(_packed char *)"w"))<0)
        return ERROR;
    /* Open for Read mode for filling Readbuffer*/
    if ((fin=Fopen(readfile,(_packed char *)"r"))<0)
        return ERROR;
    /*Seek to the End of file to get file size*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;

    /* try to write  the whole file which is open in write mode*/
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead = NumBytesToRead;

            if ((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            if ((RetValue = Fwrite(fout,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }
    }
   /*close open file */
    Fclose(fin);
    Fclose(fout);
    FlushCache();
    return  NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestHandle
//
//   Type:           Function
//
//   Description:    Test the handle
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          The test verifies that file handles up to maximum number can be allocated.
//<
////////////////////////////////////////////////////////////////////////////////

int  TestHandle()
{
    _packed char btitle[20] = "Test Handle";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *)btitle);
    /*open the maximum number of file*/
    for(i=FIRST_VALID_HANDLE;i<maxhandles;i++)
    {
    if ((fout=Fopen(handletest,(_packed char *)"r"))<0)
        return  ERROR;
    }
    /*open one more file  this fopen will return error*/
    if ((fout1=Fopen(handletest,(_packed char *)"r"))>0)
        return  ERROR;
    for(fout=FIRST_VALID_HANDLE;fout<maxhandles;fout++)
    {
        Fclose(fout);
    }
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestReadMode
//
//   Type:           Function
//
//   Description:    Test the read mode for function Fopen
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          Verify Read Mode ("r").if file does not present ,function should return error.
//           we can only do read operation from file in this mode.
//<
////////////////////////////////////////////////////////////////////////////////
int TestReadMode()
{    
    _packed char btitle[20] = "Test ReadMode";
	/*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /* Open file in  read mode (Function fopen will fail, if file  does not exist) */
    if ((fout=Fopen(testfile,(_packed char *)"r"))<0)
        return  ERROR;
   /*read some bytes*/
    if(Fread(fout,ReadBuffer,56,DEFAULT_MEMORY,-1)<0)
    {
        return ERROR;
    }              
    /*Attempt to write to a file opened in the read mode, It should fail */   
    if(Fwrite(fout,BitBuffer,25,DEFAULT_MEMORY,-1)>0)
    {
        return ERROR;
    }
    Fclose(fout);
    /* Attempt to open a non-existent file */
    if ((fout = Fopen(Nofile,(_packed char *)"r"))>0)
        return  ERROR;
    FlushCache();
    return NOERROR;
}   

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestAppendMode
//
//   Type:           Function
//
//   Description:    Test the append mode for function Fopen

//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          Open the file in append mode. If write operation is done,it will append the data
//                   at the end  of the file.if file is not present,it will create the file.
//<
////////////////////////////////////////////////////////////////////////////////
int   TestAppendMode()
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed char btitle[20] = "Test AppendMode";
    /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

      /* Open for append mode (if file is not present,it will create the file) */
    if ((fout1=Fopen(writefile1,(_packed char *)"a"))<0)
        return ERROR;
  /*Open the file in read mode*/
    if ((fin=Fopen(testfile3,(_packed char *)"r"))<0)
        return ERROR;
    /*Seek to the End of file to get file size*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    /* Try to write  the whole file which is open in append mode*/
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;

            if ((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            if ((RetValue =Fwrite(fout1,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }
    }
   /*close open file */
    Fclose(fout1);
    Fclose(fin);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestReadplusMode
//
//   Type:           Function
//
//   Description:    Test the Readplus mode for function Fopen
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          In this mode we can read as well as write in the file.
//                   if file is not present it will return error.

//<
////////////////////////////////////////////////////////////////////////////////

int   TestReadPlusMode()
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed char btitle[30] = "Test ReadPlusMode";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*Attempt to open file which is not exist,it should return error*/
    if ((fout = Fopen(Nofile,(_packed char *)"r+"))>0)
        return  ERROR;
    /*Open file in  read plus mode(Fopen fails if file  does not exist)*/
    if ((fout = Fopen(testfile,(_packed char *)"r+"))<0)
        return  ERROR;

    if ((fin=Fopen(testfile3,(_packed char *)"r"))<0)
        return ERROR;
    /*Seek to the End of file to get file size*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);

    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;

    /* Try to write  the whole file which is open in ("r+") mode*/
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;

            if ((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<=0)
                return ERROR;
            if ((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<=0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }
    }
   /*close open file */
     Fclose(fout);
     Fclose(fin);
     FlushCache();
     return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestWriteplusMode
//
//   Type:           Function
//
//   Description:    Test the writeplus mode for function Fopen
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          In this mode we can read as well write in the file
//                   if the file is not present,Function Fopen will create the file
//<
////////////////////////////////////////////////////////////////////////////////
int   TestWritePlusMode()
{
    _packed char btitle[30] = "Test WritePlusMode";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /*Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

     /*open the file in the writeplus(w+) mode*/
    if ((fout = Fopen(testfile4,(_packed char *)"w+"))<0)
        return  ERROR;
    /*write some byte to file*/
    if ((RetValue =Fwrite(fout,BitBuffer,100,DEFAULT_MEMORY,-1))<0)
    {
        return ERROR;
    }
  /*Seek back the byte written*/
    if ((RetValue=Fseek(fout,-100,SEEK_CUR))<0)
    {
        return ERROR;
    }
    /*Read back data that we had written*/
    if ((RetValue = Fread(fout,ReadBuffer,100,DEFAULT_MEMORY,-1))<0)
    {
        return ERROR;
    }
   /*Check we had  done correctly write and read in this mode*/
    if (BitBuffer[0]!= ReadBuffer[0])
    {
        return ERROR;
    }
   /*close open file */
    Fclose(fout);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestAppendplusMode
//
//   Type:           Function
//
//   Description:    Test the Appendplus ("a+") mode for function Fopen
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          In this mode we can read as well write in the file
//                   if the file is not present,function Fopen will create the file in this mode
//<
////////////////////////////////////////////////////////////////////////////////
int   TestAppendPlusMode()
{
    _packed char btitle[30] = "Test AppendPlusMode";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*open file in appendplus mode (if the file is not present,function Fopen will create the file)*/
    if ((fout = Fopen(writefile,(_packed char *)"a+"))<0)
        return  ERROR;
    /*Write to the file it should write at the end of the file*/
    if ((RetValue =Fwrite(fout,BitBuffer,50,DEFAULT_MEMORY,-1))<0)
    {
        return ERROR;
    }
    /* seek back the bytes write*/
    if ((RetValue =Fseek(fout,-50,SEEK_CUR))<0)
    {
        return ERROR;
    }
    /*Try to read bytes which we had written*/
    if ((RetValue = Fread(fout,ReadBuffer,50,DEFAULT_MEMORY,-1))<0)
    {
        return ERROR;
    }
   /*Check we had  done correctly write and read in this mode*/
    if (BitBuffer[0]!= ReadBuffer[0])
    {
        return ERROR;
    }
   /*close open file */
    Fclose(fout);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFeof
//
//   Type:           Function
//
//   Description:    Test the EOF(End of file) conditation for file

//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          It will test the End of file test.

////////////////////////////////////////////////////////////////////////////////

int    TestFeof()
{
    _packed char btitle[30] = "Test End Of File";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /*Open file in the append mode*/
    if ((fout2 = Fopen(testfile,(_packed char *)"a+"))<0)
        return  ERROR;
   /*it should return the EOF*/
    if (Feof(fout2)>0)
    {
        return  ERROR;
    }
    /*Write some bytes to the file*/
    if ((RetValue=Fwrite(fout2,BitBuffer,25,DEFAULT_MEMORY,-1))<0)
    {
        return ERROR;
    }
  /*Seek back number of bytes written*/
    if ((RetValue=Fseek(fout2,-25,SEEK_CUR))<0)
        return  ERROR;
    /* It should not Return EOF,it should return remain bytes from end of file*/
    if ((RetValue=Feof(fout2))<0)
    {
        return  ERROR;
    }
   /*close open file */
    Fclose(fout2);
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFremove
//
//   Type:           Function
//
//   Description:    Test the file is Remove or not
//
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          If the file is in read only mode it should not Deleted.
//                   //<
////////////////////////////////////////////////////////////////////////////////
int   TestFremove()
{

    _packed char btitle[30] = "Test Delete Files";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*Remove the file*/
    if (Fremove(writefile1)<0)
       return  ERROR;
    FlushCache();
    /*Try to open removed file ,it should give Error*/
    if ((fout = Fopen(removefile,(_packed char *)"r"))>0)
        return  ERROR;
   /*Open the file*/
    if ((fout2 = Fopen(testfile3,(_packed char *)"r"))<0)
        return  ERROR;
    /* Try to Delete the Opened file it Fails*/
    if (Fremove(testfile3)>0)
        return  ERROR;
    /*Try to deleted read-only file,it should return error*/
    if (Fremove(testfile6)>0)
        return  ERROR;
   /*Try to open deleted file,it should open the file because file is not deleted (it is read only)*/
    if ((fout = Fopen(testfile6,(_packed char *)"r"))<0)
        return  ERROR;
    /*Try to Delete the Directory using function Fremove, it should fail*/
    if (Fremove(testfile5)>0)
        return  ERROR;
   /*close open file */
    Fclose(fout2);
  Fclose(fout);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFtell
//
//   Type:           Function
//
//   Description:    Test the Ftell to Get the current offset in the file

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Thie function will get current position of file pointer.
//                   //<
////////////////////////////////////////////////////////////////////////////////

int   TestFtell()
{
    _packed char btitle[30] = "Test Ftell";
  /* Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /*Open the file in read mode*/
    if ((fin=Fopen(testfile2,(_packed char *)"r"))<0)
        return  ERROR;
    /*Get current position in the file*/
    CurrentOffset=Ftell(fin);
    /*Seek some bytes in file and again do Ftell to get Current pointer*/
    RetValue=Fseek(fin,51,SEEK_SET);
    CurrentOffset=Ftell(fin);
    /*Open the file in Append mode and try to do Ftell it Should Return EOF*/
    if ((fin1=Fopen(testfile1,(_packed char *)"a"))<0)
        return  ERROR;
    CurrentOffset=Ftell(fin1);
   /*close open file */
    Fclose(fin);
    Fclose(fin1);
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestMkdir

//   Type:           Function
//
//   Description:    Test the Creation of Directory

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Crate the directory in differnt level.
//                   Maxiumum Root Directory Possible -> FAT 12 Or FAT16 = 256
//                                                       FAT 32  = No Limit
//                   //<
////////////////////////////////////////////////////////////////////////////////
int LoopCount=0;
_packed char DirectoryName[50];

int TestMkdir()
{
    int i,Byte=0,j,m,k,l=0;
    _packed char btitle[30] = "Test Make DIR";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*Try to Create Directory in the roort*/
    if ((RetValue=Mkdir(rootdir))<0)
        return  ERROR;
    FlushCache();
  /*Try to create directory with same name funcrion Mkdir will return error*/
    if ((RetValue=Mkdir(rootdir))>0)
        return  ERROR;
    FlushCache();
    /*Try to Create Directory in the Diffrent level*/
    /*Try to create the directory in level 1 from root*/
    if ((RetValue=Mkdir(level1dir))<0)
        return  ERROR;
    FlushCache();
    /*Try to create the directory in level 2 from root*/
    if ((RetValue=Mkdir(level2dir))<0)
        return ERROR;
    FlushCache();
    if ((RetValue=Mkdir(level3dir))<0)
        return  ERROR;
    FlushCache();
  /*try to create the directory at level four from root directory.*/
    if ((RetValue=Mkdir(level5dir))<0)
        return   ERROR;
    FlushCache();
    /* Try to create Directory which is already present, it should return Error*/
    if ((RetValue=Mkdir(level6dir))>0)
        return ERROR;

    FlushCache();
/* Uncomment the following code if we want to create maximum number of directory
   until no space is availble on the disk.
   maximum directory entries in the root is:  FAT12 : 256, Fat16:512 , FAT32 : NOLIMIT*/

//
//    PutByte(DirectoryName,'a',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,':',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'/',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'e',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'s',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'t',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'d',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'i',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'r',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'1',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'/',l++,DEFAULT_MEMORY);
//
//   for(m=1; m <= 8 ; m++)
//   {
//      for( i='A' ;i <= 'Z';i ++)
//      {
//             PutByte(DirectoryName,i,l,DEFAULT_MEMORY);
//             for(j ='0'; j <='9'; j++)
//             {
//                for(k=1;k <=m; k++)
//                {
//                    PutByte(DirectoryName,j,k+l,DEFAULT_MEMORY);
//                }
//        LoopCount++;
//
//        if (LoopCount%128==0)
//        {
//          LoopCount++;
//          LoopCount--;
//        }
//
//                if ((RetValue = Mkdir(DirectoryName)) < 0)
//                {
//                    FlushCache();
//          if ((RetValue==-16411)||(RetValue==-16392))
//            break
//          else
//                      return ERROR;
//                }
//             }
//      }
//   }
//
//
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFopen

//   Type:           Function
//
//   Description:    Test creatiopn of files

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          This test will crate the file up to no free clusteris avalibale
///                    //<
////////////////////////////////////////////////////////////////////////////////
#ifdef MAXFILEOPENTEST

int TestFopen(void)
{
    int i,Byte=0,j,m,k,l=0;
    _packed char btitle[30] = "Test Fopen";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    PutByte(DirectoryName,'a',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,':',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'/',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'E',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'S',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'D',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'I',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'R',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'1',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'/',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'E',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'S',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'C',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'O',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'P',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'Y',l++,DEFAULT_MEMORY);

  if ((Mkdir(rootdir))<0)
    return ERROR;

   for(m=1; m <= 8 ; m++)
   {
      for( i='0' ;i <= '9';i ++)
      {
             PutByte(DirectoryName,i,l,DEFAULT_MEMORY);
             for(j ='0'; j <='9'; j++)
             {
                for(k=1;k <=m; k++)
                {
                    PutByte(DirectoryName,j,k+l,DEFAULT_MEMORY);
                }
                if ((fout=Fopen(DirectoryName,(_packed char *)"w")) < 0)
                {
                    FlushCache();
                    return -1;
                }
                Fclose(fout);
             }
      }
   }

  return NOERROR;
}
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestRmdir

//   Type:           Function
//
//   Description:    Test the Deletion of Directory

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          if dir is not empty,it will return error,
//                   if try to remove root directory or Current working directory, it will return error.                   //<
////////////////////////////////////////////////////////////////////////////////
int    TestRmdir()
{

    _packed char btitle[30] = "Test Remove DIR";
  /*Clear Lcd*/
  LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /*Try to remove Directory from root*/
    if ((RetValue=Rmdir(rootdir))<0)
        return  ERROR;
    FlushCache();
    /*Try to Remove the directory  level 1 */
    if ((RetValue=Rmdir(level1dir))<0)
        return  ERROR;
    FlushCache();
    /*Try to Remove the directory in level 2 */
    if ((RetValue=Rmdir(level2dir))<0)
        return  ERROR;
    FlushCache();
    if ((RetValue=Rmdir(level3dir))<0)
        return  ERROR;
    FlushCache();
   /*Try to Remove Directory from level4*/
    if ((RetValue=Rmdir(level5dir))>0)
        return  ERROR;
    FlushCache();
    /*Try to remove the file using this function Rmdir, it will fail.*/
    if ((RetValue=Rmdir(testfile2))>0)
        return  ERROR;
    FlushCache();
    /*Try to Remove Directory Which is not empty it should return error*/
    if ((RetValue=Rmdir(level6dir))>0)
        return  ERROR;
    /*Try to remove root directory it should give error*/
    if ((RetValue=Rmdir(chroot))>0)
        return  ERROR;

    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFgetc

//   Type:           Function
//
//   Description:    Test the Fgetc

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Get one character from the file.                   //<
////////////////////////////////////////////////////////////////////////////////

int    TestFgetc()
{
    LONG FileSize;
    _packed char btitle[30] = "Test Get CHAR";
  /*Clear LCD*/
  LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
  /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /*Open the file in read mode*/
    if ((fout = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
    /*Get one character from the file and put it in the buffer*/
    if ((GetBuffer[1]=Fgetc(fout))<0)
        return  ERROR;
    /*Seek some Bytes and Get one character from the file and put it in the buffer*/
    Fseek(fout,100,SEEK_SET);
    if ((GetBuffer[2]=Fgetc(fout))<0)
        return  ERROR;
    /*Get the file size    */
    Fseek(fout,0,SEEK_END);
    FileSize=Ftell(fout);
    if ((RetValue=Fseek(fout,0,SEEK_SET))<0)
        return ERROR;
    /* Read whole file by getting one character from file*/
    for(i=0;i<FileSize;i++)
    {
        if ((GetBuffer[i]=Fgetc(fout))<0)
            return  ERROR;
    }
  /*Verify that we had get correct data */
    for(i=0;i<FileSize;i++)
    {
       if (GetBuffer[i]!=(0x000000+i))
           return ERROR;
    }
  Fclose(fout);
    if ((fout = Fopen(getfile,(_packed char *)"a"))<0)
        return  ERROR;

    /*Write back whole file*/
    for(i=0;i<FileSize;i++)
    {
        if ((RetValue=Fwrite(fout,&GetBuffer[i],1,DEFAULT_MEMORY,-1))<=0)
        {
            return ERROR;
        }
    }
    Fclose(fout);
    FlushCache();
    /*Open file i the append mode */
    if ((fout = Fopen(testfile2,(_packed char *)"a"))<0)
        return  ERROR;
    /* Try to get one character from file it should return EOF*/
    if ((GetBuffer[1]=Fgetc(fout))>0)
        return  ERROR;
   /*close open file */
    Fclose(fout);
    return  NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFgets

//   Type:           Function
//
//   Description:    Test the TestFgets

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Get one String from the file.                   //<
////////////////////////////////////////////////////////////////////////////////

int    TestFgets()
{
   _packed char *Char;
   INT Buffer[100];
    _packed char btitle[30] = "Test Get String";
   /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
  /*Open the file in read mode*/
    if ((fout = Fopen(testfile2,(_packed char *)"r"))<0)
        return  ERROR;
    /*Try to read one string from file*/
    Char = Fgets(fout,95,(_packed char *)Buffer);
    /*Open the file in append mode*/
    if ((fout1 = Fopen(testfile2,(_packed char *)"a+"))<0)
        return  ERROR;
    /*get back 10 bytes from EOF*/
    Fseek(fout1,-10,SEEK_CUR);
    /*get 25 bytes from current position it should read only 10 bytes*/
    Char = Fgets(fout1,25,(_packed char *)Buffer);
   /*close open file */
    Fclose(fout);
    Fclose(fout1);
    return  NOERROR;

}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFputc

//   Type:           Function
//
//   Description:    Test the TestFputc

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Put one Character to  the file.                   //<
////////////////////////////////////////////////////////////////////////////////
int    TestFputc()
{
    LONG FileSize;
    _packed char btitle[30] = "Test Put CHAR";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
  /*Open the file in read mode*/
    if ((fout = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
    /*Get the file size    */
    Fseek(fout,0,SEEK_END);
    FileSize=Ftell(fout);
    if ((RetValue=Fseek(fout,0,SEEK_SET))<0)
        return ERROR;
    /*read whole file with function Fgetc*/
    for(i=0;i<FileSize;i++)
    {
       if ((GetBuffer[i]=Fgetc(fout))<0)
       {
        return  ERROR;
       }
    }
   /*open the file in append mode to do write through function Fputc */
    if ((fout1 = Fopen(getfile,(_packed char *)"a"))<0)
        return  ERROR;
    /*Write whole file back through function Fputc*/
    for(i=0;i<FileSize;i++)
    {
        if ((Fputc(fout1,GetBuffer[i]))<0)
        {
            return  ERROR;
        }
    }
   /*close open file */
    Fclose(fout);
    Fclose(fout1);
    FlushCache();
    return  NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFputs

//   Type:           Function
//
//   Description:    Test the TestFputs

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Put one string from the file.                   //<
////////////////////////////////////////////////////////////////////////////////
int   TestFputs()
{
    INT Buffer[100];
    INT *Char;
    _packed char btitle[30] = "Test Put String";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*open the file in Read mode*/
    if ((fout = Fopen(testfile2,(_packed char *)"r"))<0)
        return  ERROR;
    Fseek(fout,100,SEEK_SET);
    if (Fgets(fout,55,(_packed char *)Buffer) == (_packed char *)0)
        return  ERROR;
   /*open the file in Write mode*/
    if ((fout1 = Fopen(putfile,(_packed char *)"w"))<0)
        return  ERROR;
  /*Put btitle string to the opend fie */
    Fputs(fout1,(INT*)btitle);
    if ((Char =Fputs(fout1,Buffer)) ==(INT *)0)
        return  ERROR;
  /*Put btitle string to the opend fie */
    Fputs(fout1,(INT*)btitle);
    /*close open file */
    Fclose(fout);
    Fclose(fout1);
    FlushCache();
    return  NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFclose

//   Type:           Function
//
//   Description:    Test the File close function

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          This test will check the Fclose function                 //<
////////////////////////////////////////////////////////////////////////////////

int   TestFclose()
{

    _packed char btitle[30] = "Test File Close";
   /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*open the file*/
    if ((fout = Fopen(testfile1,(_packed char *)"w"))<0)
        return  ERROR;
   /* Close the file*/
    if ((Fclose(fout))<0)
        return  ERROR;
    /*Again close the same file,it should return error*/
    if ((Fclose(fout))>0)
        return  ERROR;
    /*open the file*/
    if ((fout = Fopen(testfile1,(_packed char *)"a"))<0)
        return  ERROR;
    /*Try to write some bytes*/
    if ((RetValue=Fwrite(fout, BitBuffer,755,DEFAULT_MEMORY,-1))<0)
        return ERROR;
    /*close the file*/
    Fclose(fout);
    FlushCache();
    /*Try to write in closed file, it should return error*/
    if ((RetValue=Fwrite(fout, BitBuffer,755,DEFAULT_MEMORY,-1))<0)
        return NOERROR;

    return  NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFseek

//   Type:           Function
//
//   Description:    Test the File seek function

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          This test check the Fseek function.
                //<  SEEK_SET=seek from Starting point of  file
//                   SEEK_END=sekk from end
//                   SEEK_CUR = Seek the current position of file.
////////////////////////////////////////////////////////////////////////////////
int   TestFseek()
{
    INT  RetValue=0,Currentpointer=0;
    LONG Filesize;
    INT NumBytesToRead=30;
    INT *Buf=ReadBuffer;
    _packed char btitle[30] = "Test Seek File";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /*Open the file in read mode to perform test for Fseek function.*/
    if ((fin = Fopen(SeekFile,(_packed char *)"r"))<0)
        return  ERROR;
    /*Get the file size*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);

    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    /*Seek from start to arbitarily large location  and read*/
    if ((RetValue = Fseek(fin,0xc0000,SEEK_SET))<0)
        return  ERROR;
    Currentpointer=Ftell(fin);
    if ( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY,-1))<0)
        return ERROR;
    Buf += NumBytesToRead/3;
    /*Seek from start to 0 bytes and read*/
    if ((RetValue = Fseek(fin,0,SEEK_SET))<0)
        return  ERROR;
    if ( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY,-1))<0)
        return ERROR;
    Buf += NumBytesToRead/3;
    /*Seek from start to whole file and read*/
    if ((RetValue = Fseek(fin,Filesize,SEEK_SET))<0)
        return  ERROR;
    if ( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY,-1))>0)
        return ERROR;
    Buf += NumBytesToRead/3;
    /*Seek from end to 0 bytes and read*/
    if ((RetValue = Fseek(fin,0,SEEK_END))<0)
        return  ERROR;
    if ( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY,-1))>0)
        return ERROR;
    Buf += NumBytesToRead/3;
    /*Seek from end to whole file and read*/
    if ((RetValue = Fseek(fin,Filesize,SEEK_END))<0)
        return  ERROR;
    if ( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY,-1))<0)
        return ERROR;

    Buf += NumBytesToRead/3;
    /*Set the offest to some location yo check SEEK_CUR*/
    if ((RetValue = Fseek(fin,NumBytesToRead,SEEK_SET))<0)
        return  ERROR;

    /*Seek from current to whole file and read*/
    if ((RetValue = Fseek(fin,(-NumBytesToRead+Filesize),SEEK_CUR))<0)
        return  ERROR;
    if ( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY,-1))<0)
        return ERROR;

    Buf += NumBytesToRead/3;
    /*close open file*/
    Fclose(fin);
    return NOERROR;

}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           Testfilegetattrib

//   Type:           Function
//
//   Description:    Test the Get Attribute and File set attribute function

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////

int   Testfilegetattrib()
{
    INT Attribute;
    _packed char btitle[30] ="Test Attribute";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*Open the file in read mode to perform test filegetattrib*/
    if ((fin = Fopen(AttrFile,(_packed char *)"r"))<0)
        return  ERROR;
    /* get the current attribute of the file*/
    Attribute=filegetattrib(AttrFile);
    Attribute=filegetattrib(AttrFile);
    /* set the attribute*/
    filesetattrib(fin,READ_ONLY+ARCHIVE);
    /*GET the Attribute*/
    Attribute=filegetattrib(AttrFile);
  Fclose(fin);
  /*Try to open directory using function Fopen,it should return error*/
    if ((fin = Fopen(Attrdir,(_packed char *)"r"))>0)
        return  ERROR;
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           Testfilesetdate

//   Type:           Function
//
//   Description:    Test the Get Date and time and  set date and time function

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////

int   Testfilesetdate()
{

    DIR_DATE dirdate;
    DIR_TIME dirtime;
    _packed char btitle[30] = "Test Setdate File";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    if ((fin = Fopen(AttrFile,(_packed char *)"r"))<0)
        return  ERROR;
    /* Get the date*/
    filegetdate(fin,CREATION_DATE,&dirdate,&dirtime);
    /* give some date to set*/
    dirdate.Day = 2;
    dirdate.Month = 9;
    dirdate.Year = 2003;
    /* set the date*/
    filesetdate(AttrFile,CREATION_DATE,&dirdate,&dirtime);
  /*get the date*/
    filegetdate(fin,CREATION_DATE,&dirdate,&dirtime);
  /*set modification date for file opened in read mode*/
    dirdate.Day = 3;
    dirdate.Month = 8;
    dirdate.Year = 2003;
    filesetdate(AttrFile,MODIFICATION_DATE,&dirdate,&dirtime);
    /*set modification time for file*/
    dirtime.Second = 5;
    dirtime.Minute = 5;
    dirtime.Hour = 10;
    filesetdate(AttrFile,MODIFICATION_TIME,&dirdate,&dirtime);
  /*closed open file*/
  Fclose(fin);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestChdir

//   Type:           Function
//
//   Description:    Test the change Directory function

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          Chnge the directory to differnt level accoding to fatdir structure.
//                   This test also checks for the change directory to differrnt drive.
///                                     //<
////////////////////////////////////////////////////////////////////////////////

int   TestChdir()
{
    _packed char *buffer_1;
    _packed char btitle[30] = "Test Change DIR";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*change to given path*/
    if ((Chdir(level6dir))<0)
    return ERROR;
    /*Try to open file in this Directory*/
    if ((fin = Fopen(chfile,(_packed char *)"r"))<0)
        return  ERROR;
    Fclose(fin);
    /*try ro change at level1*/
    if ((Chdir(chlvel1))<0)
    return ERROR;
    /* get current working  Directory*/
    buffer_1 = Getcwd();
    if ((Chdir(chlevel2))<0)
    return ERROR;
    if ((Chdir(level6dir))<0)
    return ERROR;
    /*Change to level 3 and try to open file from there*/
    if ((Chdir(chlevel3))<0)
    return ERROR;
    if ((fin = Fopen(chfile2,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
    FlushCache();
  /*Again change to level 6 directory*/
    if ((Chdir(level6dir))<0)
    return ERROR;
  /*Try to change 4 level from Current Working Directory*/
    if ((Chdir(chlevel4))<0)
    return ERROR;
  /*Create the file in this directory*/
    if ((fin = Fopen(chfile2,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
    FlushCache();
    buffer_1 =Getcwd();
    if ((Chdir(level6dir))<0)
    return ERROR;
    if ((fin = Fopen(chfile,(_packed char *)"r"))<0)
        return  ERROR;
    Fclose(fin);

    /* Try to change to Root Directory*/
    if ((Chdir(chroot))<0)
    return ERROR;
    if ((fin = Fopen(chfile1,(_packed char *)"r"))<0)
        return  ERROR;
    Fclose(fin);
    /*change directory from root to 3 level from root */
    if ((Chdir(chdir))<0)
    return ERROR;
    if ((fin = Fopen(chfile2,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
    FlushCache();

    if ((Chdir(chdir1))<0)
    return ERROR;
    buffer_1 =Getcwd();
    if ((fin = Fopen(chfile3,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
  /* Check If External Media is present */
  if (FSMediaPresent(1))
  {
    /*Try to Change Directory from current Directory to different device Directory*/
    if ((Chdir(chdirb))<0)
    return ERROR;
    /*Try to open file in this Directory*/
    if ((fin = Fopen(chfileb,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
    FlushCache();
  }
    /*Again change directory from different device directory to current device directory*/
    if ((Chdir(chdirl))<0)
    return ERROR;
    /*Try to open file in this Directory*/
    if ((fin = Fopen(chfile4,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
    FlushCache();
    return NOERROR;
}


#ifdef FINDNEXT_TEST
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFindNext

//   Type:           Function
//
//   Description:

//   Inputs:         none

//   Outputs:        none
//
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////
int  _reentrant  TestFindNext()
{
    INT RetValue = 0,HandleNumber;
    Finddata _finddata;
  INT Fastfin,buffer[5],Buffer[20];
    INT LongBuffer[50];
    LONG Key;
    _packed char btitle[30] = "Test Find First Find Next";

    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    _memset(&_finddata,0,sizeof(_finddata));

    if ((HandleNumber = FindFirst(&_finddata,(_packed char *)FindBuffer))<0)
      return NOERROR;
  if ((Fastfin = FastOpen(_finddata.Key,(_packed char *)"w+"))<0)
     return ERROR;
  Fclose(Fastfin);
  //Buffer containing the file names of the given pattern
    _memcpy(&FileNames[count],_finddata.name,4);

    count+=4;
    _memset(&_finddata,0,sizeof(_finddata));

    while(1)
    {
        if ((RetValue = FindNext(HandleNumber,&_finddata)) <0)
          break;
      /*Open the file using function Fastopen*/
    if ((Fastfin = FastOpen(_finddata.Key,(_packed char *)"w+"))<0)
           return ERROR;
    /*Read some bytes from opened file*/
        if ((RetValue = Fread(Fastfin,Buffer,20,DEFAULT_MEMORY,-1))<0)
             return ERROR;
     /*Seek at the end of  file*/
      Fseek(Fastfin,0,SEEK_END);
    /*Write some bytes at the end od\f file*/
      if ((RetValue =Fwrite(Fastfin,BitBuffer,100,DEFAULT_MEMORY,-1))<0)
      {
          Fclose(Fastfin);
          return ERROR;
      }
    /*Seek back written bytes*/
      if ((RetValue=Fseek(Fastfin,-100,SEEK_CUR))<0)
      {
          Fclose(Fastfin);
          return ERROR;
      }
      /*read back data, we had written*/
      if ((RetValue = Fread(Fastfin,ReadBuffer,100,DEFAULT_MEMORY,-1))<0)
      {
          Fclose(Fastfin);
          return ERROR;
      }
    /*Verify the Read-write Operation of the file opened by function FastOpen*/
      if (BitBuffer[0]!= ReadBuffer[0])
      {
          Fclose(Fastfin);
          return ERROR;
      }

      Fclose(Fastfin);

        _memcpy(&FileNames[count],_finddata.name,4);

        count+=4;

      _memset(&_finddata,0,sizeof(_finddata));
    }
    _memset(&_finddata,0,sizeof(_finddata));


    //Change directory to one level up
    if ((RetValue = Chdir(FindFile))<0)
      return NOERROR;

  //Recursively called
    TestFindNext();

  return NOERROR;
}

#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFileread

//   Type:           Function
//
//   Description:    Test the file read .
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          This function test for the fread function.it read from differnt place in file
//                   and checks with the test pattern.
//<
////////////////////////////////////////////////////////////////////////////////
int  TestFileread()
{
    INT wRefPattern1 = 0x020100, wRefPattern2 = 0x030201, wRefPattern3 = 0x040302;
    INT wTestPattern, i, j;
    LONG Currentoffset=0;
    _packed char btitle[30] = "Test Read File";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /* Open file*/
    if ((fin = Fopen(testread,(_packed char *)"r"))<0)
        return  ERROR;

    if (fin >0)
    {
          /* Complete do loop reads and compares 256 bytes*/
        i = 2;
        do
        {

            j = 42;
            wTestPattern = 0x020100;
            do
            {
                RetValue = Fread(fin,Buffer,3,DEFAULT_MEMORY,-1);

                if (RetValue != ERROR)
                {
                    if (Buffer[0] != wTestPattern)
                    {
                        RetValue = 1;
                        break;
                    }
                    else
                        wTestPattern = wTestPattern + 0x030303;       /* Calculate new test pattern*/
                }
                else
                    break;
            }while(--j);

            if (RetValue != ERROR)
                /* Read 2 more bytes*/
                RetValue = Fread(fin,Buffer,2,DEFAULT_MEMORY,-1);
            else
                break;

        }while(--i);

        /*Test sector boundaries (position 512)*/
        if (RetValue != ERROR)
        {
            /* Read 6 bytes starting at byte 508*/
            Fseek(fin,0, SEEK_SET);     /* Seek to beginning of file*/
            Fseek(fin,(DWORD)508, SEEK_CUR);
            if ((RetValue = Fread(fin,Buffer,6,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            if ((Buffer[0] != 0x7E7D7C) || (Buffer[1] != 0x01007F))
                RetValue = ERROR;
        }

        if (RetValue != ERROR)
        {
            /* Read 3 bytes starting at 512 */
            Fseek(fin, (DWORD)512, SEEK_SET);
            if ((RetValue = Fread(fin,Buffer,3,DEFAULT_MEMORY,-1))<0)
                return ERROR;

            if (Buffer[0] != 0x020100)
                RetValue = ERROR;
        }

        if (RetValue != ERROR)
        {
            /* Read 3 bytes starting at 511*/
            Currentoffset=Ftell(fin);
            Fseek(fin,-4, SEEK_CUR);
            Currentoffset=Ftell(fin);
            if ((RetValue = Fread(fin,Buffer,3,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            Currentoffset=Ftell(fin);
            if (Buffer[0] != 0x01007f)
                return ERROR;
        }

        Fclose(fin);

    }
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFopenwRead
//
//   Type:           Function
//
//   Description:    Test the read mode for function Fopenw which Supports the UCS3 format.
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          This function is same as fopen except that passed name is in UCS3 format.
//           if file does not exist,it will return error.
//<          In this mode we can only read from the file
//////////////////////////////////////////////////////////////////////////////////
int  TestFopenwRead()
{
    _packed char btitle[30] = "Test Fopenw R";
    /* Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /* Open for read (will fail if file  does not exist) */
    if ((foutw = Fopenw((INT *)longfile1,(_packed char *)"r"))<0)
        return  ERROR;
  /*Do read operation from file opened in read mode*/
    if (Fread(foutw,ReadBuffer,56,DEFAULT_MEMORY,-1)<0)
    {
        Fclose(foutw);
        return ERROR;
    }
    /*Open the file which is exist in the read mode and try to do Fwrite if Fails*/
    if (Fwrite(foutw,BitBuffer,25,DEFAULT_MEMORY,-1)>0)
    {
        Fclose(foutw);
        return ERROR;
    }
    Fclose(foutw);
    FlushCache();
    return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFopenwWrite
//
//   Type:           Function
//
//   Description:    Test the write mode for function Fopenw which Supports the lUCS3 format.
//
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          This function is same as fopen except that passed name is in UCS3 format.
//           if file does not exist,function will create file in this mode.
//<          In this mode we can only write in the file
//////////////////////////////////////////////////////////////////////////////////
int  TestFopenwWrite()
{
    INT fin,BytesToRead;
    LONG Filesize,NumByteToWrite=0,NumBytesToRead;
    int BytesToWrite=512;
    _packed char btitle[30] = "Test Fopenw W";
    /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /* Open for write mode (will create the  file if  file  does not exist) */
    if ((foutw = Fopenw((INT *)longfile,(_packed char *)"w"))<0)
        return  ERROR;
    /* Open for Read mode for filling Readbuffer*/
    if ((fin=Fopenw((INT *)longfile1,(_packed char *)"r"))<0)
        return ERROR;
    /*Seek to the End of file to get file size*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    /* try to write  the whole file which is open in write mode*/
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;

            if ((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            if ((RetValue =Fwrite(foutw,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }
    }
  /*Close opened file*/
    Fclose(fin);
    Fclose(foutw);
    FlushCache();
    return  NOERROR;
}
//////////////////////////////////////////////////////////////////////////////////
////
////>  Name:           TestFopenwAppend
////
////   Type:           Function
////
////   Description:    Test the append mode for function Fopenw
////   Inputs:         none
////
////   Outputs:        none
////
////   Notes:          This function is same as fopen except that passed name is in UCS3 Format.
/////          in this mode the data will append at the end of file.if file does not exist
////           function will create the file in this mode
////<          In this mode we can only write in the file
//////////////////////////////////////////////////////////////////////////////////
int  TestFopenwAppend()
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed char btitle[30] = "Test Fopenw A";
    /*Clear LCD */
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
      /* Open for append mode (will create the  file if  file  does not exist) */
    if ((foutw = Fopenw((INT *)longfile,(_packed char *)"a"))<0)
        return  ERROR;
    if ((fin=Fopen(testfile3,(_packed char *)"r"))<0)
        return ERROR;
  /*GET the filesize*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
  /* Append the file */
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;

            if ((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            if ((RetValue =Fwrite(foutw,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }
    }
  /*Close Open file*/
    Fclose(foutw);
    Fclose(fin);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestReadplusMode
//
//   Type:           Function
//
//   Description:    Test the Readplus mode for function Fopenw
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          This function is same as fopen except that passed name is in UCS3 Format
//                   In this mode we can read as well write in the file.
//           if file does not exist ,function should return error in this mode
//<
////////////////////////////////////////////////////////////////////////////////
int  TestFopenwReadPlus()
{
    INT fin,BytesToRead;
    LONG Currentoffset=0,NumBytesToRead,Filesize,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed char btitle[30] = "Test Fopenw Plus";
    /* Display Test Title*/
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*open for read plus mode(will fail if file  does not exist)*/
    if ((fout=Fopenw((INT *)longfile1,(_packed char *)"r+"))<0)
        return ERROR;

    if ((fin=Fopen(readfile,(_packed char *)"r"))<0)
        return ERROR;
  /*Get the filesize*/
    if ((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
  /*We want to write at the end of file so Seek up to end of file*/
    if ((RetValue=Fseek(fout,0,SEEK_END))<0)
        return ERROR;
    /* Try to write  the whole file which is open in write mode*/
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;

            if ((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
            Currentoffset=Ftell(fout);
            if ((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,DEFAULT_MEMORY,-1))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }
    }
     Fclose(fin);
     Fclose(fout);
     FlushCache();
     return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestWriteplusMode
//
//   Type:           Function
//
//   Description:    Test the writeplus mode for function Fopenw
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          This function is same as fopen except that passed name is in UCS3 Format.
//                   In this mode we can read as well write in the file
//                   if the file does not present , Function will creates the file in this mode.
//<
////////////////////////////////////////////////////////////////////////////////
int   TestFopenwWritePlus()
{

    _packed char btitle[30] = "Test Fopenw Wplus";
    
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
  /*Open the file in write mode */
    if ((fout=Fopenw((INT *)longfile2,(_packed char *)"w+"))<0)
        return ERROR;
  /*Seek at the end of file*/
    if ((RetValue=Fseek(fout,0,SEEK_END))<0)
        return ERROR;
    /*write some data at the End of file*/
    if ((RetValue =Fwrite(fout,BitBuffer,100,DEFAULT_MEMORY,-1))<0)
    {
        /* We don't need to recover from this error.  So, don't have
         * to close file.
         */
        return ERROR;
    }
  /*Seek back the written data*/
    if ((RetValue=Fseek(fout,-100,SEEK_CUR))<0)
    {
        return ERROR;
    }
  /*Read the written data*/
    if ((RetValue = Fread(fout,ReadBuffer,100,DEFAULT_MEMORY,-1))<0)
    {
        return ERROR;
    }
    /*check thr read-write operation for this mode*/
    if (BitBuffer[0]!= ReadBuffer[0])
        return ERROR;

    Fclose(fout);
    FlushCache();
    return NOERROR;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestAppendplusMode
//
//   Type:           Function
//
//   Description:    Test the writeplus mode for function Fopenw
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          This function is same as fopen except that passed name is in UCS3 Format.
//          In this mode we can read as well write in the file
//                   if the file does not present, function will create the file in this mode.
//<
////////////////////////////////////////////////////////////////////////////////

int   TestFopenwAppendPlus()
{

    _packed char btitle[30] = "Test Fopenw APlus";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*open file in appendplus mode (will create thes file if not present)*/
    if ((fout=Fopenw((INT *)longfile1,(_packed char *)"a+"))<0)
        return ERROR;
    /*Write to the file it should erite at the end of the file*/
    if ((RetValue =Fwrite(fout,BitBuffer,150,DEFAULT_MEMORY,-1))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /* seek back the bytes write*/
    if ((RetValue =Fseek(fout,-150,SEEK_CUR))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /*Try to read bytes which we had written*/
    if ((RetValue = Fread(fout,ReadBuffer,150,DEFAULT_MEMORY,-1))<0)
    {
        Fclose(fout);
        return ERROR;
    }
  /*Verify the read write operation for this mode*/
    if (BitBuffer[0]!= ReadBuffer[0])
        return ERROR;

    Fclose(fout);
    FlushCache();
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestWriteFile

//   Type:           Function
//
//   Description:    Test the file write.
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          Checks the function fwrite.This Test verify function Fwrite,Fread and Fseek.
//
//<
////////////////////////////////////////////////////////////////////////////////

int  TestWriteFile()
{
    INT iRetVal, iBytesToWrite, i, j,Currentposotion=0;
    LONG lError;
    char bHandleSource, bHandleDest, bEndCopyFlag=0;
    WORD wProgBarPosX = 0, wUpdateDisplay, wUpdateTicks;
    _packed char btitle[30] = "Test Write File";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /* Fill the buffers with patterns*/
    for(i=0;i<NUM_WRITE_WORDS;i++)
    {
        pwFileBuffer[i] = -5592406; // 0xAAAAAA
        pwCompareBuffer[i] = 0x555555;
    }
    if ((fout = Fopen(bFileDest,(_packed char *)"w")) <0)
         return ERROR;
     FlushCache();
    /* Write the initial pattern to the file*/
    for(i=0;i<100;i++)
    {
        RetValue =Fwrite(fout,pwFileBuffer,NUM_WRITE_charS,DEFAULT_MEMORY,-1);

        if ((RetValue <0) || (RetValue != NUM_WRITE_charS))
        {
            /* Error writting - Close files*/
            Fclose(fout);
            FlushCache();
            return ERROR;
        }

    }
    if (Fseek(fout,0,SEEK_SET)<0)
         return ERROR;
    /* Overwrite the original pattern for every other chunk of NUM_WRITE_charS*/
    for(i=0;i<100/2;i++)
    {
        RetValue =Fwrite(fout,pwCompareBuffer,NUM_WRITE_charS,DEFAULT_MEMORY,-1);

        if ((RetValue <0) || (RetValue != NUM_WRITE_charS))
        {
            /* Error writting - Close files*/
            Fclose(fout);
            return ERROR;
        }
        if (Fseek(fout,NUM_WRITE_charS,SEEK_CUR)<0)
        {
            /* Error seeking - Close files*/
            Fclose(fout);
            return ERROR;
        }
    }
    /* We should be at the end of the file*/
    if (Feof(fout)> 0)
    {
        /* Error seeking - Close files*/
            Fclose(fout);
            return ERROR;
    }
    /* Close the file then open it again in READ mode*/
    Fclose(fout);
    FlushCache();

    if ((fin = Fopen(bFileSource,(_packed char *)"r")) <0)
    {
        Fclose(fin);
        return ERROR;
    }
    /* Verify that every other chunk was changed correctly */
    /* First verify the overwritten chunks  */
    for(i=0;i<100/2;i++)
    {
        /* We're comparing words not bytes so clear the read buffer & set the
        /*  last word to the expected pattern.*/
        for(j=0;j<(NUM_WRITE_WORDS-1);j++)
        {
            pwFileBuffer[j] = 0;
        }
        pwFileBuffer[NUM_WRITE_WORDS-1] = 0x555555;

        /* Overwrite original data*/
        RetValue = Fread(fin,pwFileBuffer,NUM_WRITE_charS,DEFAULT_MEMORY,-1);

        if ((RetValue <0) ||(RetValue != NUM_WRITE_charS))
        {
            /* Error reading - Close files*/
            Fclose(fin);
            count1++;
            return ERROR;
        }
        Currentposotion=Ftell(fin);
        /* Skip past a chunk  */
        if (Fseek(fin,NUM_WRITE_charS, SEEK_CUR) == ERROR)
        {
            /* Error seeking - Close files*/
            Fclose(fin);
            count1++;
            return ERROR;
        }
        Currentposotion=Ftell(fin);
        for(j=0;j<NUM_WRITE_WORDS;j++)
        {
            if (pwFileBuffer[j] != 0x555555)
            {
                /* Error reading - Close files */
                Fclose(fin);
                count1++;
                return ERROR;
            }
        }
        count1++;
    }
    /* Now verify the original chunks */
    /* We should be at the end of the file */
    if (Feof(fin)>0)
    {
        /* Error seeking - Close files  */
        Fclose(fin);
        return ERROR;
    }
    /* Rewind the file */
    if (Fseek(fin,0, SEEK_SET)<0)
    {
        /* Error seeking - Close files */
        Fclose(fin);
        return ERROR;
    }
    /* Check 'em */
    for(i=0;i<100/2;i++)
    {
        /* We're comparing words not bytes so clear the read buffer & set the
        /*  last word to the expected pattern. */
        for(j=0;j<(NUM_WRITE_WORDS-1);j++)
        {
            pwFileBuffer[j] = 0;
        }
        pwFileBuffer[NUM_WRITE_WORDS-1] = -5592406; // 0xAAAAAA

        /* Skip past the overwritten pattern */
        if (Fseek(fin, NUM_WRITE_charS, SEEK_CUR) == ERROR)
        {
            /* Error seeking - Close files  */
            Fclose(fin);
            return ERROR;
        }
        /* Read the original data */
        RetValue = Fread(fin,pwFileBuffer,NUM_WRITE_charS,DEFAULT_MEMORY,-1);

        if ((RetValue<0) || (RetValue != NUM_WRITE_charS))
        {
            /* Error reading - Close files */
            Fclose(fin);
            return ERROR;
        }
        for(j=0;j<(NUM_WRITE_WORDS-1);j++)
        {
            if (pwFileBuffer[j] != 0xAAAAAA)
            {
                /* Error reading - Close files*/
                Fclose(fin);
                return ERROR;
            }
        }
    }
    Fclose(fin);
    Fclose(fout);
    FlushCache();
    return NOERROR;
}
#ifdef VOLUMETEST
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestVolumelabel

//   Type:           Function
//
//   Description:    Test the funcyion getvolumelabel and setvolumelabel.                   
//   Inputs:         none 
//
//   Outputs:        none
//                   
////////////////////////////////////////////////////////////////////////////////
int  TestVolumelabel(INT Device)
{
    _packed char btitle[20] = "Test volumeid";
    _packed char volumebuffer[]="sigmatel"; 
	int Tempbuffer[11];
	/*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       
    /* Display Test Title*/
    FS_SwizzleText(btitle);
    FS_SwizzleText(volumebuffer);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);

    /*get the volume label for given drive*/
   	GetVolumeLabel((_packed char *)Tempbuffer,Device);
    /*set the volume label for given drive*/
	if(SetVolumeLabel((_packed char *)volumebuffer,Device)<0)
		return ERROR;
    FlushCache();
    return NOERROR;

}
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFwritePMemory

//   Type:           Function
//
//   Description:    Test the file write and read using P MEMORY.

//   Inputs:         Memorytype
//
//   Outputs:        none
//
//   Notes:          According to pass the mermory type it read from file and put in
//                   passed memory buffer and read from that buffer
//                   Memory type is  P_MEMORY and X_MEMORY
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef MEMORYTEST
int  TestFwriteMemory(Memorytype)
{
    INT fin,BytesToRead;
    LONG FileSize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=128;
    _packed char btitle[30] = "Test memory";
  /*Clear LCD*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));
   /* Display Test Title*/
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
    /* Open the file in read mode*/
    if ((fin = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
    if (Memorytype==P_MEMORY)
    {
    /* Open the file for write in write mode*/
       if ((fout = Fopen(writefile2,(_packed char *)"w"))<0)
           return  ERROR;
    }
    else
    {
        if ((fout = Fopen(writefile2,(_packed char *)"a"))<0)
        return  ERROR;
    }

    /*Get the file size    */
    Fseek(fin,0,SEEK_END);
    FileSize=Ftell(fin);
    if ((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    /*Test the Fwrite by writing whole file*/
    NumBytesToRead =FileSize;
    BytesToRead =0;
    while(NumByteToWrite < FileSize)
    {
        if ((NumBytesToRead)>0)
        {
            if (NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
            if (Memorytype==P_MEMORY)
            {
                if ((RetValue = Fread(fin,(void *)PBuffer,BytesToRead,P_MEMORY,-1))<0)
                {
                   Fclose(fin);
                   return ERROR;
                }

                if ((RetValue =Fwrite(fout,(void *)PBuffer,BytesToRead,P_MEMORY,-1))<0)
                {
                    Fclose(fout);
                    return ERROR;
                }
             }
            else
            {
                if ((RetValue = Fread(fin,(void *)XBuffer,BytesToRead,X_MEMORY,-1))<0)
                {
                    Fclose(fin);
                    return ERROR;
                }

                if ((RetValue =Fwrite(fout,(void *)XBuffer,BytesToRead,X_MEMORY,-1))<0)
                {
                    Fclose(fout);
                    return ERROR;
                }
             }

        NumByteToWrite+=RetValue;
        }
    NumBytesToRead-=BytesToWrite ;
        if ((RetValue==0) ||(NumByteToWrite==FileSize))
        {
            NumByteToWrite = FileSize+1;
        }
    }
    Fclose(fin);
    Fclose(fout);
    FlushCache();
    return NOERROR;
}
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestMkdirMax
//
//   Type:           Function
//
//   Description:    Test the Creation of maximun number of Directory
//
//   Inputs:         none
//
//   Outputs:        none
//
//   Notes:          Maximum Root Directory entries Possible -> FAT 12=256 Or FAT16 = 512
//                                                       FAT 32  = No Limit
//                   //<
//////////////////////////////////////////////////////////////////////////////////
#ifdef MAXDIRECTORYTEST
int TestMkdirMax(INT DeviceNum)
{
    int i,Byte=0,j,m,k,l=0;
  int dircount = 0;
    _packed char btitle[30] = "Test Make DIR Max";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(btitle);
    PrintStringAddr(10, 3*8, (_packed unsigned char *) btitle);
  /* put the path of root directory*/
    PutByte(DirectoryName,'a',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,':',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'/',l++,DEFAULT_MEMORY);
   
   for(m=1; m <= 9 ; m++)
   {
	    for( i='A' ;i <= 'Z';i ++)
	    {
             PutByte(DirectoryName,i,l,DEFAULT_MEMORY);
             for(j ='0'; j <='9'; j++)
             {
                for(k=1;k <=m; k++)
                {
                    PutByte(DirectoryName,j,k+l,DEFAULT_MEMORY);
                }
                if((RetValue = Mkdir(DirectoryName)) < 0)
                {
                    break;
                }
				dircount++;
             }   
	    }
   }  
    FlushCache();
	if((FSFATType(DeviceNum)) == FAT12)
	{
		if (dircount == 256)
			return NOERROR;
	}
	else if((FSFATType(DeviceNum)) == FAT16)
	{
		if (dircount == 512)
			return NOERROR;
	}
	else if((FSFATType(DeviceNum)) == FAT32)
	{
		if (dircount == 2340)
			return NOERROR;
	}
    return ERROR;
} 
#endif                                                 

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          CopyCodeIntoBootmanagerArea
//
//   Type:          Function
//
//   Description:   This function copies functions from Y memory
//                  into the appropriate P memory area to save P 
//                  memory space.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize)
{
  INT i;

  for (i=0;i<wYCopySize;i++)
  {
    pPCopyStart[i] = pYCopyStart[i];
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           FS_SwizzleText(_packed char *pText)

//   Type:           Function
//
//   Description:    TSwizzle the text

//   Inputs:       _packed char *pText

//   Outputs:        none
//
////////////////////////////////////////////////////////////////////////////////

void  FS_SwizzleText(_packed char *pText)
{
    int bDone=0;
    unsigned int *pTmp=(unsigned int*)pText;
    unsigned int tmp;
    while(!bDone)
    {
        tmp = *pTmp;
        tmp = ((tmp & 0x0000ff)<<16) | ((tmp & 0x00ff00)) | ((tmp & 0xff0000)>>16);
        *pTmp++=tmp;
        if (!(tmp&0x0000ff) || !(tmp&0x00ff00)  || !(tmp&0xff0000))
            bDone = 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
//>  Name:          ConvertHexToASCII
//   Type:          Function 
//   Description:   Convert a nibble into its ASCII equivalent (string)
//   Inputs:        wInHex - Low nibble in hex format to be converted.
//   Outputs:       ASCII equivalent stored in a word.
//   Notes:         0-9 = 0x30-0x39
//                  A-F = 0x41-0x46       copy of func from lib source so it fits. 
//<
////////////////////////////////////////////////////////////////////////////////
#include "stringlib.h"
WORD _reentrant ConvertHexToASCII(WORD wInHex)
{
  WORD wResult = 0;
  WORD wByte;
  wByte = (wInHex & (WORD)0x00000F);
  wResult = 0x30;
  if (wByte > 9)
  {
    wResult += 7;
  }
  wResult += wByte;
  return wResult;
}

////////////////////////////////////////////////////////////////////////////////
//>  Name:          CreateNullSerialNumber
//   Description:   Returns the laser fuse ID
//   Inputs:        pBuffer         pointer to buffer to be populated with
//                                   the laser fuse ID
//   Outputs:       RETCODE
//   Notes:         This function is not portable and not re-entrant. 
//                  Func copy for test since serial num lib too big. 
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant CreateNullSerialNumber(P_SERIAL_NUMBER pSerialNumber)
{
    INT i;    
    pSerialNumber->wRawSizeInBytes = NUM_RAW_SERIAL_NUMBER_BYTES_NULL;
    pSerialNumber->wAsciiSizeInBytes = NUM_RAW_SERIAL_NUMBER_BYTES_NULL * 2;    
    for(i=0;i<pSerialNumber->wRawSizeInBytes;i++)
    {
        PutByteInArrayInY(
            pSerialNumber->wRaw,
            i,
            0x00);
    }
    for(i=0;i<pSerialNumber->wAsciiSizeInBytes;i++)
    {
        pSerialNumber->btAscii[i] = ' ';
    }
    return SUCCESS;
}
#if 0  // above 2 funcs solve first 2 link errs below     Dec20 2005
fattest.bat ALL D3500 NAND1 MMC
lk2410 E208 (0): Found unresolved external(s):
             FCreateNullSerialNumber - (dd_mmc_media.a:..\output_3500\obj\mmcserialnumberinit.obj)
             FConvertHexToASCII      - (dd_mmc_media.a:..\output_3500\obj\mmcserialnumberinit.obj)

             FHalMmcEraseBlocks      - (dd_mmc_data_drive.a:..\output_3500\obj\mmcdatadriveerase.obj)
             FMmcTestErase           - (dd_mmc_data_drive.a:..\output_3500\obj\mmcdatadriveerase.obj)
#endif

#pragma asm
  nolist
    include "resource.inc"
    include "sysequ.inc"
    include "sysresources.inc"
    list

  global  FSetIrqL,FGetIrqL,SetIrqL,GetIrqL
;;  global FSysMaskAllInterrupts,FSysUnMaskAllInterrupts,SysUnMaskAllInterrupts,SysMaskAllInterrupts
    global  SignalModule
    global  FMediaWrite, FMediaRead,FileSystemMode,FSDataDriveInit
    global SysWaitOnObject
    global FSysReleaseObject

    extern  SysLoadResource
    extern FDataDriveWriteSector
    extern FDataDriveReadSector
    extern FDataDriveInit
;;    extern FSysGetIrqLevel
;;    extern FSysSetIrqLevel

SIZEWRITEMODULEY    equ     $1fff
SIZEWRITEMODULEP    equ     $6000

    org     p,"FATTest_P":
SignalModule
FSetIrqL
FGetIrqL
;;FSysMaskAllInterrupts
;;FSysUnMaskAllInterrupts
;;SysUnMaskAllInterrupts
;;SysMaskAllInterrupts
GetIrqL
SetIrqL
SysWaitOnObject
FSysReleaseObject
;;FSysGetIrqLevel
;;FSysSetIrqLevel
Kernel_ContextSwitch
FKernel_SetTimeout
Overlay_IsTaskLoaded
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



    org y,".yFATTEST":
FileSystemMode      dc      1    
    
#pragma endasm 
	  // This is a hack.  It's a problem with building library
	  // for funclet and building test for non-funclet HACK.
#pragma asm
        org p,".ptextskiponeword":
        dc 1
#pragma endasm


