#include "DirOffset.h"

typedef struct
{
INT	Dir_name[4];
INT	Dir_Attr;
INT	Dir_CrtTime;
INT	Dir_CrtDate;
INT	Dir_LstaccDate;
INT	Dir_FstclusHi;
INT	Dir_WrtTime;
INT	Dir_WrtDate;
INT	Dir_FstclusLo;
LONG	Dir_FileSize;
}DIR_RECORD;

typedef struct
{

INT	LDir_Ord;
INT	LDir_Name1[LDIRNAME1SIZE];
INT	LDir_Attribute;
INT	LDir_Type;
INT	LDir_Chksum;
INT	LDir_Name2[LDIRNAME2SIZE];
INT	LDir_FstcludNo;       
INT	LDir_Name3[LDIRNAME3SIZE];
}LFN_RECORD;


typedef struct
{

INT		Day;
INT		Month;
INT		Year;
}DIR_DATE;


typedef struct
{

INT		Second;
INT		Minute;
INT		Hour;
}DIR_TIME;
