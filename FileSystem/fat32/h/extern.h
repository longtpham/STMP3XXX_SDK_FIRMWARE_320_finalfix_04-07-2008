#include "handletable.h"
#include "devicetable.h"
#include "cachemem.h"
//#include "dirrecord.h"

extern INT maxhandles;
extern INT maxdevices;
extern INT maxcaches;
extern INT device[];
extern INT gCurrentRecord;
extern HANDLETABLE *Handle;
extern FSMEDIA_TABLE *MediaTable;
extern _packed char gCurrentWorkingdirectory[]; // use blank instead of 80 here. actually 256 multiple now
extern tCACHEDESCR *CacheDesc;
extern ss_FsCopyBuffer;

//Non-reentrant

	//	SGTL-HK 27-05-2005
extern _reentrant INT CreateDirectory(INT HandleNumber,_packed char *Buffer,INT length,INT index,INT stringtype);

extern _reentrant LONG Ftell(INT HandleNumber);//Non _reentrant 
extern _reentrant INT Fcreate(INT HandleNumber,_packed char *FileName,INT stringtype,INT length,INT index);
extern _reentrant LONG GetFileSize(INT HandleNumber);
extern _reentrant INT UpdateFileSize(INT HandleNumber,INT flag);
extern _reentrant INT GetNewcluster(INT Handlenumber);
extern _reentrant INT IsHandleWriteAllocated(INT HandleNumber);
extern _reentrant INT Searchfreehandleallocate(void);
extern _reentrant LONG FirstfreeAndallocate(INT DeviceNum);
extern _reentrant LONG FindnextFreecurrentcluster(INT DeviceNum,LONG clusterno);
extern _reentrant INT _X *FirstfreeAndallocateFAT32(INT DeviceNum,LONG FATsectrono,INT FAToffset ,INT _X *buf, LONG *ClusterNo);
extern _reentrant INT  WriteFATentry(INT DeviceNum,LONG FATsector,INT FATNtryoffset,LONG clusterno,LONG writentry);
extern _reentrant INT WriteSector(INT deviceNumber, LONG sectorNumber, INT Offset, INT *Sourcebuffer,INT SourceOffset,INT size,INT Source_Memory, INT SectorType,INT Modulo);
extern _reentrant INT FlushSector(INT deviceNumber, LONG sectorNumber,INT SectorType);
#ifdef k_opt_dynamic_cache
extern _reentrant INT _X *ReadSector(INT DeviceNum, LONG sectorNum,INT SectorType);
#else
extern INT _X *ReadSector(INT DeviceNum, LONG sectorNum,INT SectorType);
#endif
extern _reentrant INT EraseSector(INT deviceNumber, LONG sectorNumber);


//extern INT filesetattrib(INT HandleNumber,INT dirattribute); TOVERIFY2. 3.05 commented this ln out. 2.600 had is present.
//extern INT filesetdate(INT HandleNumber,INT crt_mod_date_time_para,DIR_DATE *dirdate,DIR_TIME *dirtime);
//Reentrant
//extern INT ReleaseBuffer(INT deviceNumber, LONG sectorNumber);
extern _reentrant LONG Firstsectorofcluster(INT DeviceNum,LONG clusterno);
extern _reentrant LONG ReadFATentry(INT Devicenum,LONG FATsector,INT FATNtryoffset,LONG clusterno);
extern _reentrant INT _X *ReadFAT12Entry(INT DeviceNum,INT *FATsectorNo,INT FATntryoffset,LONG clusterNum,INT _X *buf,INT *FATentry);
extern INT  FSInit(INT _X *bufx, INT _Y *bufy, INT maxdevices, INT maxhandles, INT maxcaches);

extern RETCODE _reentrant MediaInit(WORD wLogMediaNumber);
extern _reentrant INT Handleactive(INT HandleNumber);
extern INT DriveLetter[];
extern _reentrant LONG Findnextcluster(INT DeviceNum,LONG clusterno);

extern _reentrant INT UpdateHandleOffsets(INT HandleNumber);
extern _reentrant INT FindNextSector(INT Device,INT HandleNumber);
extern _reentrant INT Updatehandlemode(INT HandleNumber,INT Mode);

extern _reentrant LONG Feof(INT HandleNumber);
extern _reentrant INT *Fputs(INT HandleNumber,INT *Buffer);
extern _reentrant LONG Fread(INT HandleNumber,INT *Buffer,LONG NumBytesToRead,INT Source_Memory, INT modulo);
extern  _reentrant INT Fgetc(INT HandleNumber);
extern _reentrant INT Fputc(INT HandleNumber,INT ByteToWrite);
extern _reentrant INT Stringlength(INT *Buffer);
extern _reentrant LONG ReadDirectoryRecord(INT HandleNumber,INT RecordNumber,INT *Buffer);
extern _reentrant INT Fseek(INT HandleNumber,LONG NumBytesToSeek,INT SeekPosition);
extern _reentrant INT CreateDirRecord(_packed char *Buffer,INT HandleNumber,LONG ClusterNumber,INT DirAttr,INT stringtype,INT length,INT index,INT *unicodebuffer);
extern _reentrant INT FindfreeRecord(INT Handlenumber,INT count);
extern _reentrant LONG Firstrootdirsector(INT DeviceNum);
//extern _reentrant INT Searchfreerecord(INT DeviceNum ,LONG sectorNum);

extern _reentrant INT  Readdevicerecord(INT DeviceNum);
extern INT MediaRead(INT deviceNumber, LONG sectorNumber,int _X *readbuf,INT SectorType);
extern INT MediaWrite(INT deviceNumber,LONG sectorNumber, int _X *readbuf,INT SectorType);
extern _reentrant INT Shortdirmatch(INT HandleNumber,INT RecordNo,_packed char *shortname,_packed char *file,INT *buf,INT Flag,INT lenght,INT index,INT *Buffer);
extern _reentrant INT UpdateHandle(INT HandleNumber,LONG clusterno);
extern _reentrant void Uppercase(_packed char *file); 
extern _reentrant void ArrangeFileName(INT *Buffer,INT *ShortFileName);
extern _reentrant INT ClearCluster(INT HandleNumber);

extern _reentrant INT Fopen(_packed char *filepath,_packed char *mode);
extern _reentrant LONG FATsectorno(INT DeviceNum,LONG clusterno,INT *FATNtryoffset);

extern _reentrant LONG Fwrite(INT HandleNumber,INT  *Buffer,LONG NumBytesToWrite,INT Source_Memory,INT modulo);

extern _reentrant INT ExtractPath(_packed char *filepath,INT *index);
extern _reentrant INT ExtractPathW(_packed char *filepath,INT *index);
extern _reentrant INT SetHandleforsearch(INT HandleNumber,_packed char *filepath,INT stringtype,INT *index);
extern _reentrant INT Changepath(INT HandleNumber,_packed char *filepath,INT stringtype,INT startposition,INT index,INT RecordNumber);
extern _reentrant INT Longdirmatch(INT HandleNumber,INT RecordNo,_packed char *file,INT length,INT index,INT stringtype);
extern _reentrant INT Getname(_packed char *filepath, INT currentPosition);
extern _reentrant INT Strlength(_packed char *filepath);
extern _reentrant INT StrlengthW(_packed char *filepath);                        
extern _reentrant INT MatchdirRecordW(INT HandleNumber,INT RecordNo,_packed char *file,INT *buf,INT length,INT index);
extern _reentrant INT MatchdirRecord(INT HandleNumber,INT RecordNo,_packed char *file,INT *buf,INT Flag,INT lenght,INT index,INT *Buffer);
extern _reentrant LONG Searchdirectory(INT HandleNumber,_packed char *file,INT stringtype,INT Flag,INT lenght,INT index,INT *Buffer,BOOL *Ptr); // sdk3.05 ver at left; ,BOOL bInputIsSFN,BOOL *Ptr); extra end param in 2.600 ver. TOVERIFY2
extern _reentrant INT GetnameW(_packed char *filepath,INT currentPosition);
extern _reentrant INT Extractfilename(_packed char *filepath,INT length, INT *index);
extern _reentrant INT Changecase(INT wordno);
extern _reentrant INT ReadRootdirRecord(INT HandleNumber,INT RecordNumber,INT *Buffer);
extern _reentrant LONG Totalfreecluster(INT DeviceNum);

extern INT maxhandles;
extern INT maxdevices;

extern _reentrant INT Isfileopen(INT HandleNumber);
extern _reentrant INT Freehandle(INT HandleNumber);
extern _reentrant INT DeleteRecord(INT HandleNumber, INT RecordNo);
extern _reentrant INT Isdirectoryempty( INT Handlenumber);
extern _reentrant INT Strcpy(_packed char *filepath, _packed char *file_path1,INT length, INT index);
extern _reentrant INT Strcpyw(_packed char *filepath, _packed char *file_path1,INT length, INT index);
extern _reentrant void Setfilename(_packed char *buf, _packed char *buffer_1);
extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT Rmdir(_packed char *filepath);
extern _reentrant INT Rmdirw(_packed char *filepath);

extern _reentrant INT Mkdir(_packed char *filepath);

	//	SGTL-HK 27-05-2005
extern _reentrant INT Mkdirw(UCS3 *filepath);

extern _reentrant INT Setcwd(_packed char *filepath, _packed char *gCworkingDir,INT index,INT length);
extern _reentrant INT DeleteContent(INT HandleNumber, INT flag);

extern _reentrant _packed char *Getcwd(void);

extern _reentrant _packed char *Fgets(INT HandleNumber,INT NumBytes,_packed char *Buffer);
extern _reentrant INT Fopenw(INT  *filepath,_packed char *mode);
extern _reentrant INT SetcurrentPos(INT HandleNumber,INT RecordNumber);
extern _reentrant INT Fremove(_packed char *filepath);
extern _reentrant INT Fremovew(_packed char *filepath);

extern  _reentrant  INT ChangeToRootdirectory(INT HandleNumber);
                                                  
extern _reentrant void DBCSToUnicode(_packed unsigned char *pDBCS, WORD *pUnicode,INT iLength);
extern _reentrant LONG TotalfreeclusterFAT16(INT DeviceNum);
extern _reentrant Ferror(INT HandleNumber);
extern _reentrant INT Fclose(INT HandleNumber);
extern _reentrant Fflush(INT HandleNumber);
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
//extern _reentrant INT filegetdate(INT HandleNumber,INT crt_mod_date_time_para,DIR_DATE *dirdate,DIR_TIME *dirtime);
extern _reentrant INT filegetattrib(_packed char *FilePath);
extern _reentrant INT FileSystemPresent(INT DeviceNum);
extern _reentrant INT CheckspaceinRootdir(INT Handlenumber,INT count);
extern _reentrant LONG FgetFastHandle(INT HandleNumber);

extern  _reentrant void Setshortfilename(INT HandleNumber,INT *Buffer,_packed char *shortfilename);
extern _reentrant INT ArrangeLongFileName(INT HandleNumber,_packed char *filename,INT count,INT chksum);
extern  _reentrant INT IsShortNameValid(_packed char *Buffer,INT length,INT index);

extern _reentrant INT ChkSum(_packed char *filename);
extern INT Short_NameRes_Ch[];  // sdk3.05 moved these from explicit _X to default Y mem space. TOVERIFY2 ensure that references don't refer to _X now.
extern INT Long_NameRes_Ch[];   // "
extern _reentrant void UnicodeToOEM(_packed char *file,_packed char *shortname,INT length,INT index);

extern _reentrant INT getcontentcode(INT Type,_packed char *filepath,INT OUTcode);

extern _reentrant INT Checkcode(_packed char *buf,INT Count);
extern _reentrant INT CreateShortDirRecord(INT *filename,INT HandleNumber,LONG ClusterNumber,INT DirAttr);
extern _reentrant INT CheckVolumeCode(INT HandleNumber);
extern _reentrant INT Convert_itoa(INT Number,_packed char *string);
extern _reentrant INT GetChar(_packed char *Buffer,INT *offset);
extern _reentrant void PutChar(_packed char *Buffer,INT *offset,INT Char);
extern _reentrant void DBCStoTwoByteString(_packed char *filename,INT *string,INT length,INT index);
extern _reentrant INT DiscardTrailigPeriods(_packed char *Buffer,INT length,INT index,INT flag);
extern _reentrant void GetDBCSstring(_packed char *filename,INT *string);
extern _reentrant INT  DiscardTrailigPeriodsw(_packed char *Buffer,INT length,INT index);
extern _reentrant  INT Extractfilenamew(_packed char *filepath, INT *index);
extern _reentrant INT IsCurrWorkDir(INT HandleNumber);
extern INT _reentrant ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern _reentrant INT GetUnicodeWord(INT *Buffer,INT LFNOffset);
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);
extern _reentrant LONG GetRootdirkey(INT DeviceNumber);
extern _reentrant INT strcpyUCS3_2(INT *filepath_UCS3,INT *filepath_UCS2, INT index, INT length);
extern _reentrant INT StrlengthUCS3(_packed char *filepath);
extern _reentrant INT  DiscardTrailigPeriodsUCS3(_packed char *Buffer,INT length,INT index);
extern  INT _reentrant  GetVolumeLabel(_packed char *Buffer,INT DeviceNum);
extern _reentrant INT SetVolumeLabel(_packed char *Buffer,INT DeviceNum);

extern _reentrant void UpdateFSInfo (void); // sdk2.600 had this line but sdk3.05 did not. New func from TH.
