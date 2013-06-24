////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
//
// Filename: fattest.c
// Description: File System Test Program
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////


#include "fstypes.h"
#include "hwequ.h"
#include "mediadetection.h"
#include "project.h"
#include "messages.h"
#include "fattest.h"
#include "lcdsupport.h"
#include "FSAPI.h"
#include "fsproj.h"
#include "FileSpec.h"
#include "HandleTable.h"


#define PROFILER
extern HANDLETABLE *Handle;

extern INT maxhandles;
extern INT maxdevices;
extern INT maxcaches; 

extern int device[];
extern INT DriveLetter[];
extern _asmfunc void MyDebug(void);

// The cache buffers are allocated in the X memory
extern INT _X bufx[XBUF_SIZE];

// All other global, custom configurable data is in Y memory.
// Two handles are reserved by the file system, so the number of handles must be more than 2
extern INT _Y bufy[YBUF_SIZE];

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
#define NUM_WRITE_BYTES  1000
#define NUM_WRITE_WORDS  334
#define MAX_COPY_WORDS  500
#define NUM_COMPARE_READ_WORDS 250
#define DRIVE_TAG_RESOURCE_BIN      0x02




////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

void SetDclkCount(DWORD dwCount);
DWORD GetDclkCount(void);
RETCODE ProgramPll(INT iPdiv);
////////////////////////////////////////////////////////////////////////////////
//  Variables
////////////////////////////////////////////////////////////////////////////////
extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern WORD  InitFileSystem(INT device);
//void CacheManager(WORD Device);
void  FS_SwizzleText(_packed BYTE *pText);
void GetUnicodeString(_packed char *filepath,_packed char *buf,INT Strlen);
int   TestReadMode();
int   TestWriteMode();
int   TestAppendMode();  
INT GetBuffer[300];
INT DirectoryCount=0;
INT BitBuffer[512];
INT ReadBuffer[512];
INT buf[100];
INT buf1[100];
INT buf2[100];
INT FileNames[100];
INT count=0;
INT count1=0;

int  pwFileBuffer[MAX_COPY_WORDS+1];
int pwCompareBuffer[NUM_WRITE_WORDS+1];
INT   Buffer[MAX_COPY_WORDS+1];
INT _P PBuffer[300];
INT _X XBuffer[300];
int TestResult=TESTSUCCESS;
int i;
LONG CurrentOffset;
int RetValue=NOERROR;
INT fin,fout,fout1,fout2,fout3,fin1,foutw;


_packed char testget[]="a:\\file2.txt";
_packed char bigfile[]="a:\\test.wav";
_packed char writebig[]="a:\\copy.wav";
_packed char testread[]="a:\\file1.txt";
_packed char testfile[]="a:\\TEST.h";
_packed char testfile1[]="a:\\C\\C1\\test.txt";
_packed char Nofile[]="a:\\module\\sbrdecoder\\applysbr\\c\\src\\test.c";
_packed char handletest[]="a:\\Handletable.h";
_packed char putfile[]="a:\\putfile.h";
_packed char testfile2[]="a:\\MYDIR4\\test.h";
_packed char writefile[]="a:\\Handletable.h";
_packed char writefile1[]="a:\\writefile.h";
_packed char writefile2[]="a:\\testp.h";
_packed char testfile3[]="a:\\MYDIR1\\EOF.asm";
_packed char testfile4[]="a:\\C\\C2\\C3\\test.h";
_packed char testfile5[]="a:\\C\\C2";
_packed char testfile6[]="a:\\MYDIR1\\EOF.asm";
_packed char getfile[]="a:\\getfile1.h";
_packed char SeekFile[]="a:\\seek.wav";
_packed char AttrFile[]="a:\\Handletable.h";
_packed char chfile[]="a:\\MyDir\\SubDir\\SubDir1\\SubDir2\\SubDir3\\SubDir4\\test.asm";
_packed char chfile1[]="a:\\inp.hex";
_packed char chfile2[]=".\\temp.hex";
_packed char chfile3[]="temp.hex";
_packed char bFileDest[] ="a:\\test.asm";
_packed char bFileSource[] ="a:\\test.asm";
_packed char readfile[]="a:\\c\\test.c";


_packed char FindBuffer[] = "*.mp3";
_packed char FindFile[]="..";
_packed char WorkingDir[]="a:\\Songs1\\Songs2\\Songs3\\Songs4";

_packed char longfile[]="a:\\longfiletest.asm";
_packed char longfile1[]= "a:\\MyDir\\SubDir\\SubDir1\\SubDir2\\SubDir5\\FileSystem_input.inc";
_packed char longfile2[]="a:\\longfilenametest.asm";
_packed char ReadFILE[] = "Test.wav";
_packed char rootdir[]="a:\\testdir1";
_packed char rootdir1[]="a:\\C\\C2\\C3\\testdir1";
_packed char level1dir[]="a:\\MYDIR3\\testdir1";
_packed char level2dir[]="a:\\MYDIR2\\SubDir\\testdir1";
_packed char level3dir[]="a:\\MyDir\\testdir1";
_packed char level5dir[]="a:\\MyDir\\SubDir\\SubDir1\\SubDir6\\testdir1";
_packed char level6dir[]="a:\\MyDir\\SubDir\\SubDir1\\SubDir2\\SubDir3\\SubDir4";
_packed char chdir[]="a:\\C\\C2\\C3";
_packed char chdir1[]="a:\\MYDIR2\\SubDir";
_packed BYTE bTextFail[] = "Fail";

_packed char Attrdir[]="a:\\MyDir";

_packed char DeleteDirectory[]="a:\\MyDir";
_packed char chroot[]="\\";
_packed char chlvel1[]="..";
_packed char chlevel2[]="..\\..\\";
_packed char chlevel3[]="..\\..\\..\\..";
_packed char chlevel4[]="..\\..\\..\\..\\..";
_packed char chlevel5[]="..\\..\\..\\..\\..\\..";
int   TestFseek(void);

extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT Strlength(_packed char *Buffer);
extern CHAR FSGetByte(void *buffer, INT Offset,CHAR MemoryType);
extern void PutByte(void *buffer, INT Offset, CHAR byte,CHAR MemoryType);
extern _reentrant INT FindFirst(INT HandleNumber,Finddata *_finddata,_packed char *FileName);
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern void PutWord(void *buffer, INT Offset, INT word,CHAR MemoryType);
extern RETCODE _reentrant MediaDiscoverAllocation(WORD wLogMediaNumber);
extern RETCODE _reentrant MediaInit(WORD wLogMediaNumber);
extern RETCODE _reentrant DriveInit(WORD wLogDriveNumber);
extern INT  FSInit(INT _X *bufx, INT _Y *bufy, INT maxdevices, INT maxhandles, INT maxcaches);
int TestFwriteProfile(void);
int TestHandle(void);
int TestReadMode(void);
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
int TestMkdirMax(void);
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
int _reentrant TestFindNext(void);

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
void main(void)
{
    INT Strlen;

    BYTE bDeviceCounter = MAX_LOGICAL_DEVICES - 1;
    BYTE bNumberDevTested = 0;
    _packed BYTE bEndOfTest[] = "End of Test";
    _packed BYTE bDevTested[] = "Dev(s) Tested :";
    
    MyDebug();
    
    if(ProgramPll(0x006000) != NOERROR)
    {
        #pragma asm
            debug
        #pragma endasm
    }


    // Init the media
    if(MediaInit(0) != SUCCESS)
    {
        SystemHalt();
    }

    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }

#ifdef MMC
        if(DriveInit(5) != SUCCESS)
#else
        if(DriveInit(4) != SUCCESS)
#endif      
    {
        SystemHalt();
    }

 
//#ifdef MMC
//    MediaInit(1);
//#endif    

    DriveInit(0);

//#ifdef MMC    
//    DriveInit(1);
//#endif

    FSInit(bufx, bufy, maxdevices, maxhandles, maxcaches);           // Initialize the file system

    DisplayModuleInit(ModuleTableDisplay);
	SetContrast(10);

    Delay(200);
    
	LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

    /*Swizzle the given text string*/
    FS_SwizzleText((_packed BYTE *)Nofile);
    FS_SwizzleText((_packed BYTE *)testfile);
    FS_SwizzleText((_packed BYTE *)testfile1);
    FS_SwizzleText((_packed BYTE *)testfile2);
    FS_SwizzleText((_packed BYTE *)testfile3);
    FS_SwizzleText((_packed BYTE *)testfile4);
    FS_SwizzleText((_packed BYTE *)testfile5);
    FS_SwizzleText((_packed BYTE *)testfile6);
    FS_SwizzleText((_packed BYTE *)readfile);
    FS_SwizzleText((_packed BYTE *)handletest);
    FS_SwizzleText((_packed BYTE *)writefile);
    FS_SwizzleText((_packed BYTE *)writefile1);
    FS_SwizzleText((_packed BYTE *)getfile);
    FS_SwizzleText((_packed BYTE *)rootdir);
    FS_SwizzleText((_packed BYTE *)rootdir1);
    FS_SwizzleText((_packed BYTE *)level1dir);
    FS_SwizzleText((_packed BYTE *)level2dir);
    FS_SwizzleText((_packed BYTE *)level3dir);
    FS_SwizzleText((_packed BYTE *)level5dir);
    FS_SwizzleText((_packed BYTE *)level6dir);
    FS_SwizzleText((_packed BYTE *)chdir);
    FS_SwizzleText((_packed BYTE *)chdir1);   
    FS_SwizzleText((_packed BYTE *)SeekFile);
    FS_SwizzleText((_packed BYTE *)AttrFile);
    FS_SwizzleText((_packed BYTE *)Attrdir);
    FS_SwizzleText((_packed BYTE *)chfile);
    FS_SwizzleText((_packed BYTE *)chfile1);
    FS_SwizzleText((_packed BYTE *)chfile2);
    FS_SwizzleText((_packed BYTE *)chfile3);
    FS_SwizzleText((_packed BYTE *)chroot);
    FS_SwizzleText((_packed BYTE *)chlvel1);
    FS_SwizzleText((_packed BYTE *)chlevel2);
    FS_SwizzleText((_packed BYTE *)chlevel3);
    FS_SwizzleText((_packed BYTE *)chlevel4);
    FS_SwizzleText((_packed BYTE *)chlevel5);
    FS_SwizzleText((_packed BYTE *)FindFile);
    FS_SwizzleText((_packed BYTE *)FindBuffer);
    FS_SwizzleText((_packed BYTE *)WorkingDir);
    FS_SwizzleText((_packed BYTE *)longfile);
    FS_SwizzleText((_packed BYTE *)longfile1);
    FS_SwizzleText((_packed BYTE *)longfile2);
    FS_SwizzleText((_packed BYTE *)bFileDest);
    FS_SwizzleText((_packed BYTE *)bFileSource);
    FS_SwizzleText((_packed BYTE *)testread);
    FS_SwizzleText((_packed BYTE *)testget);
    FS_SwizzleText(bTextFail);
    FS_SwizzleText((_packed BYTE *)writefile2);
    FS_SwizzleText((_packed BYTE *)putfile);
    FS_SwizzleText((_packed BYTE *)bigfile);
    FS_SwizzleText((_packed BYTE *)writebig);
    FS_SwizzleText(bEndOfTest);
    FS_SwizzleText(bDevTested);
    FS_SwizzleText((_packed BYTE *)ReadFILE);
    FS_SwizzleText((_packed BYTE *)DeleteDirectory);
    
    Strlen = Strlength((_packed char*)longfile); 
    GetUnicodeString(longfile,(_packed char*)buf,Strlen);
    
    
    Strlen = Strlength(longfile1); 
    GetUnicodeString(longfile1,(_packed char*)buf1,Strlen);
    
    Strlen = Strlength(longfile2); 
    GetUnicodeString(longfile2,(_packed char*)buf2,Strlen);
    
    /*fill the buffer for test*/
    for(i=0;i<512;i++)
    {
      BitBuffer[i]=100+i;
    }
    
//	if(TestFwriteProfile()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test the Handle */
//
//    if(TestHandle()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /* Test Fopen in read("r") mode*/
//    if(TestReadMode()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /* Test Fopen in write("w") mode*/
//    if(TestWriteMode()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//
//    /* Test Fopen in append ("a") mode*/
//    if(TestAppendMode()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /* Test Fopen in readplus ("r+") mode*/
//    if(TestReadPlusMode()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /* Test Fopen in writeplus ("w+") mode*/
//    if(TestWritePlusMode()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /* Test Fopen in appendplus("a+") mode*/
//    if(TestAppendPlusMode()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test the end of file*/    
//    if(TestFeof()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    if(TestFileread()==ERROR) 
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    if(TestWriteFile()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
////     /*Test the Fremove*/
//    if(TestFremove()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Ftell*/
//    if(TestFtell()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
    /*Test the Make Directories*/
    if(TestMkdir()==ERROR)   
    {
        PrintStringAddr(40, 5*8, bTextFail);
        while(1);
    }
//    /* Test the Remove Directories*/ 
//    if(TestRmdir()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fgetc (Get one character from file)*/ 
//    if(TestFgetc()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    
//    /*Test Fgets (Get one string from file)*/ 
//    if(TestFgets()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fputc (Put one character to the file)*/ 
//    if(TestFputc()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fputs (Put one string to the file)*/ 
//    if(TestFputs()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fileclose*/
//    if(TestFclose()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    if(TestFseek()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Testfilegetattrib*/
//    if( Testfilegetattrib()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Testfilesetdate*/
//    if( Testfilesetdate()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Change Directories*/
//    if(TestChdir()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//
//    /*Test Fopenw for Read(r) mode*/
//    if(TestFopenwRead()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fopenw for Write(w) mode*/
//    if(TestFopenwWrite()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fopenw for Append(a) mode*/
//    if(TestFopenwAppend()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fopenw for Rplus mode*/
//    if(TestFopenwReadPlus()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fopenw for Wplus mode*/
//    if(TestFopenwWritePlus()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fopenw for Aplus mode*/
//    if(TestFopenwAppendPlus()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fred and Fwrite from P MEMORY*/
//    if(TestFwritePMemory()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    /*Test Fred and Fwrite from X MEMORY*/
//    if(TestFwriteXMemory()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    if(TestDeletTree()==ERROR)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//    if((RetValue=Chdir(WorkingDir)) <0)
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }

//    if(TestFindNext()==ERROR)   
//    {
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//    }
//	if(TestFopen()==ERROR)
//	{
//        PrintStringAddr(40, 5*8, bTextFail);
//        while(1);
//	}


    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

	PrintStringAddr(5, 2*8, bEndOfTest);

	PrintStringAddr(1, 4*8, bDevTested);

	PrintNumber(84, 4*8, bNumberDevTested, 2, '0');

//            
   while(1);
}

INT TestDeletTree(void)
{
	INT RetValue =0;
	if((RetValue =DeleteTree(DeleteDirectory)) <0)
		return RetValue;
	FlushCache();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFwrite

//   Type:           Function
//
//   Description:    Test the File Write function

//   Inputs:         none 

//   Outputs:        none
//                   
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////
int   TestFwriteProfile()
{
    INT fin,BytesToRead;
    LONG FileSize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=1500;
    
#ifdef PROFILER
	ProfilerInit();
#endif
    if ((fin = Fopen(bigfile,(_packed char *)"r"))<0)
        return  ERROR;
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
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,Y_MEMORY))<=0)
            {
                Fclose(fin);
                return ERROR;
            }
#ifdef PROFILER
	ProfilerStop(0);
	ProfilerStart(1);
#endif
            if((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,Y_MEMORY))<=0)
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
    FlushCache();
    Fclose(fin);
    return NOERROR;
} 


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetDclkCount
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        none          
//
//   Outputs:       DWORD               DCLK count
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
DWORD GetDclkCount(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SetDclkCount
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        DWORD               DCLK count          
//
//   Outputs:       none               
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void SetDclkCount(DWORD dwCount)
{
    // TODO:  protect this from interrupts
    HW_DCLKCNTU.I = (WORD)(dwCount >> 24);
    HW_DCLKCNTL.I = (WORD)(dwCount);
}
        
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
RETCODE ProgramPll(INT iPdiv)
{
    // Get it started
    HW_CCR.I = iPdiv|HW_CCR_CKRST_SETMASK|HW_CCR_PLLEN_SETMASK|HW_CCR_ACKEN_SETMASK;

    // Start the lock timer counter
    HW_CCR.B.LTC = 1;
    
    // delay for 3*10k clocks.
    // Only need to wait for 12.288k but what the heck.
    // TODO:  The following asm code should be replaced with a C function
#pragma asm
    clr     a
    move    #>$001388,a0
_wait_loop
    dec     a
    jne     _wait_loop
#pragma endasm
    
    // We're locked so change the digital clock source to the PLL
    HW_CCR.B.CKSRC = 1;
    
    return NOERROR;
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
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////

int  TestHandle()
{
 
    _packed BYTE bTitle[] = "Test Handle"; 
    
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
 
    for(i=FIRST_VALID_HANDLE;i<maxhandles;i++)
    {
    if ((fout=Fopen(handletest,(_packed char *)"r"))<0)
        return  ERROR;
    }
    if ((fout1=Fopen(handletest,(_packed char *)"r"))>0)
        return  ERROR;
    for(fout=FIRST_VALID_HANDLE;fout<maxhandles;fout++)
    {
        Fclose(fout);
    }
    Fclose(fout1);    
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
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
    
int TestReadMode()
{    
    _packed BYTE bTitle[] = "Test ReadMode"; 
    
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    /* Display Test Title*/
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
   
    /* Open for read (will fail if file  does not exist) */
    if ((fout=Fopen(testfile,(_packed char *)"r"))<0)
        return  ERROR;
   
    if(Fread(fout,ReadBuffer,56,DEFAULT_MEMORY)<0)
    {
        Fclose(fout);
        return ERROR;
    }              
    /*Open the file which is exist in the read mode and try to do Fwrite if Fails*/   
    if(Fwrite(fout,BitBuffer,25,DEFAULT_MEMORY)>0)
    {
        Fclose(fout);
        return ERROR;
    }
    Fclose(fout);
    /*Ooen file which is not exist,it should return error*/
    if ((fout = Fopen(Nofile,(_packed char *)"r"))>0)
        return  ERROR;
        
    Fclose(fout);
    FlushCache();
    return NOERROR;
}   
  
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
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
  
int   TestWriteMode()
{    
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed BYTE bTitle[] = "Test WriteMode"; 
    
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    /* Display Test Title*/
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    /* Open for write mode (will create the  file if  file  does not exist) */
    if ((fout=Fopen(writefile1,(_packed char *)"w"))<0)
        return ERROR;
    /* Open for Read mode for filling Readbuffer*/  
    if ((fin=Fopen(readfile,(_packed char *)"r"))<0)
        return ERROR;
    /*Seek to the End of file to get file size*/
    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;

    /* try to write  the whole file which is open in write mode*/   
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
       
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
            if((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }     
    }
//
//    if ((fout1 = Fopen(writefile1,"w"))>0)
//        return ERROR;
    Fclose(fin);
    Fclose(fout);           
    FlushCache();
    return  NOERROR;
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
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
int   TestAppendMode()
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed BYTE bTitle[] = "Test AppendMode"; 
    
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    /* Display Test Title*/
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
      /* Open for append mode (will create the  file if  file  does not exist) */
    if ((fout1=Fopen(writefile1,(_packed char *)"a"))<0)
        return ERROR;

    if ((fin=Fopen(testfile3,(_packed char *)"r"))<0)
        return ERROR;
        
    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    /* try to write  the whole file which is open in append  mode*/   
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
       
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
            if((RetValue =Fwrite(fout1,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }     
    }
      
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
//   Notes:          In this mode we can read as well write in the file
//<
////////////////////////////////////////////////////////////////////////////////

int   TestReadPlusMode()
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed BYTE bTitle[] = "Test ReadPlusMode"; 
    
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    /*Ooen file which is not exist,it should return error*/
    if ((fout = Fopen(Nofile,(_packed char *)"r+"))>0)
        return  ERROR;
    /*open for read plus mode(will fail if file  does not exist)*/
    if ((fout = Fopen(testfile,(_packed char *)"r+"))<0)
        return  ERROR;
        
    if ((fin=Fopen(testfile3,(_packed char *)"r"))<0)
        return ERROR;

    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
        
    /* try to write  the whole file which is open in write mode*/   
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
       
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<=0)
                return ERROR;
            if((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<=0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }     
    }
   
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
//                   if the file is not present the it creates the file
//<
////////////////////////////////////////////////////////////////////////////////

int   TestWritePlusMode()
{
    _packed BYTE bTitle[] = "Test WritePlusMode"; 
    
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
     /*opwn the file in the writeplus(w+) mode*/
    if ((fout = Fopen(testfile4,(_packed char *)"w+"))<0)
        return  ERROR;
    /*write some byte to file*/    
    if((RetValue =Fwrite(fout,BitBuffer,100,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    if((RetValue=Fseek(fout,-100,SEEK_CUR))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /*read back whatever we had written*/
    if((RetValue = Fread(fout,ReadBuffer,100,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    } 
   /*Check we had  done correctly write and read in this mode*/    
    if(BitBuffer[0]!= ReadBuffer[0])
    {
        Fclose(fout);
        return ERROR;
    }
            
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
//   Description:    Test the writeplus mode for function Fopen                   
//   Inputs:         none 
//
//   Outputs:        none
//                   
//   Notes:          In this mode we can read as well write in the file
//                   if the file is not present the it creates the file
//<
////////////////////////////////////////////////////////////////////////////////

int   TestAppendPlusMode()
{
    _packed BYTE bTitle[] = "Test AppendPlusMode"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    /*open file in appendplus mode (will create the file if not present)*/
    if ((fout = Fopen(writefile,(_packed char *)"a+"))<0)
        return  ERROR;
    /*Write to the file it should erite at the end of the file*/        
    if((RetValue =Fwrite(fout,BitBuffer,50,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /* seek back the bytes write*/
    if((RetValue =Fseek(fout,-50,SEEK_CUR))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /*Try to read bytes which we had write*/
    if((RetValue = Fread(fout,ReadBuffer,50,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
   /*Check we had  done correctly write and read in this mode*/    
    if(BitBuffer[0]!= ReadBuffer[0])
    {
        Fclose(fout);
        return ERROR;
    }
    
    FlushCache();   
    Fclose(fout);
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
//   Notes:          none

////////////////////////////////////////////////////////////////////////////////

int    TestFeof()
{
    _packed BYTE bTitle[] = "Test End Of File"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    /*Open file in the append mode*/
    if ((fout2 = Fopen(testfile,(_packed char *)"a+"))<0)
        return  ERROR;
   /*it should return the EOF*/     
    if(Feof(fout2)>0)
    {
        Fclose(fout2);
        return  ERROR;
    }    
    if((RetValue=Fwrite(fout2,BitBuffer,25,DEFAULT_MEMORY))<0)
    {
        Fclose(fout2);
        return ERROR;
    }
    if((RetValue=Fseek(fout2,-25,SEEK_CUR))<0)
        return  ERROR;         
    /* It should not Return EOF*/
    if((RetValue=Feof(fout2))<0)
    {
        Fclose(fout2);
        return  ERROR;
    } 
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
    
    _packed BYTE bTitle[] = "Test Delete Files"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    /*Remove the file*/ 
    if(Fremove(testfile)<0)
       return  ERROR; 
    FlushCache();
       
    /*Try to opend removed file it should give Error*/
    if ((fout = Fopen(testfile,(_packed char *)"r"))>0)
        return  ERROR;
   /*Open the file*/        
    if ((fout2 = Fopen(testfile3,(_packed char *)"r"))<0)
        return  ERROR;
    /* Try to Delete the opened file it Fails*/
    if(Fremove(testfile3)>0)
        return  ERROR;
    /*try to deleted readonly file,it should return error*/    
    if(Fremove(testfile6)>0)
        return  ERROR; 
   /*try to open deleted file,it should fail*/     
    if ((fout = Fopen(testfile6,(_packed char *)"r"))<0)
        return  ERROR;

    /*Try to Delete the Directory it should fail*/ 
    if(Fremove(testfile5)>0)
        return  ERROR;
        
    FlushCache();
    Fclose(fout2);
    Fclose(fout);
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
//   Notes:          none
//                   //<
////////////////////////////////////////////////////////////////////////////////

int   TestFtell()
{
    _packed BYTE bTitle[] = "Test Ftell"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    /*Open the file in read mode*/
    if ((fin=Fopen(testfile2,(_packed char *)"r"))<0)
        return  ERROR;
    /*Get current position in the file*/    
    CurrentOffset=Ftell(fin); 
    
    RetValue=Fseek(fin,51,SEEK_SET);
    CurrentOffset=Ftell(fin); 
    /*Open the file in Append mode and try to do Ftell it Should Return EOF*/
    
    if ((fin1=Fopen(testfile1,(_packed char *)"a"))<0)
        return  ERROR;
    CurrentOffset=Ftell(fin1); 
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
//   Notes:          Maxiumum Root Directory Possible -> FAT 12 Or FAT16 = 256
//                                                       FAT 32  = No Limit
//                   //<
////////////////////////////////////////////////////////////////////////////////
int LoopCount=0;                          
_packed char DirectoryName[50];

int TestMkdir()
{
    int i,Byte=0,j,m,k,l=0;
    _packed BYTE bTitle[] = "Test Make DIR"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
//    /*Try to Create Directory in the roort*/ 
//    if((RetValue=Mkdir(rootdir))<0)
//        return  ERROR;
//    FlushCache();
//
//    if((RetValue=Mkdir(rootdir))>0)
//        return  ERROR;
//    FlushCache();
//        
//    /*Try to Create Directory in the Diffrent level*/         
//    
//    if((RetValue=Mkdir(level1dir))<0)
//        return  ERROR;
//    FlushCache();
//        
//    if((RetValue=Mkdir(level2dir))<0)
//        return ERROR;
//    FlushCache();
//        
//    if((RetValue=Mkdir(level3dir))<0)
//        return  ERROR;
//    FlushCache();
//        
//    if((RetValue=Mkdir(level5dir))<0)
//        return   ERROR;
//    FlushCache();
//    /* Try to create Directory which is already avalibale it should return Error*/     
//    if((RetValue=Mkdir(level6dir))>0)
//        return ERROR;
        
    FlushCache();
    
    PutByte(DirectoryName,'a',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,':',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'\\',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'e',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'s',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'t',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'d',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'i',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'r',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'1',l++,DEFAULT_MEMORY);
//    PutByte(DirectoryName,'\\',l++,DEFAULT_MEMORY);
   
   for(m=1; m <= 8 ; m++)
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
				LoopCount++;

				if(LoopCount%128==0)
				{
					LoopCount++;
					LoopCount--;
				}

                if((RetValue = Mkdir(DirectoryName)) < 0)
                {
                    FlushCache(); 
					if((RetValue==-16411)||(RetValue==-16392))
						return NOERROR;
					else
	                    return ERROR;
                }
             }   
	    }
   }  
        
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
//   Notes:          //                   //<
////////////////////////////////////////////////////////////////////////////////
                             
int TestFopen(void)
{
    int i,Byte=0,j,m,k,l=0;
    _packed BYTE bTitle[] = "Test Fopen"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    PutByte(DirectoryName,'a',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,':',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'\\',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'E',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'S',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'D',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'I',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'R',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'1',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'\\',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'E',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'S',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'C',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'O',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'P',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'Y',l++,DEFAULT_MEMORY);

	if((Mkdir(rootdir))<0)
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
                if((fout=Fopen(DirectoryName,(_packed char *)"w")) < 0)
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

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestRmdir

//   Type:           Function
//
//   Description:    Test the Deletion of Directory 

//   Inputs:         none 

//   Outputs:        none
//                   
//   Notes:          //                   //<
////////////////////////////////////////////////////////////////////////////////

int    TestRmdir()
{
    
    _packed BYTE bTitle[] = "Test Remove DIR"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    /*Try to remove Directory from root*/ 
    if((RetValue=Rmdir(rootdir))<0)
        return  ERROR;

    FlushCache();    
    if((RetValue=Rmdir(level1dir))<0)
        return  ERROR;
    FlushCache();    

    if((RetValue=Rmdir(level2dir))<0)
        return  ERROR;
        
    FlushCache();    
    if((RetValue=Rmdir(level3dir))<0)
        return  ERROR;
    FlushCache();    

    if((RetValue=Rmdir(level5dir))>0)
        return  ERROR;
    FlushCache();
    /*Try to remove the file using thid function it failed*/
    if((RetValue=Rmdir(testfile2))>0)
        return  ERROR;
    FlushCache();    
    /*Try to Remove Directory Which is not empty it should return error*/
    
    if((RetValue=Rmdir(level6dir))>0)
        return  ERROR;
    /*Try to remove root directory it should give error*/        
    if((RetValue=Rmdir(chroot))>0)
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
    _packed BYTE bTitle[] = "Test Get CHAR"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    if ((fout = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
    /*Get one character from the file and put it in the buffer*/
    if((GetBuffer[1]=Fgetc(fout))<0)
        return  ERROR;
        
    /*Seek some Bytes and Get one character from the file and put it in the buffer*/
    
    Fseek(fout,100,SEEK_SET);
    
    if((GetBuffer[2]=Fgetc(fout))<0)
        return  ERROR;
        
    /*Get the file size    */
    Fseek(fout,0,SEEK_END);
    FileSize=Ftell(fout);
    
    if((RetValue=Fseek(fout,0,SEEK_SET))<0)
        return ERROR;
    
    /* Read whole file by getting one character from file*/
    for(i=0;i<FileSize;i++)                     
    { 
        if((GetBuffer[i]=Fgetc(fout))<0)
            return  ERROR;
    }
    for(i=0;i<FileSize;i++)                     
    { 
       if(GetBuffer[i]!=(0x000000+i))
           return ERROR;
    }
    if ((fout = Fopen(getfile,(_packed char *)"a"))<0)
        return  ERROR;
        
    /*Write back whole file*/
    for(i=0;i<FileSize;i++)
    { 
        if((RetValue=Fwrite(fout,&GetBuffer[i],1,DEFAULT_MEMORY))<=0)
        {
            Fclose(fout);
            return ERROR;
        } 
    }
    Fclose(fout);
    FlushCache();
    /*Open file i the append mode */ 
    if ((fout = Fopen(testfile2,(_packed char *)"a"))<0)
        return  ERROR;
    /* Try to get one character from file it should return EOF*/
    if((GetBuffer[1]=Fgetc(fout))>0)
        return  ERROR;
        
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
    _packed BYTE bTitle[] = "Test Get String"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    if ((fout = Fopen(testfile2,(_packed char *)"r"))<0)
        return  ERROR;
    /*Try to read one string from file*/
    Char = Fgets(fout,95,Buffer);

    /*Open the file in append mode*/
    if ((fout1 = Fopen(testfile2,(_packed char *)"a+"))<0)
        return  ERROR;

    /*get back 10 bytes from EOF*/
    Fseek(fout,-10,SEEK_CUR);
    /*get 25 bytes from current position it should read only 10 bytes*/
    Char = Fgets(fout,25,Buffer);
    
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

    _packed BYTE bTitle[] = "Test Put CHAR"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    
    if ((fout = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
        
    /*Get the file size    */
    Fseek(fout,0,SEEK_END);
    FileSize=Ftell(fout);
    
    if((RetValue=Fseek(fout,0,SEEK_SET))<0)
        return ERROR;
    
    /*read whole file with function Fgetc*/    
    for(i=0;i<FileSize;i++)                     
    { 
       if((GetBuffer[i]=Fgetc(fout))<0)
       {    
        Fclose(fout);
        return  ERROR;
       }
    }
    
    if ((fout1 = Fopen(getfile,(_packed char *)"a"))<0)
        return  ERROR;

    /*Write whole file back through function Fputs*/
    for(i=0;i<FileSize;i++)                     
    { 

        if((Fputc(fout1,GetBuffer[i]))<0)
        {                                        
            Fclose(fout1);
            return  ERROR;
        }
    }
    FlushCache();
    Fclose(fout);
    Fclose(fout1);
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
    _packed char *Char;
    
    _packed BYTE bTitle[] = "Test Put String"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    
    if ((fout = Fopen(testfile2,(_packed char *)"r"))<0)
        return  ERROR;
        
     Fseek(fout,100,SEEK_SET);
     Char = Fgets(fout,55,Buffer);
     
    if ((fout1 = Fopen(putfile,(_packed char *)"w"))<0)
        return  ERROR;
    Fputs(fout1,(INT*)bTitle);
    
    if((Char =Fputs(fout1,Buffer)) ==(_packed char*)0)
        return  ERROR;
    FlushCache();
    Fclose(fout);
    Fclose(fout1);
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
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////

int   TestFclose()
{
    
    _packed BYTE bTitle[] = "Test File Close"; 
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    /*open the file*/
    if ((fout = Fopen(testfile1,(_packed char *)"w"))<0)
        return  ERROR;
   /* Close the file*/     
    if((Fclose(fout))<0)
        return  ERROR;
    /*Again close the sme file,it should return error*/
    if((Fclose(fout))>0)
        return  ERROR;
    /*open the file*/    
    if ((fout = Fopen(testfile1,(_packed char *)"a"))<0)
        return  ERROR;
    /*try to write some bytes*/        
    if((RetValue=Fwrite(fout, BitBuffer,755,DEFAULT_MEMORY))<0)
        return ERROR;
    /*close the file*/    
    Fclose(fout);
    FlushCache();
    /*Try to write, it should return error*/
    if((RetValue=Fwrite(fout, BitBuffer,755,DEFAULT_MEMORY))<0)
        return NOERROR;

    return  NOERROR;
}  
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFwrite

//   Type:           Function
//
//   Description:    Test the File Write function

//   Inputs:         none 

//   Outputs:        none
//                   
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////
int   TestFwrite()
{
    INT fin,BytesToRead;
    LONG NumBytesToRead,NumByteToWrite=0,FileSize;
    int BytesToWrite=1500;
    
    
    if ((fin = Fopen(bigfile,(_packed char *)"r"))<0)
        return  ERROR;
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

            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,Y_MEMORY))<0)
            {
                Fclose(fin);
                return ERROR;
            }

            if((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,Y_MEMORY))<0)
            {
                Fclose(fout);
                return ERROR;
            }
        NumByteToWrite+=RetValue;
        }
    NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==FileSize))
        {
            NumByteToWrite = FileSize+1;
        }     
    }
    Fclose(fout);
    FlushCache();
    Fclose(fin);
    return NOERROR;
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
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////
                     
int   TestFseek()
{
    INT  RetValue=0;
    LONG Filesize;
    INT NumBytesToRead=30;
    INT *Buf=ReadBuffer;
    
    _packed BYTE bTitle[] = "Test Seek File";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    if ((fin = Fopen(SeekFile,(_packed char *)"r"))<0)
        return  ERROR;
    
    /*Get the file size*/    
    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;

        
    //Seek from start to arbitarily large location  and read
    if ((RetValue = Fseek(fin,0xc0000,SEEK_SET))<0)
        return  ERROR;
    if( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY))<0)
        return ERROR;
    
    Buf += NumBytesToRead/3;

    //Seek from start to 0 bytes and read
    if ((RetValue = Fseek(fin,0,SEEK_SET))<0)
        return  ERROR;

    if( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY))<0)
        return ERROR;

    Buf += NumBytesToRead/3;

    //Seek from start to whole file and read

    if ((RetValue = Fseek(fin,Filesize,SEEK_SET))<0)
        return  ERROR;

    if( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY))>0)
        return ERROR;

    Buf += NumBytesToRead/3;

    //Seek from end to 0 bytes and read
    if ((RetValue = Fseek(fin,0,SEEK_END))<0)
        return  ERROR;
    if( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY))>0)
        return ERROR;

    Buf += NumBytesToRead/3;

    //Seek from end to whole file and read
    if ((RetValue = Fseek(fin,Filesize,SEEK_END))<0)
        return  ERROR;
        
    if( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY))<0)
        return ERROR;

    Buf += NumBytesToRead/3;

    //Set the offest to some location yo check SEEK_CUR
    if ((RetValue = Fseek(fin,NumBytesToRead,SEEK_SET))<0)
        return  ERROR;
    
    //Seek from current to whole file and read
    if ((RetValue = Fseek(fin,(-NumBytesToRead+Filesize),SEEK_CUR))<0)
        return  ERROR;
    if( (RetValue = Fread(fin,Buf,NumBytesToRead,DEFAULT_MEMORY))<0)
        return ERROR;

    Buf += NumBytesToRead/3;
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
    _packed BYTE bTitle[] ="Test Attribute";
    
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    
    if ((fin = Fopen(AttrFile,(_packed char *)"r"))<0)
        return  ERROR;
    
    /* get the current attribute of the file*/
    Attribute=filegetattrib(fin);
    if ((fin = Fopen(Attrdir,(_packed char *)"r"))>0)
        return  ERROR;                                          
    Attribute=filegetattrib(fin);  
    /* set the attribute*/                            
    filesetattrib(fin,READ_ONLY+ARCHIVE+DIRECTORY); 
    /*GET the Attribute*/           
    Attribute=filegetattrib(fin);
    Fclose(fin);                                            
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
    _packed BYTE bTitle[] = "Test Setdate File";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    if ((fin = Fopen(AttrFile,(_packed char *)"r"))<0)
        return  ERROR;
    /* Get the date*/
    filegetdate(fin,CREATION_DATE,&dirdate,&dirtime);
    /* give some date to set*/
    dirdate.Day = 2;
    dirdate.Month = 9;
    dirdate.Year = 2003;
    /* set the date*/                               
    filesetdate(fin,CREATION_DATE,&dirdate,&dirtime);
    filegetdate(fin,CREATION_DATE,&dirdate,&dirtime);

    dirdate.Day = 3;
    dirdate.Month = 8;
    dirdate.Year = 2003;
    filesetdate(fin,MODIFICATION_DATE,&dirdate,&dirtime);
    
    dirtime.Second = 5;   
    dirtime.Minute = 5;   
    dirtime.Hour = 10;
          
    filesetdate(fin,MODIFICATION_TIME,&dirdate,&dirtime);
    
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
//   Notes:          none                //<
////////////////////////////////////////////////////////////////////////////////

int   TestChdir()
{
    INT *buffer_1;
 
    _packed BYTE bTitle[] = "Test Change DIR";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    /*change to given path*/
    if((Chdir(level6dir))<0)
    return ERROR;
    /*Try to open file in this Directory*/
    if ((fin = Fopen(chfile,(_packed char *)"r"))<0)
        return  ERROR;
    Fclose(fin);
    /*try ro change at level1*/    
    if((Chdir(chlvel1))<0)
    return ERROR;
    /* get current working  Directory*/
    buffer_1 =Getcwd();
    if((Chdir(chlevel2))<0)
    return ERROR;
    if((Chdir(level6dir))<0)
    return ERROR;
    /*Change to level 3 and try to open file from there*/
    if((Chdir(chlevel3))<0)
    return ERROR;
    if ((fin = Fopen(chfile2,(_packed char *)"w"))<0)
        return  ERROR;
      
    FlushCache();
    Fclose(fin);
    
    if((Chdir(level6dir))<0)
    return ERROR;
    if((Chdir(chlevel4))<0)
    return ERROR;
    if ((fin = Fopen(chfile2,(_packed char *)"w"))<0)
        return  ERROR;
    FlushCache();
    Fclose(fin);
    
    buffer_1 =Getcwd();
    if((Chdir(level6dir))<0)
    return ERROR;
    if ((fin = Fopen(chfile,(_packed char *)"r"))<0)
        return  ERROR;
    Fclose(fin);
        
    /* Try to change to Root Directory*/
    if((Chdir(chroot))<0)
    return ERROR;
    if ((fin = Fopen(chfile1,(_packed char *)"r"))<0)
        return  ERROR;
    Fclose(fin);
    
    /*Try to Change Directory from current Directory to diffrenr device Directory*/  
    if((Chdir(chdir))<0)
    return ERROR;
    if ((fin = Fopen(chfile2,(_packed char *)"w"))<0)
        return  ERROR;
    Fclose(fin);
    FlushCache();
    
    if((Chdir(chdir1))<0)
    return ERROR;
    buffer_1 =Getcwd();
    if ((fin = Fopen(chfile3,(_packed char *)"w"))<0)
        return  ERROR;
    
    Fclose(fin);    
    FlushCache();
    return NOERROR;
}
//////////////////////////////////////////////////////////////////////////////////
////
////>  Name:           TestFindNext
//
////   Type:           Function
////
////   Description:    
//
////   Inputs:         none 
//
////   Outputs:        none
////                   
////   Notes:          none                //<
//////////////////////////////////////////////////////////////////////////////////
int  _reentrant  TestFindNext()
{
    INT RetValue = 0,HandleNumber=2;
    Finddata _finddata;

    _packed BYTE bTitle[] = "Test Find First Find Next";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    Handle[HandleNumber]=Handle[0];

    _memset(&_finddata,0,sizeof(_finddata));
    if((RetValue = FindFirst(HandleNumber,&_finddata,FindBuffer))<0)
	    return NOERROR;

	//Buffer containing the file names of the given pattern
    _memcpy(&FileNames[count],_finddata.name,4);

    count+=4;
    _memset(&_finddata,0,sizeof(_finddata));
   
    while(1)
    {
        if((RetValue = FindNext(HandleNumber,&_finddata)) <0)
	        break;                                          

        _memcpy(&FileNames[count],_finddata.name,4);

        count+=4;
        
	    _memset(&_finddata,0,sizeof(_finddata));
    }
    _memset(&_finddata,0,sizeof(_finddata));

	//Change directory to one level up
    if((RetValue = Chdir(FindFile))<0)
	    return NOERROR;

	//Recursively called 
    if((RetValue = TestFindNext()) <0)
		return NOERROR;

	return NOERROR;
}

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
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
int  TestFileread()
{
    INT wRefPattern1 = 0x020100, wRefPattern2 = 0x030201, wRefPattern3 = 0x040302;
    INT wTestPattern, i, j;
    LONG Currentoffset=0;
    
    _packed BYTE bTitle[] = "Test Read File";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    // Open file
    if ((fin = Fopen(testread,(_packed char *)"r"))<0)
        return  ERROR;
       
    if(fin >0)
    {
          // Complete do loop reads and compares 256 bytes
        i = 2;
        do
        {        

            j = 42;
            wTestPattern = 0x020100;
            do
            {        
                RetValue = Fread(fin,Buffer,3,DEFAULT_MEMORY);
            
                if(RetValue != ERROR)
                {
                    if(Buffer[0] != wTestPattern)
                    {
                        RetValue = 1;
                        break;
                    }
                    else
                        wTestPattern = wTestPattern + 0x030303;       // Calculate new test pattern
                }
                else
                    break;
            }while(--j);
    
            if(RetValue != ERROR)
                // Read 2 more bytes
                RetValue = Fread(fin,Buffer,2,DEFAULT_MEMORY); 
            else
                break;               

        }while(--i);
        
        // Test sector boundaries (position 512)
        if(RetValue != ERROR)
        {
            // Read 6 bytes starting at byte 508
            Fseek(fin,0, SEEK_SET);     // Seek to beginning of file
            Fseek(fin,(DWORD)508, SEEK_CUR);
            if((RetValue = Fread(fin,Buffer,6,DEFAULT_MEMORY))<0)
                return ERROR; 
            if((Buffer[0] != 0x7E7D7C) || (Buffer[1] != 0x01007F))
                RetValue = ERROR;
        }                

        if(RetValue != ERROR)
        {
            // Read 3 bytes starting at 512            
            Fseek(fin, (DWORD)512, SEEK_SET);
            if((RetValue = Fread(fin,Buffer,3,DEFAULT_MEMORY))<0)
                return ERROR; 
            
            if(Buffer[0] != 0x020100)
                RetValue = ERROR;
        }                
        
        if(RetValue != ERROR)
        {
            // Read 3 bytes startint at 511
            Currentoffset=Ftell(fin);
            Fseek(fin,-4, SEEK_CUR);
            Currentoffset=Ftell(fin);            
            if((RetValue = Fread(fin,Buffer,3,DEFAULT_MEMORY))<0)
                return ERROR; 
            Currentoffset=Ftell(fin);            
            if(Buffer[0] != 0x01007f)
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
//   Description:    Test the read mode for function Fopenw which Supports the long file name.                   
//   Inputs:         none 
//
//   Outputs:        none
//                   
//   Notes:          none
//<
//////////////////////////////////////////////////////////////////////////////////
int  TestFopenwRead()
{    

    _packed BYTE bTitle[] = "Test Fopenw R";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    /* Open for read (will fail if file  does not exist) */
    if ((foutw = Fopenw((_packed char*)buf1,(_packed char *)"r"))<0)
        return  ERROR;
   
    if(Fread(foutw,ReadBuffer,56,DEFAULT_MEMORY)<0)
    {
        Fclose(foutw);
        return ERROR;
    }              
    /*Open the file which is exist in the read mode and try to do Fwrite if Fails*/   
    if(Fwrite(foutw,BitBuffer,25,DEFAULT_MEMORY)>0)
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
//   Description:    Test the write mode for function Fopenw which Supports the long file name.                   
//
//   Inputs:         none 
//
//   Outputs:        none
//                   
//   Notes:          none
//<
//////////////////////////////////////////////////////////////////////////////////
int  TestFopenwWrite()
{    
    INT fin,BytesToRead;
    LONG Filesize,NumByteToWrite=0,NumBytesToRead;
    int BytesToWrite=512;

    _packed BYTE bTitle[] = "Test Fopenw W";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    
    /* Open for write mode (will create the  file if  file  does not exist) */
    if ((foutw = Fopenw((_packed char*)buf,(_packed char *)"w"))<0)
        return  ERROR;
           
    /* Open for Read mode for filling Readbuffer*/  
    if ((fin=Fopenw((_packed char*)buf1,(_packed char *)"r"))<0)
        return ERROR;
           
    /*Seek to the End of file to get file size*/
    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;

    /* try to write  the whole file which is open in write mode*/   
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
       
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
            if((RetValue =Fwrite(foutw,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }     
    }
    FlushCache();
    Fclose(fin);
    Fclose(foutw);
    return  NOERROR;
}
//////////////////////////////////////////////////////////////////////////////////
////
////>  Name:           TestFopenwAppend
////
////   Type:           Function
////
////   Description:    Test the append mode for function Fopen                   
////   Inputs:         none 
////
////   Outputs:        none
////                   
////   Notes:          none
////<
//////////////////////////////////////////////////////////////////////////////////
int  TestFopenwAppend()
{
    INT fin,BytesToRead;
    LONG Filesize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed BYTE bTitle[] = "Test Fopenw A";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

//    
//      /* Open for append mode (will create the  file if  file  does not exist) */

    if ((foutw = Fopenw((_packed char*)buf,(_packed char *)"a"))<0)
        return  ERROR;

    if ((fin=Fopen(testfile3,(_packed char *)"r"))<0)
        return ERROR;
        
    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
           
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
       
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
            if((RetValue =Fwrite(foutw,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }     
    }
    FlushCache();
    Fclose(foutw);
    Fclose(fin);
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
//   Notes:          In this mode we can read as well write in the file
//<
////////////////////////////////////////////////////////////////////////////////
int  TestFopenwReadPlus()
{
    INT fin,BytesToRead;
    LONG Currentoffset=0,NumBytesToRead,Filesize,NumByteToWrite=0;
    int BytesToWrite=512;
    _packed BYTE bTitle[] = "Test Fopenw Plus";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    /*open for read plus mode(will fail if file  does not exist)*/
    if ((fout=Fopenw((_packed char*)buf,(_packed char *)"r+"))<0)
        return ERROR;

    if ((fin=Fopen(readfile,(_packed char *)"r"))<0)
        return ERROR;

    if((RetValue=Fseek(fin,0,SEEK_END))<0)
        return ERROR;
    Filesize=Ftell(fin);
    
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    if((RetValue=Fseek(fout,0,SEEK_END))<0)
        return ERROR;
        
    /* try to write  the whole file which is open in write mode*/   
    NumBytesToRead =Filesize;
    BytesToRead =0;
    while(NumByteToWrite < Filesize)
    {
        if((NumBytesToRead)>0)
        {
            if(NumBytesToRead> BytesToRead )
                BytesToRead=BytesToWrite;
            else
                BytesToRead =NumBytesToRead;
       
            if((RetValue = Fread(fin,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
            Currentoffset=Ftell(fout);
            if((RetValue =Fwrite(fout,ReadBuffer,BytesToRead,DEFAULT_MEMORY))<0)
                return ERROR;
        NumByteToWrite+=RetValue;
        }
        NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==Filesize))
        {
            NumByteToWrite = Filesize+1;
        }     
    }
     FlushCache();
     Fclose(fin);
     Fclose(fout);
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
//                   if the file is not present the it creates the file
//<
////////////////////////////////////////////////////////////////////////////////
int   TestFopenwWritePlus()
{

    _packed BYTE bTitle[] = "Test Fopenw Wplus";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
     

    if ((fout=Fopenw((_packed char*)buf2,(_packed char *)"w+"))<0)
        return ERROR;
    if((RetValue=Fseek(fout,0,SEEK_END))<0)
        return ERROR;
        
    if((RetValue =Fwrite(fout,BitBuffer,100,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    if((RetValue=Fseek(fout,-100,SEEK_CUR))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    if((RetValue = Fread(fout,ReadBuffer,100,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    
    if(BitBuffer[0]!= ReadBuffer[0])
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
//   Description:    Test the writeplus mode for function Fopen                   
//   Inputs:         none 
//
//   Outputs:        none
//                   
//   Notes:          In this mode we can read as well write in the file
//                   if the file is not present the it creates the file
//<
////////////////////////////////////////////////////////////////////////////////

int   TestFopenwAppendPlus()
{

    _packed BYTE bTitle[] = "Test Fopenw APlus";
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
      
    /*open file in appendplus mode (will create thes file if not present)*/
    if ((fout=Fopenw((_packed char*)buf1,(_packed char *)"a+"))<0)
        return ERROR;
    /*Write to the file it should erite at the end of the file*/        
    if((RetValue =Fwrite(fout,BitBuffer,150,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /* seek back the bytes write*/
    if((RetValue =Fseek(fout,-150,SEEK_CUR))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    /*Try to read bytes which we had write*/
    if((RetValue = Fread(fout,ReadBuffer,150,DEFAULT_MEMORY))<0)
    {
        Fclose(fout);
        return ERROR;
    }
    if(BitBuffer[0]!= ReadBuffer[0])
        return ERROR;
    
    FlushCache();   
    Fclose(fout);
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
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////

int  TestWriteFile()
{
    INT iRetVal, iBytesToWrite, i, j,Currentposotion=0;
    LONG lError;
    BYTE bHandleSource, bHandleDest, bEndCopyFlag=0;
    WORD wProgBarPosX = 0, wUpdateDisplay, wUpdateTicks;
    
    _packed BYTE bTitle[] = "Test Write File";
     
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);
    
    /* Fill the buffers with patterns*/
    for(i=0;i<NUM_WRITE_WORDS;i++)
    {
        pwFileBuffer[i] = 0xAAAAAA;
        pwCompareBuffer[i] = 0x555555;
    }

    if((fout = Fopen(bFileDest,(_packed char *)"w")) <0)
         return ERROR;

     FlushCache();
    /* Write the initial pattern to the file*/
    for(i=0;i<100;i++)
    {
//     if(i==100)
//     {
//        i++ ;
//        i--;
//     }
        RetValue =Fwrite(fout,pwFileBuffer,NUM_WRITE_BYTES,DEFAULT_MEMORY);
        
        if((RetValue <0) || (RetValue != NUM_WRITE_BYTES))
        {        
            /* Error writting - Close files*/
            Fclose(fout);
            FlushCache();
            return ERROR;
        }

    }
    
    if(Fseek(fout,0,SEEK_SET)<0)
         return ERROR;

    
    /* Overwrite the original pattern for every other chunk of NUM_WRITE_BYTES*/
    for(i=0;i<100/2;i++)
    {
        RetValue =Fwrite(fout,pwCompareBuffer,NUM_WRITE_BYTES,DEFAULT_MEMORY);
       
        if((RetValue <0) || (RetValue != NUM_WRITE_BYTES))
        {        
            /* Error writting - Close files*/
            Fclose(fout);
            return ERROR;
        }
        if(Fseek(fout,NUM_WRITE_BYTES,SEEK_CUR)<0)
        {        
            /* Error seeking - Close files*/
            Fclose(fout);
            return ERROR;
        }
    }

    /* We should be at the end of the file*/
    if(Feof(fout)> 0)
    {
        /* Error seeking - Close files*/
            Fclose(fout);
            return ERROR;
    }
    
    /* Close the file then open it again in READ mode*/
    FlushCache(); 
    Fclose(fout);

    if((fin = Fopen(bFileSource,(_packed char *)"r")) <0)
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
        RetValue = Fread(fin,pwFileBuffer,NUM_WRITE_BYTES,DEFAULT_MEMORY);
        
        if((RetValue <0) ||(RetValue != NUM_WRITE_BYTES))
        {        
            /* Error reading - Close files*/
            Fclose(fin);
            count1++;
            return ERROR;
        }
        Currentposotion=Ftell(fin);
        /* Skip past a chunk  */
        if(Fseek(fin,NUM_WRITE_BYTES, SEEK_CUR) == ERROR)
        {        
            /* Error seeking - Close files*/
            Fclose(fin);
            count1++;
            return ERROR;
        }
        Currentposotion=Ftell(fin);
        for(j=0;j<NUM_WRITE_WORDS;j++)
        {
            if(pwFileBuffer[j] != 0x555555)
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
    if(Feof(fin)>0)
    {
        /* Error seeking - Close files  */
        Fclose(fin);
        return ERROR;
    }
    /* Rewind the file */
    if(Fseek(fin,0, SEEK_SET)<0)
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
        pwFileBuffer[NUM_WRITE_WORDS-1] = 0xAAAAAA;

        /* Skip past the overwritten pattern */
        if(Fseek(fin, NUM_WRITE_BYTES, SEEK_CUR) == ERROR)
        {        
            /* Error seeking - Close files  */
            Fclose(fin);
            return ERROR;
        }
        /* Read the original data */
        RetValue = Fread(fin,pwFileBuffer,NUM_WRITE_BYTES,DEFAULT_MEMORY);
        
        if((RetValue<0) || (RetValue != NUM_WRITE_BYTES))
        {        
            /* Error reading - Close files */
            Fclose(fin);
            return ERROR;
        }
        for(j=0;j<(NUM_WRITE_WORDS-1);j++)
        {
            if(pwFileBuffer[j] != 0xAAAAAA)
            {
                /* Error reading - Close files*/
                Fclose(fin);
                return ERROR;
            }
        }
    }    
    FlushCache();
    Fclose(fin);
    Fclose(fout);
    return NOERROR;            
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFwritePMemory

//   Type:           Function
//
//   Description:    Test the file write and read using P MEMORY.

//   Inputs:         none 
//
//   Outputs:        none
//                   
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
int  TestFwritePMemory()
{
    INT fin,BytesToRead;
    LONG FileSize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=128;
    
    /* Open the file in read mode*/        
    if ((fin = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
    /* Open the file for write in write mode*/            
    if ((fout = Fopen(writefile2,(_packed char *)"w"))<0)
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

            if((RetValue = Fread(fin,(void *)PBuffer,BytesToRead,P_MEMORY))<0)
            {
                Fclose(fin);
                return ERROR;
            }

            if((RetValue =Fwrite(fout,(void *)PBuffer,BytesToRead,P_MEMORY))<0)
            {
                Fclose(fout);
                return ERROR;
            }
        NumByteToWrite+=RetValue;
        }
    NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==FileSize))
        {
            NumByteToWrite = FileSize+1;
        }     
    }
    FlushCache();
    Fclose(fin);
    Fclose(fout);
    return NOERROR;
}
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestFwriteXMemory

//   Type:           Function
//
//   Description:    Test the file Read and Write using X MEMORY.

//   Inputs:         none 
//
//   Outputs:        none
//                   
//   Notes:          none
//<
////////////////////////////////////////////////////////////////////////////////
int  TestFwriteXMemory()
{
    INT fin,BytesToRead;
    LONG FileSize,NumBytesToRead,NumByteToWrite=0;
    int BytesToWrite=128;
    
    /* Open the file for read*/    
    if ((fin = Fopen(testget,(_packed char *)"r"))<0)
        return  ERROR;
    /* Open the file for write in append mode*/    
    if ((fout = Fopen(writefile2,(_packed char *)"a"))<0)
        return  ERROR;
    /*Get the file size    */
    Fseek(fin,0,SEEK_END);
    FileSize=Ftell(fin);
    if((RetValue=Fseek(fin,0,SEEK_SET))<0)
        return ERROR;
    
    
    /*Test the Fwrite by writing whole file*/
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

            if((RetValue = Fread(fin,(void *)XBuffer,BytesToRead,X_MEMORY))<0)
            {
                Fclose(fin);
                return ERROR;
            }

            if((RetValue =Fwrite(fout,(void *)XBuffer,BytesToRead,X_MEMORY))<0)
            {
                Fclose(fout);
                return ERROR;
            }
        NumByteToWrite+=RetValue;
        }
    NumBytesToRead-=BytesToWrite ;
        if((RetValue==0) ||(NumByteToWrite==FileSize))
        {
            NumByteToWrite = FileSize+1;
        }     
    }
    FlushCache();
    Fclose(fin);
    Fclose(fout);
    return NOERROR;
}
    


void  FS_SwizzleText(_packed BYTE *pText)
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
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           TestMkdirMax

//   Type:           Function
//
//   Description:    Test the Creation of maximun number of Directory
 
//   Inputs:         none 

//   Outputs:        none
//                   
//   Notes:          Maxiumum Root Directory Possible -> FAT 12 Or FAT16 = 256
//                                                       FAT 32  = No Limit
//                   //<
////////////////////////////////////////////////////////////////////////////////
int TestMkdirMax(void)
{
    int i,Byte=0,j,m,k,l=0;
    _packed BYTE bTitle[] = "Test Make DIR Max"; 
    /* Display Test Title*/
    LcdClear(0, 1*8, LCD_X_PIXELS, LCD_Y_PIXELS-(1*8));       // Clear LCD
    FS_SwizzleText(bTitle);
    PrintStringAddr(10, 3*8, bTitle);

    if((RetValue=Mkdir(rootdir))<0)
        return  ERROR;
    FlushCache();

    PutByte(DirectoryName,'a',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,':',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'\\',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'T',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'e',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'s',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'t',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'d',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'i',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'r',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'1',l++,DEFAULT_MEMORY);
    PutByte(DirectoryName,'\\',l++,DEFAULT_MEMORY);
   
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
                    FlushCache(); 
                    return NOERROR;
                }
             }   
	    }
   }  
    FlushCache();
    return NOERROR;
}                                                  



void GetUnicodeString(_packed char* filepath,_packed char* buf,INT Strlen)
{
   INT offset=0,word=0,i;
   CHAR Byte;

  for(i=0;i<=Strlen;i++)
   {
   Byte = FSGetByte(filepath,i,DEFAULT_MEMORY);
   PutByte(&word,Byte,0,DEFAULT_MEMORY);
   PutWord(buf,word,offset,DEFAULT_MEMORY); 
   offset+=2; 
   } 

 }





#pragma asm
	nolist
    include "resource.inc"
    include "sysequ.inc"
    include "sysresources.inc"
    list

	global  FSetIrqL,FGetIrqL
    global  SignalModule
    global  FMediaWrite, FMediaRead,FileSystemMode,FSDataDriveInit
    global  MyDebug

    extern  SysLoadResource

SIZEWRITEMODULEY    equ     $1fff
SIZEWRITEMODULEP    equ     $6000

    org     p,"FATTest_P":
SignalModule
FSetIrqL
FGetIrqL
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

MyDebug
    debug
    rts
    
    
    org y,".yFATTEST":
FileSystemMode      dc      1    
    
#pragma endasm 





