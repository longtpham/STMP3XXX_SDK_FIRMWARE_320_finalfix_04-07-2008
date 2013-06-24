#ifndef _FSAPI_H_
#define _FSAPI_H_
#include "filespec.h"
extern _reentrant LONG Ftell(INT HandleNumber);
extern _reentrant LONG Feof(INT HandleNumber);
extern _reentrant INT *Fputs(INT HandleNumber,INT *Buffer);
extern _reentrant LONG Fread(INT HandleNumber,INT *Buffer,LONG NumBytesToRead,INT Source_Memory,INT MOdulo);
extern _reentrant INT Fgetc(INT HandleNumber);
extern _reentrant INT Fputc(INT HandleNumber,INT ByteToWrite);
extern _reentrant LONG ReadDirectoryRecord(INT HandleNumber,INT RecordNumber,INT *Buffer);
extern _reentrant INT Fseek(INT HandleNumber,LONG NumBytesToSeek,INT SeekPosition);
extern _reentrant INT Fopen(_packed char *filepath,_packed char *mode);
extern _reentrant LONG Fwrite(INT HandleNumber,INT  *Buffer,LONG NumBytesToWrite,INT Source_Memory,INT MOdulo);
extern _reentrant LONG Totalfreecluster(INT DeviceNum);
extern _reentrant INT Rmdir(_packed char *filepath);
extern _reentrant INT Rmdirw(_packed char *filepath);
extern _reentrant INT Mkdir(_packed char *filepath);

	//	SGTL-HK 27-05-2005
extern _reentrant INT Mkdirw(UCS3 *filepath);

extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);

extern _reentrant INT Setcwd(_packed char *filepath, _packed char *gCworkingDir,INT index,INT length);
extern _reentrant _packed char *Getcwd(void);
extern  _reentrant _packed char *Fgets(INT HandleNumber,INT NumBytesToRead, _packed char *Buffer);
extern INT  FSInit(INT _X *bufx, INT _Y *bufy, INT maxdevices, INT maxhandles, INT maxcaches);
extern INT  FlushCache(void);
extern _reentrant INT FsShutDown(void);
extern _reentrant LONG GetFileSize(INT HandleNumber);

extern _reentrant INT filegetattrib(_packed char *FilePath);
extern _reentrant INT Fopenw(INT *filepath,_packed char *mode);
extern _reentrant INT Fremove(_packed char *filepath);
extern _reentrant INT Fremovew(_packed char *filepath);
extern _reentrant void DBCSToUnicode(_packed unsigned char *pDBCS, WORD *pUnicode,INT iLength);
extern INT FlushCache(void);
extern _reentrant INT DeleteTree(_packed char *Path);
extern _reentrant INT Fclose(INT HandleNumber);
extern INT FSMediaPresent(INT DeviceNum);
extern INT FSFATType (INT DeviceNum);
extern  INT _reentrant  GetVolumeLabel(_packed char *Buffer,INT DeviceNum);
extern _reentrant INT SetVolumeLabel(_packed char *Buffer,INT DeviceNum);
extern _reentrant LONG FgetFastHandle(INT HandleNumber);

extern _reentrant INT ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern _reentrant void Uppercase(_packed char *file); 
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _reentrant void ClearData(Finddata *_finddata);
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);




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


typedef struct
{
LONG CurrentOffset;     
LONG CurrentCluster;
}HANDLECONTEXT;

extern _reentrant INT filesetattrib(INT HandleNumber,INT dirattribute);
extern _reentrant INT filesetdate(_packed char *FilePath,INT crt_mod_date_time_para,DIR_DATE *dirdate,DIR_TIME *dirtime);
extern _reentrant INT filegetdate(INT HandleNumber,INT crt_mod_date_time_para,DIR_DATE *dirdate,DIR_TIME *dirtime);
#endif