


#define X_MEMORY	0
#define Y_MEMORY	2
#define P_MEMORY	4
#define SC_MEMORY   X_MEMORY

#define DEFAULT_MEMORY Y_MEMORY

//Its for data  of various structures its in Y Memory can be moved  to P Memory
// right now onlt secstore.c is changed for this
#define TFR_MEMORY  _X
//Its for constans used for various purposes can be moved to P Memory
#define CONST_MEMORY   _X
//Its for drmstrings can be moved to P Memory
#define CHANGE_MEM  _X
//Its for buffer which are not used often in the current code 
//It should be placed in the P memory right now for sim its placed in Y Memory
#define P_MEM   _X

/* for good parameter documentation */
#define IN
#define OUT


#define SEC_RGBSLOTDATA_MEM X_MEMORY
#define HEADERDATA_OFFSET 0

//#define DRM_SUPPORT_DLA                   1
#define DRM_SUPPORT_SECURE_CLOCK          1
#define DRM_SUPPORT_LICENSE_SYNC          1
#define DRM_SUPPORT_METERING              1
#define DRM_SUPPORT_ENCRYPTION            1
#define DRM_SUPPORT_ANTIROLLBACK_CLOCK    1
#define DRM_SUPPORT_SYMMETRIC_OPTIMIZATIONS 1
#define IGNORE_OUTPUT_RESTRICTIONS		1
#ifdef MTP_BUILD

#define DRM_SUPPORT_CERTIFICATE_CACHING 1
#endif
/* Would be good to have a test for platform.  If the platform isn't Windows
	we should define __cdecl to be nothing.  We can't just use _MSC_VER though
	as there are other compilers for the Windows platforms. */
#if defined (_MSC_VER)
#ifdef __STDC__ /* when compiling as ANSI, parameter-passing specifications aren't allowed */
#define DRM_API 
#else
#define DRM_API __cdecl    
#endif
#elif defined (__GNUC__)
#define DRM_API 
#elif defined (__TMS320C55X__)
#define SIXTEEN_BIT_ADDRESSING 1
#define DRM_NO_NATIVE_64_TYPES 1
#define DRM_API 
#else
#define TWENTYFOR_BIT_ADDRESSING 1
//#error Unknown compiler -- fix up drmcommon.h to understand this compiler.
#define DRM_API    
#endif  


/* 
* In General, 
* DRM_CHAR* is used for null terminated ANSI strings
* DRM_BYTE* is used for just plain byte buffer, w/o any termination
*/

#if defined(_M_IX86)
#define TARGET_LITTLE_ENDIAN 1
#elif defined( _M_AMD64 )
#define TARGET_LITTLE_ENDIAN 1
#elif defined( _M_IA64 )
#define TARGET_LITTLE_ENDIAN 1
#elif defined( ARM )
#define TARGET_LITTLE_ENDIAN 1
#elif defined(__TMS320C55X__)
//#define DRM_USE_PORTABLE_WIDE_STRINGS 1
#define TARGET_LITTLE_ENDIAN 0
#define SIXTEEN_BIT_ADDRESSING 1
#else
#define TARGET_LITTLE_ENDIAN 1
#endif
/* _M_IA64 _M_AMD64 */

                                                                        /* 8-bit addressing    16 bit addressing*/
typedef void                    DRM_VOID;
typedef unsigned char           DRM_BYTE;                               /* 1 byte              1 byte */
typedef char                    DRM_CHAR;                               /* 1 byte              1 byte */
typedef unsigned short          DRM_WORD, DRM_WCHAR;                    /* 2 bytes             1 byte */
typedef short                   DRM_SHORT;                              /* 2 bytes             1 byte */
#if SIXTEEN_BIT_ADDRESSING
typedef unsigned int            DRM_BOOL;                               /* 1 bytes             1 bytes */
typedef unsigned long           DRM_UINT;                               /* 4 bytes             2 bytes */
typedef long                    DRM_INT;                                /* 4 bytes             2 bytes */
typedef long                    DRM_LONG;                               /* 4 bytes             2 bytes */
typedef unsigned long           DRM_RESULT;                             /* 4 bytes             2 bytes */
#else
typedef unsigned int            DRM_BOOL,DRM_UINT;                      /* 4 bytes             2 bytes */
typedef int                     DRM_INT,DRM_RESULT;                                /* 4 bytes             2 bytes */
typedef long                    DRM_LONG;                   /* 4 bytes             2 bytes */
#endif
typedef unsigned long           DRM_DWORD, DRM_ULONG;                   /* 2 bytes             1 bytes */
typedef unsigned short          DRM_LANGID;

#define SIZEOF_DRM_DWORD	    4
#define SIZEOF_DRM_WORD		    2
#define SIZEOF_DRM_WCHAR	    2
#define SIZEOF_DRM_UINT		    4
#define	SIZEOF_DRM_BYTE		    1
#define SIZEOF_TOKEN		    12
#define SIZEOF_DRM_UINT64	    8
#define SIZEOF_DRM_CHAR		    1
#define SIZEOF_DRM_DWORD_PTR    SIZEOF_DRM_DWORD
#define SIZEOF_NFREEMEM         SIZEOF_DRM_DWORD
#define SIZEOF_DIGIT_T          SIZEOF_DRM_DWORD
#define SIZEOF_RANDOM_SEED      SIZEOF_DRM_UINT64
 

#define SIZEOF_DRM_LICENSE_STATE_DATA  (SIZEOF_DRM_DWORD + SIZEOF_DRM_DWORD \
                                     + SIZEOF_DRM_DWORD + 4*SIZEOF_DRM_DWORD \
                                     + SIZEOF_DRM_DWORD + 2*SIZEOF_DRM_DWORD*4 +SIZEOF_DRM_DWORD)
 
#define TYPE_DWORD	            4
#define TYPE_WORD	            2
#define TYPE_BYTE	            1

#define DRM_SUCCESS ((DRM_RESULT)0x000000L)
#define DRM_E_FAIL				((DRM_RESULT)0x804005L)
#define DRM_E_BUFFERTOOSMALL	((DRM_RESULT)0x87007AL)
#define DRM_E_LICRESPONSEEXCEEDSSIZELIMIT     (DRM_RESULT)0x84ffffL
#define DRM_E_METER_RESPONSEEXCEEDSSIZELIMIT     (DRM_RESULT)0x84fffeL
#define DRM_E_METERCHALLENGEQUERY_EXCEEDS     (DRM_RESULT)0x84fffdL
#define DRM_E_NOMORE			((DRM_RESULT)0x870103L) /* End of enum, or no more data available */
#define DRM_E_NOMORE_EXTENDED   (0x80070103) /* End of enum, or no more data available */


#if SIXTEEN_BIT_ADDRESSING
#define CB_NATIVE_BYTE 2
#define __CB_DECL(x) ((x)/CB_NATIVE_BYTE+(((x)%CB_NATIVE_BYTE)?1:0))
#else
#define CB_NATIVE_BYTE 1
#define __CB_DECL(x) (x)
#endif

#ifndef SIZEOF
#define SIZEOF(x) (sizeof(x)*CB_NATIVE_BYTE)
#endif

#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE (CB_NATIVE_BYTE*8)
#endif

typedef struct __tagDRM_GUID {
    DRM_DWORD Data1;
    DRM_WORD  Data2;
    DRM_WORD  Data3;
    DRM_BYTE  Data4 [__CB_DECL(8)];
} DRM_GUID;

typedef struct tagDRM_CONST_STRING
{
    const DRM_WCHAR *pwszString;
    DRM_DWORD        cchString;
} DRM_CONST_STRING;

typedef struct tagDRM_CONST_STRING_MEM
{
    const DRM_WCHAR CHANGE_MEM *pwszString;
    DRM_DWORD       CHANGE_MEM cchString;
} CHANGE_MEM DRM_CONST_STRING_MEM;

typedef struct tagDRM_PACKED_STRING
{
    const DRM_WCHAR CHANGE_MEM *pwszString;
    DRM_DWORD       CHANGE_MEM cchString;
    DRM_DWORD       CHANGE_MEM Offset;
} CHANGE_MEM DRM_PACKED_STRING;


/*
**
*/
typedef struct tagDRM_STRING
{
    DRM_WCHAR *pwszString;
    DRM_DWORD  cchString;
} DRM_STRING;

typedef struct tagDRM_STRING_MEM
{
    DRM_WCHAR CHANGE_MEM *pwszString;
    DRM_DWORD CHANGE_MEM  cchString;
} CHANGE_MEM  DRM_STRING_MEM;
typedef struct tagDRM_ANSI_CONST_STRING
{
    const DRM_CHAR *pszString;
    DRM_DWORD       cchString;
} DRM_ANSI_CONST_STRING;

typedef struct tagDRM_ANSI_CONST_STRING_MEM
{
    const DRM_CHAR CHANGE_MEM *pszString;
    DRM_DWORD       cchString;
} DRM_ANSI_CONST_STRING_MEM;

typedef struct tagDRM_ANSI_STRING
{
    DRM_CHAR  *pszString;
    DRM_DWORD  cchString;
} DRM_ANSI_STRING;

typedef struct __tagSubString
{
    DRM_DWORD m_ich;
    DRM_DWORD m_cch;
} DRM_SUBSTRING;


