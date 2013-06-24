////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: types.h
// Description: Standard data types
////////////////////////////////////////////////////////////////////////////////

#ifndef _TYPES_H
#define _TYPES_H

// TODO:  move this outta here!
#if !defined(NOERROR)
#define NOERROR 0
#define SUCCESS 0
#endif 
#if !defined(SUCCESS)
#define SUCCESS  0
#endif
#if !defined(ERROR)
#define ERROR   -1
#endif
#if !defined(FALSE)
#define FALSE 0
#endif
#if !defined(TRUE)
#define TRUE  1
#endif

#if !defined(NULL)
#define NULL 0
#endif

#define MAX_INT     0x7FFFFF
#define MAX_LONG    0x7FFFFFffffff
        // max positive unsigned long value: -1 == 0xFFFFFFffffff
#define MAX_ULONG   (-1) 

#define WORD_SIZE   24              // word size in bits
#define BYTES_PER_WORD 3            // 8 bit bytes


#define BYTE    unsigned char       // btVarName
#define CHAR    signed char         // cVarName
#define USHORT  unsigned short      // usVarName
#define SHORT   unsigned short      // sVarName
#define WORD    unsigned int        // wVarName
#define INT     signed int          // iVarName
#define DWORD   unsigned long       // dwVarName
#define LONG    signed long         // lVarName
#define BOOL    unsigned int        // bVarName
#define FRACT   _fract              // frVarName
#define LFRACT  long _fract         // lfrVarName
#define CIRC    _circ               // cp<type prefix>VarName (see below)
#define FLOAT   float               // fVarName
#define DBL     double              // dVarName
#define ENUM    enum                // eVarName
#define CMX     _complex            // cmxVarName
typedef WORD UCS3;                   // 

#define UINT16  unsigned short
#define UINT8   unsigned char	
#define UINT32  unsigned long
#define UINT64  unsigned long	// need to figure out how to do 64 bit values.  Or port exisiting code to only use 48 bit
#define INT64   unsigned long	// need to figure out how to do 64 bit values.  Or port exisiting code to only use 48 bit
#define WCHAR   UINT16

//UINT128 is 16 bytes or 6 words
typedef struct UINT128_3500 {   
    int val[6];     
} UINT128_3500;

#define UINT128   UINT128_3500

// Little endian word packed byte strings:   
//      _packed BYTE bTextVarName[]="a string";  bText is a packed byte string prefix
// Convert to big endian words by reordering bytes from high to low using swizzle.
// Little endian word packed byte strings:   
//      _packed BYTE bTextVarName[]="a string";  bText is a packed byte string prefix
// Convert to big endian words by reordering bytes from high to low using swizzle.

// Declare Memory Spaces To Use When Coding
// A. Sector Buffers
#define SECTOR_BUFFER_MEM_X _X
#define SECTOR_BUFFER_MEM_Y _Y
#define SECTOR_BUFFER_MEM SECTOR_BUFFER_MEM_X
typedef WORD SECTOR_BUFFER_MEM SECTOR_BUFFER;
typedef SECTOR_BUFFER * P_SECTOR_BUFFER;
// B. Media DDI Memory
#define MEDIA_DDI_MEM _Y



// Pointers to linear structures are labeled as: p<type prefix>VarName
// Examples of circular pointers:
//    INT CIRC cpiVarName
//    DWORD CIRC cpdwVarName

#define RETCODE INT                 // rcVarName

// generic bitfield structure
struct Bitfield {
    unsigned int B0  :1;
    unsigned int B1  :1;
    unsigned int B2  :1;
    unsigned int B3  :1;
    unsigned int B4  :1;
    unsigned int B5  :1;
    unsigned int B6  :1;
    unsigned int B7  :1;
    unsigned int B8  :1;
    unsigned int B9  :1;
    unsigned int B10 :1;
    unsigned int B11 :1;
    unsigned int B12 :1;
    unsigned int B13 :1;
    unsigned int B14 :1;
    unsigned int B15 :1;
    unsigned int B16 :1;
    unsigned int B17 :1;
    unsigned int B18 :1;
    unsigned int B19 :1;
    unsigned int B20 :1;
    unsigned int B21 :1;
    unsigned int B22 :1;
    unsigned int B23 :1;
};

union BitInt {
	struct Bitfield B;
	int	   I;
};

#define MAX_MSG_LENGTH 10
struct CMessage
{
	unsigned int m_uLength;
	unsigned int m_uMsg[MAX_MSG_LENGTH];
};

typedef struct {
    WORD m_wLength;
    WORD m_wMessage;
    WORD m_wParams[MAX_MSG_LENGTH];
} Message;

struct MessageQueueDescriptor
{
	int *m_pBase;
	int m_iModulo;
	int m_iSize;
	int *m_pHead;
	int *m_pTail;
};

struct ModuleEntry
{
    int m_iSignaledEventMask;
    int m_iWaitEventMask;
    int m_iResourceOfCode;
    struct MessageQueueDescriptor *m_pMessageQueue;
    int _asmfunc (*m_pProcessEntryPoint)(union WaitMask Signals,struct MessageQueueDescriptor *);
    int _asmfunc (*m_pInitFunction)(struct MessageQueueDescriptor *);
    int m_uTimeOutHigh;
    int m_uTimeOutLow;
    int _asmfunc (*m_pBackgroundFunction)(struct MessageQueueDescriptor *);
};

union WaitMask{
    struct B{
        unsigned int m_bNone     :1;
        unsigned int m_bMessage  :1;
        unsigned int m_bTimer    :1;
        unsigned int m_bButton   :1;
    } B;
    int I;
} ;


struct Button {
	WORD wButtonEvent;
	WORD wUnused[MAX_MSG_LENGTH];	
};

struct Message {
	WORD wMsgLength;
	WORD wMsgCommand;
	WORD wMsgParms[MAX_MSG_LENGTH-1];
};

union EventTypes {
	struct CMessage msg;
	struct Button Button ;
	struct Message Message;
};

// next 2 used to get updater build to return correct protocol ver in scsi response.
#define BUILD_TYPE_UPDATER  0x100
#define BUILD_TYPE_HOSTLINK 0x101
#define BUILD_TYPE_PLAYER   0x102

// Resets part if RETAIL build. DEBUG build executes debug instruction to halt.
#define SystemHalt() __asm(" include 'sysmacro.asm' \n error") 
#define assert(x)    if(!(x)) __asm(" include 'sysmacro.asm' \n error")

#if DEBUG
// For DEBUG builds, DebugBuildAssert(condition) halts if condition evaluates to FALSE. 
#define DebugBuildAssert(x)    if(!(x)) __asm(" debug ")
#else 
// For RETAIL builds, DebugBuildAssert(condition) does nothing & execution continues rather than halt or reset. 
#define DebugBuildAssert(x)    
#endif

// Do this in your C files to make a reminder that's displayed during the build.
//  #pragma asm
//  WARN 'Note to builder: reminder to do this after verifying that'
//  #pragma endasm


#ifdef COLOR_262K
#define RGB(r,g,b)    ((0x0000FC & (r)) << 10) | ((0x0000FC & (g)) << 4) | ((0x0000FC & (b)) >> 2)    //262k
#elif defined(COLOR_65K)
#define RGB(r,g,b)    ((0x0000F8 & (r)) << 8) | ((0x0000FC & (g)) << 3) | ((0x0000F8 & (b)) >> 3)  //65k
#else
#define RGB(r,g,b) 	 ((0x0000F0 & (r)) << 4) | (0x0000F0 & (g)) | ((0x0000F0 & (b)) >> 4)
#endif
    
#endif // #ifndef _TYPES_H
