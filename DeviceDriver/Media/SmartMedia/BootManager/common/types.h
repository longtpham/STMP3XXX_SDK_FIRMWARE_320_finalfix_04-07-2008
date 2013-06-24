////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
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
#define MAX_LONG    0x7FFFFFFFFFFF


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
typedef WORD UCS3;                  // 

// Pointers to linear structures are labeled as: p<type prefix>VarName
// Examples of circular pointers:
//    INT CIRC cpiVarName
//    DWORD CIRC cpdwVarName

#define RETCODE WORD                // rcVarName

// generic bitfield structure
struct Bitfield {
    int B0  :1;
    int B1  :1;
    int B2  :1;
    int B3  :1;
    int B4  :1;
    int B5  :1;
    int B6  :1;
    int B7  :1;
    int B8  :1;
    int B9  :1;
    int B10 :1;
    int B11 :1;
    int B12 :1;
    int B13 :1;
    int B14 :1;
    int B15 :1;
    int B16 :1;
    int B17 :1;
    int B18 :1;
    int B19 :1;
    int B20 :1;
    int B21 :1;
    int B22 :1;
    int B23 :1;
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
        int m_bNone     :1;
        int m_bMessage  :1;
        int m_bTimer    :1;
        int m_bButton   :1;
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




#define SystemHalt() __asm(" include 'sysmacro.asm' \n error")

    
#endif // #ifndef _TYPES_H
